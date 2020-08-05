#include "stdafx.h"
#include "MessageHandler.h"
#include "Logging.h"

CMessageHandler::CMessageHandler()
{
}

CMessageHandler::~CMessageHandler()
{
}

void CMessageHandler::Open(HWND a_hwndParent)
{
    m_hwndParent = a_hwndParent;
}

void CMessageHandler::Close()
{
}

void CMessageHandler::OnRecieveMessage(CPipeServer * pPipeServer, IGTGenObjPtr pGenObjPtr)
{
    CRITICAL_SECTION oCriticalSection;

    InitializeCriticalSection(&oCriticalSection);

    try
    {
        m_ptrGenOb = pGenObjPtr;
        _bstr_t str = m_ptrGenOb->Name;
        // g_GTOCtrlObj.setGenObj(m_gpGenObj);
        _bstr_t xmlStr = m_ptrGenOb->Getxml();

        Trace(TRACE_INFO, (LPCTSTR)xmlStr);

        if (_tcsicmp((_TCHAR*)str, _T("config")) == 0)
        {
            //if (S_bFirsConfigMsg)
            //{
            //    // Generate new transaction guid on first configuration message.
            //    S_csTransGuid = GenerateGuid();
            //    S_bFirsConfigMsg = FALSE;
            //}

            Trace(TRACE_INFO, _T("Received Configuration Message"));
            PostStatusMsgToParent(_T("Received Configuration Message"));
            //g_GTOCtrlObj.handleConfiguartionMessage();
        }
        else if (_tcsicmp((_TCHAR*)str, _T("itemDetail")) == 0)
        {
            //mapMetaData[_T("ssbTransGuid")] = S_csTransGuid;
            //mapMetaData[_T("ssbItemGuid")] = GenerateGuid();
            //mapMetaData[_T("eventType")] = _T("PA");

            Trace(TRACE_INFO, _T("Received itemDetail Message"));
            PostStatusMsgToParent(_T("Received itemDetail Message"));
            //S_bFirsConfigMsg = TRUE;
            //g_GTOCtrlObj.handleitemDetail();
        }
        else if (_tcsicmp((_TCHAR*)str, _T("stateData")) == 0)
        {
            Trace(TRACE_INFO, _T("Received State Message"));
            PostStatusMsgToParent(_T("Received stateData Message"));
            //g_GTOCtrlObj.handlestateDataMessage();
        }
        else if (_tcsicmp((_TCHAR*)str, _T("getMatchingItems")) == 0)
        {
            //mapMetaData[_T("ssbTransGuid")] = S_csTransGuid;
            //mapMetaData[_T("eventType")] = _T("PLA");

            Trace(TRACE_INFO, _T("Received getMatchingItems message"));
            PostStatusMsgToParent(_T("Received getMatchingItems Message"));
            //S_bFirsConfigMsg = TRUE;
            //g_GTOCtrlObj.handlegetMatchingItems();
        }
        else if (_tcsicmp((_TCHAR*)str, _T("dbManagement")) == 0)
        {
            Trace(TRACE_INFO, _T("Received dbManagement message"));
            PostStatusMsgToParent(_T("Received getMatchingItems Message"));
            //g_GTOCtrlObj.handledbManagement();
        }

        m_ptrGenOb = NULL;
    }
    catch (_com_error& e)
    {
        Trace(TRACE_ERROR, _T("Com Error Handling Pipe Server Message %s"), e.ErrorMessage());
        //g_GTOCtrlObj.Stop();
    }
    catch (...)
    {
        Trace(TRACE_ERROR, _T("Unknow Exception Handling Pipe Server Message"));
        //g_GTOCtrlObj.Stop();
    }

    LeaveCriticalSection(&oCriticalSection);

    return;
}

