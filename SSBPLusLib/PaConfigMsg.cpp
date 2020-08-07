#include "stdafx.h"
#include "PaConfigMsg.h"
#include "Logging.h"

CPaConfigMsg::CPaConfigMsg() : 
    hdr(L""),
    bEnable(false),
    bCenterDetection(false),
    bScaleMetric(false),
    eMode(ePA_MODE_0_CONSERVATIVE),
    ltimeout(0),
    bMaskTransId(false),  // for unit testing support only
    m_eStatus(eGeneralFailure)
{
}

CPaConfigMsg::~CPaConfigMsg()
{
}

void CPaConfigMsg::Set(IGTGenObjPtr a_ptrGenObj)
{
    try
    {
        if (a_ptrGenObj != NULL)
        {
            bool bInvalidParam = false;

            CString csName = (LPCTSTR)a_ptrGenObj->GetAttribute(_T("/"), _T("_name"));

            Trace(TRACE_INFO, _T("Configuration recieved for %s"), csName.GetString());

            if (csName.CompareNoCase(_T("PA")) == 0)
            {
                hdr = a_ptrGenObj->GetXml(_T("/hdr"));

                CString csTimeout = (LPCTSTR)GetObjectValue(a_ptrGenObj, _T("timeout"));
                CString csCenterDetection = (LPCTSTR)GetObjectValue(a_ptrGenObj, _T("centerDetection"));
                CString csEnable = (LPCTSTR)GetObjectValue(a_ptrGenObj, _T("enable"));
                CString csScaleMetric = (LPCTSTR)GetObjectValue(a_ptrGenObj, _T("scalemetric"));
                CString csMode = (LPCTSTR)GetObjectValue(a_ptrGenObj, _T("mode"));

                if (csTimeout.IsEmpty() || csCenterDetection.IsEmpty() || csEnable.IsEmpty() || csScaleMetric.IsEmpty() || csMode.IsEmpty())
                {
                    bInvalidParam = true;
                }
                else
                {
                    if (csTimeout.SpanIncluding(_T("0123456789")) == csTimeout)
                    {
                        ltimeout = _ttol(csTimeout.GetString());
                    }
                    else
                    {
                        bInvalidParam = true;
                    }
                    if (csCenterDetection.CompareNoCase(_T("on")) == 0)
                    {
                        bCenterDetection = true;
                    }
                    else if (csCenterDetection.CompareNoCase(_T("off")) == 0)
                    {
                        bCenterDetection = false;
                    }
                    else
                    {
                        bInvalidParam = true;
                    }

                    if (csEnable.CompareNoCase(_T("on")) == 0)
                    {
                        bEnable = true;
                    }
                    else if (csEnable.CompareNoCase(_T("off")) == 0)
                    {
                        bEnable = false;
                    }
                    else
                    {
                        bInvalidParam = true;
                    }

                    if (csScaleMetric.CompareNoCase(_T("on")) == 0)
                    {
                        bScaleMetric = true;
                    }
                    else if (csScaleMetric.CompareNoCase(_T("off")) == 0)
                    {
                        bScaleMetric = false;
                    }
                    else
                    {
                        bInvalidParam = true;
                    }

                    if (csMode.CompareNoCase(_T("aggressive")) == 0)
                    {
                        eMode = ePA_MODE_3_AGGRESSIVE;
                    }
                    else if (csMode.CompareNoCase(_T("conservative")) == 0)
                    {
                        eMode = ePA_MODE_0_CONSERVATIVE;
                    }
                    else
                    {
                        if (csMode.SpanIncluding(_T("0123456789")) == csMode)
                        {
                            // stPAConfig.eMode = std::atol(csMode.GetString());
                            eMode = (ePAMODE)_ttol(csMode.GetString());
                        }
                        else
                        {
                            bInvalidParam = true;
                        }
                        if (eMode < 0 || eMode > 4)
                            bInvalidParam = true;
                    }

                    if (bInvalidParam)
                    {
                        m_eStatus = eStatusInvalidParameter;
                    }
                    else
                    {
                        m_eStatus = eStatusOk;
                    }
                }
            }
            else
            {
                m_eStatus = eGeneralFailure;
                TRACE(_T("Invalid Configuration recieved"));
            }
        }
    }
    catch (_com_error& e)
    {
        Trace(TRACE_ERROR, _T("Com Error Handling Pipe Server Message %s"), e.ErrorMessage());
        m_eStatus = eGeneralFailure;
    }
    catch (...)
    {
        Trace(TRACE_ERROR, _T("Unknown Exception Handling Pipe Server Message"));
        m_eStatus = eGeneralFailure;
    }
}

