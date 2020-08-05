#include "stdafx.h"
#include "PaConfig.h"
#include "Logging.h"


CPaConfig::CPaConfig() : m_eStatus(eGeneralFailure)
{
}


CPaConfig::~CPaConfig()
{
}

void CPaConfig::Set(IGTGenObjPtr pGenObjPtr)
{
    name = (LPCTSTR)pGenObjPtr->GetAttribute(_T("/"), _T("_name"));

    hdr = (LPCTSTR)pGenObjPtr->GetXml(_T("/hdr"));

    transId = (LPCTSTR)GetObjectValue(pGenObjPtr, _T("hdr/transId"));
    computerName = (LPCTSTR)GetObjectValue(pGenObjPtr, _T("hdr/computerName"));
    sequenceID = (LPCTSTR)GetObjectValue(pGenObjPtr, _T("hdr/sequenceID"));
    lane = (LPCTSTR)GetObjectValue(pGenObjPtr, _T("hdr/lane"));

    timeout = (LPCTSTR)GetObjectValue(pGenObjPtr, _T("timeout"));
    centerDetection = (LPCTSTR)GetObjectValue(pGenObjPtr, _T("centerDetection"));
    enable = (LPCTSTR)GetObjectValue(pGenObjPtr, _T("enable"));
    mode = (LPCTSTR)GetObjectValue(pGenObjPtr, _T("mode"));
    scalemetric = (LPCTSTR)GetObjectValue(pGenObjPtr, _T("scalemetric"));

    m_eStatus = eStatusOk;

}

IGTGenObjPtr CPaConfig::Get()
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

        ptrGenObj->SetAttribute(_T("/"), _T("_name"), name.GetString());
        ptrGenObj->SetXml(_T("/hdr"), hdr.GetString());

        ptrGenObj->SetValue(_T("hdr/transID"), currentDateTime().c_str());

        if (eStatusOk != eStatusInvalidParameter)
        {
            ptrGenObj->SetValue(_T("timeout"), timeout.GetString());
            ptrGenObj->SetValue(_T("centerDetection"), centerDetection.GetString());
            ptrGenObj->SetValue(_T("enable"), enable.GetString());
            ptrGenObj->SetValue(_T("scalemetric"), scalemetric.GetString());
            ptrGenObj->SetValue(_T("mode"), mode.GetString());
        }
    }
    return ptrGenObj;
}

_bstr_t CPaConfig::GetObjectValue(IGTGenObjPtr a_ptrGenObjPtr, LPWSTR a_szName)
{
    _variant_t var;
    _bstr_t str = _T("");

    try
    {
        var = a_ptrGenObjPtr->GetValue2(_bstr_t(a_szName));
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

BOOL CPaConfig::SetObjectValue(IGTGenObjPtr a_ptrGenObjPtr, LPWSTR a_szName, _bstr_t a_szValue)
{
    a_ptrGenObjPtr->SetValue(a_szName, a_szValue);
    return 0;
}

BOOL CPaConfig::Validate()
{
    if (timeout.GetLength() == 0 ||
        centerDetection.GetLength() == 0 ||
        enable.GetLength() == 0 ||
        mode.GetLength() == 0 ||
        scalemetric.GetLength() == 0)
    {
        m_eStatus = eStatusInvalidParameter;
    }
    else if (timeout.SpanIncluding(_T("0123456789")) == timeout)
    {

    }
    return 0;
}

const std::string CPaConfig::currentDateTime()
{
    SYSTEMTIME st;

    GetSystemTime(&st);

    char currentTime[84] = "";

    sprintf_s(currentTime, "%.4d-%.2d-%.2dT%.2d:%.2d:%.2d.%.3dZ", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);

    return std::string(currentTime);
}