void CMessageHandler::OnConfigMsg(IGTGenObjPtr pGenObjPtr)
{
    /*
    try
    {
        if (pGenObjPtr != NULL)
        {
            bool bInvalidParam = false;

            CString csName = CString((LPCTSTR)pGenObjPtr->GetAttribute(_T("/"), _T("_name")));

            Trace(TRACE_INFO, _T("Configuration recieved for %s"), csName.GetString());

            // m_vCameraIdList.clear();

            if (csName.CompareNoCase(_T("PA")) == 0)
            {
                //SSBPACONFIG stPAConfig = { false, //bEnable
                //    false, //bCenterDetection
                //    false, //bScaleMetric
                //    PA_MODE_0, //nMode
                //    0 //ltimeout
                //};

                CString csTimeout = CString((LPCTSTR)GetObjectValue(_T("timeout")));
                CString csCenterDetection = CString((LPCTSTR)GetObjectValue(_T("centerDetection")));
                CString csEnable = CString((LPCTSTR)GetObjectValue(_T("enable")));
                CString csScaleMetric = CString((LPCTSTR)GetObjectValue(_T("scalemetric")));
                CString csMode = CString((LPCTSTR)GetObjectValue(_T("mode")));

                if (csTimeout.IsEmpty() || csCenterDetection.IsEmpty() || csEnable.IsEmpty() || csScaleMetric.IsEmpty() || csMode.IsEmpty())
                {
                    //sendConfigResponse(-1, stPAConfig);
                    return;
                }
                if (csTimeout.SpanIncluding(_T("0123456789")) == csTimeout)
                {
                    //stPAConfig.ltimeout = _ttol(csTimeout.GetString());
                }
                else
                {
                    bInvalidParam = true;
                }
                if (csCenterDetection.CompareNoCase(_T("on")) == 0)
                {
                    //stPAConfig.bCenterDetection = true;
                }
                else if (csCenterDetection.CompareNoCase(_T("off")) == 0)
                {
                    //stPAConfig.bCenterDetection = false;
                }
                else
                {
                    bInvalidParam = true;
                }

                if (csEnable.CompareNoCase(_T("on")) == 0)
                {
                    // stPAConfig.bEnable = true;
                }
                else if (csEnable.CompareNoCase(_T("off")) == 0)
                {
                    // stPAConfig.bEnable = false;
                }
                else
                {
                    bInvalidParam = true;
                }

                if (csScaleMetric.CompareNoCase(_T("on")) == 0)
                {
                    // stPAConfig.bScaleMetric = true;
                }
                else if (csScaleMetric.CompareNoCase(_T("off")) == 0)
                {
                    // stPAConfig.bScaleMetric = false;
                }
                else
                {
                    bInvalidParam = true;
                }

                if (csMode.CompareNoCase(_T("aggressive")) == 0)
                {
                    // stPAConfig.nMode = PA_MODE_3;
                }
                else if (csMode.CompareNoCase(_T("conservative")) == 0)
                {
                    // stPAConfig.nMode = PA_MODE_0;
                }
                else
                {
                    if (csMode.SpanIncluding(_T("0123456789")) == csMode)
                    {
                        // stPAConfig.nMode = std::atol(csMode.GetString());
                        stPAConfig.nMode = _ttol(csMode.GetString());
                    }
                    else
                    {
                        bInvalidParam = true;
                    }
                    if (stPAConfig.nMode < 0 || stPAConfig.nMode > 4)
                        bInvalidParam = true;
                }

                if (bInvalidParam)
                {
                    sendConfigResponse(-1, stPAConfig);
                    return;
                }
                //SSBRESSTATUS enRes = SetSSBPAConfig(stPAConfig);
                //if(enRes == SUCCESS)
                //{
                sendConfigResponse(0, stPAConfig);
                //}
                //else
                //{
                //	sendConfigResponse(1, stPAConfig);
                //}
            }
            else if (csName.CompareNoCase(_T("PLA")) == 0)
            {
                SSBPLACONFIG stPLAConfig = { 0,		// no of items
                    false, //bEnable
                    false, //bCenterDetection
                    false, // bValidation
                    false, //bScaleMetric
                    0 //ltimeout
                };
                CString csTimeout = CString((LPCTSTR)GetObjectValue(_T("timeout")));
                CString csCenterDetection = CString((LPCTSTR)GetObjectValue(_T("centerDetection")));
                CString csEnable = CString((LPCTSTR)GetObjectValue(_T("enable")));
                CString csValidation = CString((LPCTSTR)GetObjectValue(_T("validation")));
                CString csNumofItems = CString((LPCTSTR)GetObjectValue(_T("numberofItems")));
                CString csScaleMetric = CString((LPCTSTR)GetObjectValue(_T("scalemetric")));
                if (csTimeout.IsEmpty() || csCenterDetection.IsEmpty() || csEnable.IsEmpty() || csScaleMetric.IsEmpty() ||
                    csValidation.IsEmpty() || csNumofItems.IsEmpty())
                {
                    sendConfigResponse(-1, stPLAConfig);
                    return;
                }

                if (csTimeout.SpanIncluding(_T("0123456789")) == csTimeout)
                {
                    // stPLAConfig.ltimeout = std::atol(csTimeout.GetString());
                    stPLAConfig.ltimeout = _ttol(csTimeout.GetString());
                }
                else
                {
                    bInvalidParam = true;
                }

                if (csCenterDetection.CompareNoCase(_T("on")) == 0)
                {
                    stPLAConfig.bCenterDetection = true;
                }
                else if (csCenterDetection.CompareNoCase(_T("off")) == 0)
                {
                    stPLAConfig.bCenterDetection = false;
                }
                else
                {
                    bInvalidParam = true;
                }
                if (csEnable.CompareNoCase(_T("on")) == 0)
                {
                    stPLAConfig.bEnable = true;
                }
                else if (csEnable.CompareNoCase(_T("off")) == 0)
                {
                    stPLAConfig.bEnable = false;
                }
                else
                {
                    bInvalidParam = true;
                }

                if (csScaleMetric.CompareNoCase(_T("on")) == 0)
                {
                    stPLAConfig.bScaleMetric = true;
                }
                else if (csScaleMetric.CompareNoCase(_T("off")) == 0)
                {
                    stPLAConfig.bScaleMetric = false;
                }
                else
                {
                    bInvalidParam = true;
                }

                if (csNumofItems.SpanIncluding(_T("0123456789")) == csNumofItems)
                {
                    stPLAConfig.noofitems = _ttoi(csNumofItems.GetString());
                    // stPLAConfig.noofitems = std::atoi(csNumofItems.GetString());
                }
                else
                {
                    bInvalidParam = true;
                }

                if (csValidation.CompareNoCase(_T("on")) == 0)
                {
                    stPLAConfig.bValEnable = true;
                }
                else if (csValidation.CompareNoCase(_T("off")) == 0)
                {
                    stPLAConfig.bValEnable = false;
                }
                else
                {
                    bInvalidParam = true;
                }

                if (bInvalidParam)
                {
                    sendConfigResponse(-1, stPLAConfig);
                    return;
                }

                //            SSBRESSTATUS enRes = SetSSBPLAConfig(stPLAConfig);
                //if(enRes == SUCCESS)
                //{
                sendConfigResponse(0, stPLAConfig);
                //}
                //else
                //{
                //	sendConfigResponse(1, stPLAConfig);
                //}
            }
            else
            {
                sendConfigResponse();
                TRACE(_T("Invalid Configuration recieved"));
            }
        }
    }
    catch (_com_error& e)
    {
        Trace(TRACE_ERROR, _T("Com Error Handling Pipe Server Message %s"), e.ErrorMessage());
    }
    catch (...)
    {
        Trace(TRACE_ERROR, _T("Unknown Exception Handling Pipe Server Message"));
    }
    */

}

void CMessageHandler::PostStatusMsgToParent(const CString a_csStatusMsg)
{
    ::PostMessage(m_hwndParent, WM_USER_UPDATE_STATUS, 0, reinterpret_cast<LPARAM>(new CString(a_csStatusMsg))); \
}