IGTGenObjPtr CPaConfigMsg::Get()
{
    HRESULT hr = S_OK;
    IGTGenObjPtr ptrGenObj;

    hr = ptrGenObj.CreateInstance(__uuidof(CGTGenObj));
    if (FAILED(hr))
    {
        // log failure.
        Trace(TRACE_ERROR, _T("Error! Failed to create instance of PAConfigResponse gen obj, hr=0x%x"), hr);
    }
    else
    {
        ptrGenObj->Name = _T("configResponse");
        ptrGenObj->IsXMLOutput = true;
        ptrGenObj->Validate = false;
        ptrGenObj->MsgId = 0xCF;
        ptrGenObj->IsNamedElementsXML = true;

        if (eStatusOk == eStatusOk)
        {
            ptrGenObj->SetValue(_T("status"), _T("success"));
        }
        else if (eStatusOk == eStatusInvalidParameter)
        {
            ptrGenObj->SetValue(_T("status"), _T("failure"));
            ptrGenObj->SetValue(_T("error"), _T("Parameter value is empty/not as expected"));
        }
        else if (eStatusOk == eGeneralFailure)
        {
            ptrGenObj->SetValue(_T("status"), _T("failure"));
            ptrGenObj->SetValue(_T("error"), _T("Unknown SSB Error"));
        }

        ptrGenObj->SetAttribute(_T("/"), _T("_name"), _T("PA"));
        ptrGenObj->SetXml(_T("/hdr"), hdr);

        if (bMaskTransId)
        {
            ptrGenObj->SetValue(_T("hdr/transID"), L"*" );
        }
        else
        {
            ptrGenObj->SetValue(_T("hdr/transID"), GetCurrentDateTime().c_str());
        }

        if (eStatusOk != eStatusInvalidParameter)
        {
            ptrGenObj->SetValue(_T("timeout"), ltimeout);
            ptrGenObj->SetValue(_T("centerDetection"), bCenterDetection ? _T("on") : _T("off"));
            ptrGenObj->SetValue(_T("enable"), bEnable ? _T("on") : _T("off"));
            ptrGenObj->SetValue(_T("scalemetric"), bScaleMetric ? _T("on") : _T("off"));
            ptrGenObj->SetValue(_T("mode"), eMode);
        }
    }
    return ptrGenObj;
}

_bstr_t CPaConfigMsg::GetObjectValue(IGTGenObjPtr a_ptrGenObj, LPWSTR a_szName)
{
    _variant_t var;
    _bstr_t str = _T("");

    try
    {
        var = a_ptrGenObj->GetValue2(_bstr_t(a_szName));
        str = var.bstrVal;
    }
    catch (_com_error& ex)
    {
        Trace(TRACE_ERROR, _T("Unable to get object value: '%s', error: %s"), a_szName, ex.ErrorMessage());
    }
    catch (...)
    {
        Trace(TRACE_ERROR, _T("Unable to get object value: '%s' , Unknow Exception"), a_szName );
    }
    return str;
}

BOOL CPaConfigMsg::SetObjectValue(IGTGenObjPtr a_ptrGenObj, LPWSTR a_szName, _bstr_t a_szValue)
{
    a_ptrGenObj->SetValue(a_szName, a_szValue);
    return 0;
}

const std::string CPaConfigMsg::GetCurrentDateTime()
{
    SYSTEMTIME st;

    GetSystemTime(&st);

    char currentTime[84] = "";

    sprintf_s(currentTime, "%.4d-%.2d-%.2dT%.2d:%.2d:%.2d.%.3dZ", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

    return std::string(currentTime);
}
