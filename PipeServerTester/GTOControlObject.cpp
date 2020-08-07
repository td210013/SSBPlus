#include "StdAfx.h"
#include "GTOControlObject.h"
#include "string"
#include "Logging.h"
#include <fstream>
#include <streambuf>
#include "Base64EncodeDecode.h"
#include "Config.h"
#include "algorithm"
#include "Resource.h"
#include "map"

// #include "SSBErrors.h"
//
#pragma comment(lib, "mpr.lib")

using namespace std;
#define DEFAULT_DELAY 1;

IGTConnectPtr CGTOControlObject::m_gPosConnection = NULL;
IGTGenObjPtr CGTOControlObject::m_gpGenObj = NULL;
HWND CGTOControlObject::m_ParentHwnd = NULL;

const std::string currentDateTime() 
{
	SYSTEMTIME st;

	GetSystemTime(&st);

	char currentTime[84] = "";

	sprintf_s(currentTime,"%.4d-%.2d-%.2dT%.2d:%.2d:%.2d.%.3dZ",st.wYear,st.wMonth,st.wDay, st.wHour, st.wMinute, st.wSecond , st.wMilliseconds);

	return string(currentTime); 
}

const std::string currentDate()
{
    SYSTEMTIME st;

    GetSystemTime(&st);

    char currentDate[84] = "";

    sprintf_s(currentDate, "%.4d-%.2d-%.2d", st.wYear, st.wMonth, st.wDay);

    return string(currentDate);
}

const std::string currentFilename()
{
    SYSTEMTIME st;

    GetSystemTime(&st);

    char currentDate[84] = "";

    sprintf_s(currentDate, "PipeserverTester_Msgs_%.4d-%.2d-%.2d", st.wYear, st.wMonth, st.wDay);

    return string(currentDate);
}


/// <summary>
/// Initializes the GTO Control.
/// </summary>
/// <returns>TRUE/FALSE</returns>
BOOL CGTOControlObject::InitializeGTOCO(HWND a_ParentHwnd)
{
	BOOL bResult = TRUE;
	try
	{

		CoInitialize(NULL);
		HRESULT hr;
		 IGTGenObjPtr item;
		// Create the GTO control instance
		hr = item.CreateInstance(__uuidof( CGTGenObj));
		if (hr != S_OK)
		{
			item.Release();
			throw _com_error(hr);
		}

		item.Release();
		// Setup the connection
		hr = SetupConnection();
		if (hr != S_OK)
		{
			throw _com_error(hr);
		}

        m_ParentHwnd = a_ParentHwnd;

	}
	catch(_com_error& er)
	{
		bResult = FALSE;
		// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Unable to Initialize the GTO Control %s"), er.ErrorMessage());
        Trace(TRACE_ERROR, _T("Unable to Initialize the GTO Control %s"), er.ErrorMessage());
	}
	catch(...)
	{
		bResult = FALSE;
		// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Unable to Initialize the GTO Control Unknow Exception"));
        TRACE("Unable to Initialize the GTO Control Unknow Exception");
	} 
	return bResult;
}
/// <summary>
/// Unintializes the GTO Control.
/// </summary>
/// <returns>TRUE/FALSE</returns>
BOOL CGTOControlObject::UnintializeGTOCO()
{
	g_GTOCtrlObj.ReleaseGenObj();
	if(CGTOControlObject::m_gpGenObj != NULL)
	{
		CGTOControlObject::m_gpGenObj.Release();
		CGTOControlObject::m_gpGenObj = NULL;
	}
	if(CGTOControlObject::m_gPosConnection != NULL)
	{
		CGTOControlObject::m_gPosConnection->Disconnect();
		CGTOControlObject::m_gPosConnection.Release();
		CGTOControlObject::m_gPosConnection = NULL;
		g_GTOCtrlObj.setPosConnection(NULL);
	}

	CoUninitialize();
	return TRUE;
}
/// <summary>
/// Releases the GTO object.
/// </summary>
void CGTOControlObject::ReleaseGenObj()
{
    if (m_pGenObj != NULL)
    {
        m_pGenObj.Release();
		m_pGenObj = NULL;
    }
	LeaveCriticalSection(&m_CSvariable);
}

void CGTOControlObject::setPosConnection( IGTConnectPtr posConnection)
{
		m_posConnection = posConnection;
}

/// <summary>
/// Sets the Pipeserver connection.
/// </summary>
/// <returns>COM Error</returns>
HRESULT CGTOControlObject::SetupConnection()
{
	HRESULT hr = S_OK;

    hr = m_gPosConnection.CreateInstance(__uuidof( CGTConnect));
	if(hr != S_OK)
		throw _com_error(hr);

		
	//Read Pipeserver details from the registry
	_TCHAR szServerName[_MAX_PATH+1]; 
	_TCHAR szReadPort[_MAX_PATH+1];
	_TCHAR szWritePort[_MAX_PATH+1]; 

	HKEY  hKey;
	long  lRC;
	DWORD dwDataSize = _MAX_PATH;
	long lReadPort = 0;
	long lWritePort = 0;

	lRC = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\NCR\\PipeServerConnect\\PipeServerConnections\\SecureCam"),
				0, KEY_QUERY_VALUE, &hKey);
    if (lRC != ERROR_SUCCESS)
    {
        Trace(TRACE_ERROR, _T("Failed to open registry, error code: %ld"), lRC);
        UnintializeGTOCO();
        return E_FAIL;

    }
	lRC = ::RegQueryValueEx(hKey,
			_T("ServerName"),
			NULL,
			NULL,
			(LPBYTE)szServerName,
			&dwDataSize);
	if(lRC != ERROR_SUCCESS)
    {
        Trace(TRACE_ERROR, _T("Failed to open registry, error code: %ld"), lRC);
        UnintializeGTOCO();
        return E_FAIL;

    }
    dwDataSize = _MAX_PATH;
	
	lRC = ::RegQueryValueEx(hKey,
			_T("SocketReadPort"),
			NULL,
			NULL,
			(LPBYTE)szReadPort,
			&dwDataSize);

    if(lRC != ERROR_SUCCESS)
    {
        Trace(TRACE_ERROR, _T("Failed to open registry, error code: %ld"), lRC);
        UnintializeGTOCO();
        return E_FAIL;

    }

    dwDataSize = _MAX_PATH;

	lRC = ::RegQueryValueEx(hKey,
			_T("SocketWritePort"),
			NULL,
			NULL,
			(LPBYTE)szWritePort,
			&dwDataSize);

	if(lRC != ERROR_SUCCESS)
    {
        Trace(TRACE_ERROR, _T("Failed to open registry, error code: %ld"), lRC);
        UnintializeGTOCO();
        return E_FAIL;

    }

    RegCloseKey(hKey);
	_bstr_t bstrDevices = szServerName;
	lReadPort = _tstol(szReadPort);
	lWritePort = _tstol(szWritePort);

    //Subscribe to the Messages
    hr = m_gPosConnection->SubscribeTo(GetConfig()->csMessageSubscriptionList.GetString());

    // _bstr_t(_T("CA,CB,C6,C7,C9")));

	if(hr != S_OK)
    {
        Trace(TRACE_ERROR, _T("Failed to subscribe to pipeserver messages: : %s"), GetConfig()->csMessageSubscriptionList.GetString());
        UnintializeGTOCO();
        return E_FAIL;

    }

	// Connect to the Pipeserver
	hr = m_gPosConnection->ConnectEx(bstrDevices, lReadPort, lWritePort, _bstr_t(_T("")));
	if(hr != S_OK)
    {
        Trace(TRACE_ERROR, _T("Failed to connect to pipeserver, error code: %x"), hr);
        UnintializeGTOCO();
        return E_FAIL;
    }

	g_GTOCtrlObj.setPosConnection(m_gPosConnection);

	LPEVENTCALLBACKPROC pCallback = (LPEVENTCALLBACKPROC)Callback;
	_variant_t vtCallback;
	vtCallback.byref= (void*)pCallback;
	vtCallback.vt = VT_BYREF | VT_I4;
	_variant_t vtUser;
	vtUser.byref= (void*)NULL;
	vtUser.vt = VT_BYREF | VT_I4;
	// Set the Call back Function
	hr = m_gPosConnection->SetCallback(vtCallback, vtUser);
	if(hr != S_OK)
    {
        Trace(TRACE_ERROR, _T("Failed to connect to pipeserver, error code: %x"), hr);
        UnintializeGTOCO();
        return E_FAIL;
    }
	return hr;
}

/// <summary>
/// Initializes a new instance of the <see cref="CGTOControlObject"/> class.
/// </summary>
CGTOControlObject::CGTOControlObject(void)
{
	m_posConnection = NULL;
	m_pGenObj = NULL;
	//m_hThread = NULL;
	//m_hlFrequency = 2500;
	InitializeCriticalSection(&m_CSvariable);
	//m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
}

/// <summary>
/// Finalizes an instance of the <see cref="CGTOControlObject"/> class.
/// </summary>
CGTOControlObject::~CGTOControlObject(void)
{
	DeleteCriticalSection(&m_CSvariable);
}

/// <summary>
/// Sends the stream to Pipeserver.
/// </summary>
/// <param name="pGenObj">GTO Control object.</param>
void CGTOControlObject::sendStream( IGTGenObjPtr pGenObjPtr)
{
    IGTStreamPtr stream = pGenObjPtr;
	try
	{
	   m_posConnection->Send(stream);
	}
	catch(_com_error& er)
	{
		// g_cmDc.DoDCPrintf(TRACE_ALWAYS, _T("Unable to send the stream over pipe server,%s, Please restart IMA"), er.ErrorMessage() );
        Trace(TRACE_ERROR, _T("Unable to send the stream over pipe server,%s, Please restart IMA"), er.ErrorMessage());
	}
	catch(...)
	{
		// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Unable to send the stream over pipe server, Unhandled Exception. Please restart IMA"));
        Trace(TRACE_ERROR, _T("Unable to send the stream over pipe server, Unhandled Exception. Please restart IMA"));
	}
}

/// <summary>
/// Sends the stream to Pipeserver.
/// </summary>
/// <param name="a_bstrtXml">Message XML.</param>
void CGTOControlObject::sendStream(_bstr_t a_bstrtName, _bstr_t a_bstrtXml)
{
    try
    {
        HRESULT hr = S_OK;
        IGTGenObjPtr ptrGenObj;

        hr = ptrGenObj.CreateInstance(__uuidof(CGTGenObj));

        // ptrGenObj->Name = a_bstrtName;
        //		 ptrGenObj->IsXMLOutput = true;
        //		 ptrGenObj->Validate = false;
        //		 ptrGenObj->MsgId = 0xCF;
        //		 ptrGenObj->IsNamedElementsXML = true;
        //		 ptrGenObj->IsXMLOutput = true;
        //		 ptrGenObj->Validate = false;

        if (SUCCEEDED(hr))
        {
            ptrGenObj->SetXml("/", a_bstrtXml);

            ptrGenObj->Name = a_bstrtName;
            ptrGenObj->IsXMLOutput = true;
            ptrGenObj->Validate = false;
            //ptrGenObj->MsgId = 0xCF;
            ptrGenObj->IsNamedElementsXML = true;

            _bstr_t bstrtTest = ptrGenObj->GetXml(L"/");

            IGTStreamPtr stream = ptrGenObj;

            g_GTOCtrlObj.m_posConnection->Send(stream);
        }
        else
        {
            Trace(TRACE_ERROR, _T("Failed to create generic object, hr=0x%x"), hr);
        }
    }
    catch (_com_error& er)
    {
        ASSERT(0);
        // g_cmDc.DoDCPrintf(TRACE_ALWAYS, _T("Unable to send the stream over pipe server,%s, Please restart IMA"), er.ErrorMessage() );
        Trace(TRACE_ERROR, _T("Unable to send the stream over pipe server,%s, Please restart IMA"), er.ErrorMessage());
    }
    catch (...)
    {
        ASSERT(0);
        // g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Unable to send the stream over pipe server, Unhandled Exception. Please restart IMA"));
        Trace(TRACE_ERROR, _T("Unable to send the stream over pipe server, Unhandled Exception. Please restart IMA"));
    }
}

//HRESULT CGTOControlObject::SendPLAMessage(PSSBPLARESPONSE pRes)
//{
//	HRESULT hr =S_OK;
//	IGTGenObjPtr PLAMessage;
//	IGTGenObjPtr PLAList;
//	try
//	{
//		
//		 //setup xml for GTO object
//		 hr = PLAMessage.CreateInstance(__uuidof( CGTGenObj));
//		 if(FAILED(hr))
//			 throw _com_error(hr);
//		
//		 hr = PLAList.CreateInstance(__uuidof( CGTGenObj));
//		 if(FAILED(hr))
//			 throw _com_error(hr);
//			
//		 PLAMessage->Name = _T("getMatchingItemsResponse");
//		 PLAMessage->IsXMLOutput = true;
//		 PLAMessage->Validate = false;
//		 PLAMessage->MsgId = 0xCF;
//		 PLAMessage->IsNamedElementsXML = true;
//		 PLAList->IsXMLOutput = true;
//		 PLAList->Validate = false;
//		 
//		 _bstr_t hdr = m_pGenObj->GetXml(_T("/hdr"));
//		 PLAMessage->SetXml(_T("/hdr"),hdr);
//		 PLAMessage->SetValue(_T("hdr/transID"), currentDateTime().c_str());
//		 PLAMessage->SetValue(_T("name"), _T("Picklist"));
//		 if(pRes->enStatus == SUCCESS)
//		 {
//			 PLAList->Name = _T("items");
//			 PLAList->IsNamedElementsXML = true;
//			 
//			 for(int i =0; i< pRes->number; i++)
//			 {
//				TCHAR csConfidence[10];
//				_itot(pRes->pCandidatelist[i].nConfidence,csConfidence,10);
//				_variant_t varConfidence(csConfidence);
//				 PLAList->SetCurrentNode(_T("/item"),i);
//				 PLAList->SetValue(_T("code"), pRes->pCandidatelist[i].nItemUPC);
//				 PLAList->SetValue(_T("confidence"), varConfidence);
//			 }
//		 	 
//			 PLAMessage->SetXml(_T("/getMatchingItemsResponse"),PLAList->xml);
//			 PLAMessage->SetValue(_T("status"),_T("success"));
//		 }
//		 else if(pRes->enStatus == NOALERT)
//		 {
//			 PLAMessage->SetValue(_T("status"),_T("success"));
//		 }
//		 else if(pRes->enStatus == CENTER)
//		 {
//			 PLAMessage->SetValue(_T("status"),_T("center"));
//		 }
//		 else 
//		 {
//			PLAMessage->SetValue(_T("status"),_T("failed"));
//		 }
//		 _bstr_t xmlStr = PLAMessage->Getxml();
//		 Trace(TRACE_INFO,(LPCTSTR) xmlStr);
//		
//        PostStatusMsgToParent(_T("Sending getMatchingItemsResponse"));
//		sendStream(PLAMessage);
//		PLAList.Release();
//		PLAMessage.Release();
//	}
//	catch(_com_error& er)
//	{
//		Trace(TRACE_ERROR, _T("Unable to send Picklist Assist Message over pipe server,%s"), er.ErrorMessage() );
//        PostStatusMsgToParent(_T("Failed to send getMatchingItemsResponse"));
//    }
//    return hr;
//}

//HRESULT CGTOControlObject::SendAlertMessage(PSSBVALRESPONSE pRes)
//{
//	HRESULT hr =S_OK;
//	IGTGenObjPtr AlertMessage;
//	try
//	{
//    	 //setup xml for GTO object
//		 hr = AlertMessage.CreateInstance(__uuidof( CGTGenObj));
//		 if(FAILED(hr))
//			 throw _com_error(hr);
//  		 //TCHAR csConfidence[10];
//
//		 AlertMessage->Name = _T("verifyItemResponse");
//		 AlertMessage->IsXMLOutput = true;
//		 AlertMessage->Validate = false;
//		 AlertMessage->MsgId = 0xCF;
//		 AlertMessage->IsNamedElementsXML = true;
//		 _bstr_t hdr = m_pGenObj->GetXml(_T("/hdr"));
//		 AlertMessage->SetXml(_T("/hdr"),hdr);
//		 AlertMessage->SetValue(_T("hdr/transID"), currentDateTime().c_str());
//		 
//		 if(pRes->enSource == PA)
//				AlertMessage->SetValue(_T("validationsource"),_T("PA"));
//			else
//				AlertMessage->SetValue(_T("validationsource"),_T("PLA"));
//
//		 //if(nSource == 0)
//		 //if(pRes->enStatus == CENTER)
//		 //{
//			// Trace(TRACE_INFO,_T("Validation Response for ItemUPC : %s found not in center"), pRes->cItemUPC);
//			//AlertMessage->SetValue(_T("status"), _T("center"));
//		 //}
//		 //else if(pRes->enStatus == SUCCESS || pRes->enStatus == NOALERT)
//		 //{
//			AlertMessage->SetValue(_T("status"), _T("success"));
//		 //}
//		 //else if(pRes->enStatus == ALERT) 
//		 //{
//			// Trace(TRACE_INFO,_T("Validation Response for ItemUPC : %s"), pRes->cItemUPC);
//			// AlertMessage->SetValue(_T("item"), pRes->cItemUPC);
//			// CString csImagePath = m_pGenObj->GetValue(_T("hdr/computerName"));
//			// CString csTemp = CString(pRes->cImagePath);
//			// int nCount =  csTemp.Find(':');
//			// if(nCount > 0)
//			// {
//			//	 csImagePath = CString(pRes->cImagePath) ;
//			// }
//			// else
//			// {
//			//	 if(!csImagePath.IsEmpty())
//			//	 {
//			//		 CString csImgPath = _T("\\\\") + csImagePath + csTemp;
//			//		 csImagePath = csImgPath ;
//			//	 }
//			//	 else
//			//	 {
//			//		 csImagePath = CString(pRes->cImagePath) ;
//			//	 }
//			// }
//			// AlertMessage->SetValue(_T("imageuri"),csImagePath.GetString());
//			// AlertMessage->SetValue(_T("status"), _T("alert"));
//		 //}
//		 //else 
//		 //{
//			//AlertMessage->SetValue(_T("status"),_T("failed"));
//		 //}
//		 _bstr_t xmlStr = AlertMessage->Getxml();
//		 Trace(TRACE_INFO,(LPCTSTR) xmlStr);
//		
//		sendStream(AlertMessage);
//		AlertMessage.Release();
//	}
//	catch(_com_error& er)
//	{
//		Trace(TRACE_ERROR, _T("Unable to send Picklist Assist Message over pipe server,%s"), er.ErrorMessage() );
//	}
//    return hr;
//}

//HRESULT CGTOControlObject::SendImage(IMAGEDATA imaData)
//{
//    HRESULT hr = S_OK;
//    IGTGenObjPtr PLAMessage;
//    IGTGenObjPtr PLAList;
//    try
//    {
//
//        //setup xml for GTO object
//        hr = PLAMessage.CreateInstance(__uuidof(CGTGenObj));
//        if (FAILED(hr))
//            throw _com_error(hr);
//
//        hr = PLAList.CreateInstance(__uuidof(CGTGenObj));
//        if (FAILED(hr))
//            throw _com_error(hr);
//
//        PLAMessage->Name = _T("postImage");
//        PLAMessage->IsXMLOutput = true;
//        PLAMessage->Validate = false;
//        PLAMessage->MsgId = 0xD4;
//        PLAMessage->IsNamedElementsXML = true;
//        PLAList->IsXMLOutput = true;
//        PLAList->Validate = false;
//
//        //_bstr_t hdr = m_pGenObj->GetXml(_T("/hdr"));
//        //PLAMessage->SetXml(_T("/hdr"), hdr);
//        //PLAMessage->SetValue(_T("hdr/transID"), currentDateTime().c_str());
//        // PLAMessage->SetValue(_T("name"), _T("Picklist"));
//        PLAMessage->SetValue(_T("imagePath"), _bstr_t( imaData.cImgePath.c_str() ));
//        PLAMessage->SetValue(_T("bitsPerPixel"), imaData.nBitsPerPixel );
//        PLAMessage->SetValue(_T("frameType"), imaData.nFrameType);
//        PLAMessage->SetValue(_T("nImageHeight"), imaData.nImageHeight);
//        PLAMessage->SetValue(_T("nImageLength"), imaData.nImageLength);
//        PLAMessage->SetValue(_T("nImageType"), imaData.nImageType);
//        PLAMessage->SetValue(_T("nImageWidth"), imaData.nImageWidth);
//
//        std::map<CString, CString>::iterator iter;
//
//        for (iter = mapMetaData.begin(); iter != mapMetaData.end(); iter++)
//        {
//            PLAMessage->SetValue(iter->first.GetString(), iter->second.GetString() );
//        }
//
//        int nCameraNum = m_vCameraIdList.back();
//        PLAMessage->SetValue(_T("CamerId"), nCameraNum );
//        m_vCameraIdList.pop_back();
//
//        PLAMessage->SetValue(_T("companyNumber"), GetConfig()->csCompanyNumber.GetString());
//        PLAMessage->SetValue(_T("storeNumber"), GetConfig()->csStoreNumber.GetString());
//        PLAMessage->SetValue(_T("laneNumber"), GetConfig()->csLaneNumber.GetString());
//
//
//        // PLAMessage->SetValue(_T("imageType"), _bstr_t(imaData.cImgePath.c_str()));
//
//        std::string ssBase64;
//
//        GetBase64String(imaData.cImgePath, ssBase64);
//
//        PLAMessage->SetValue(_T("base64Image"), _bstr_t(ssBase64.c_str()));
//
//        /*
//        if (pRes->enStatus == SUCCESS)
//        {
//            PLAList->Name = _T("items");
//            PLAList->IsNamedElementsXML = true;
//
//            for (int i = 0; i< pRes->number; i++)
//            {
//                TCHAR csConfidence[10];
//                _itot(pRes->pCandidatelist[i].nConfidence, csConfidence, 10);
//                _variant_t varConfidence(csConfidence);
//                PLAList->SetCurrentNode(_T("/item"), i);
//                PLAList->SetValue(_T("code"), pRes->pCandidatelist[i].nItemUPC);
//                PLAList->SetValue(_T("confidence"), varConfidence);
//            }
//
//            PLAMessage->SetXml(_T("/getMatchingItemsResponse"), PLAList->xml);
//            PLAMessage->SetValue(_T("status"), _T("success"));
//        }
//        else if (pRes->enStatus == NOALERT)
//        {
//            PLAMessage->SetValue(_T("status"), _T("success"));
//        }
//        else if (pRes->enStatus == CENTER)
//        {
//            PLAMessage->SetValue(_T("status"), _T("center"));
//        }
//        else
//        {
//            PLAMessage->SetValue(_T("status"), _T("failed"));
//        }
//        */
//
//        _bstr_t xmlStr = PLAMessage->Getxml();
//        Trace(TRACE_INFO, (LPCTSTR)xmlStr);
//
//        // sendStream(PLAMessage);
//
//        IGTStreamPtr stream = PLAMessage;
//        try
//        {
//            CString csMsg;
//            csMsg.Format(_T("Sending image data, camera %d"), nCameraNum);
//            PostStatusMsgToParent(csMsg);
//            m_gPosConnection->Send(stream);
//        }
//        catch (_com_error& er)
//        {
//            // g_cmDc.DoDCPrintf(TRACE_ALWAYS, _T("Unable to send the stream over pipe server,%s, Please restart IMA"), er.ErrorMessage() );
//            Trace(TRACE_ERROR, _T("Unable to send the stream over pipe server,%s, Please restart IMA"), er.ErrorMessage());
//            PostStatusMsgToParent(_T("Failed to send image data to pipeserver"));
//        }
//        catch (...)
//        {
//            // g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Unable to send the stream over pipe server, Unhandled Exception. Please restart IMA"));
//            Trace(TRACE_ERROR, _T("Unable to send the stream over pipe server, Unhandled Exception. Please restart IMA"));
//            PostStatusMsgToParent(_T("Failed to send image data to pipeserver"));
//        }
//
//        PLAList.Release();
//        PLAMessage.Release();
//    }
//    catch (_com_error& er)
//    {
//        Trace(TRACE_ERROR, _T("Unable to send Picklist Assist Message over pipe server,%s"), er.ErrorMessage());
//    }
//    return hr;
//}


HRESULT WINAPI CGTOControlObject::Callback(VARIANT vUser, GTConnectReceiveEvent tbcre, IDispatch *pObj)
{
	try
	{
		m_gpGenObj = pObj;
		_bstr_t str = m_gpGenObj->Name;
		g_GTOCtrlObj.setGenObj(m_gpGenObj);
		_bstr_t xmlStr = m_gpGenObj->GetXml(L"/");

        Trace(TRACE_INFO, (LPCTSTR)xmlStr);

        std::string ssCurrentFilename = currentFilename();
        WriteToFile(ssCurrentFilename.c_str(), xmlStr);

        //if (_tcsicmp((_TCHAR*)str, _T("postImage")) == 0)
        //{
        //    g_GTOCtrlObj.handlePostImageMessage();
        //}

        PostStatusMsgToParent((LPCTSTR)xmlStr);

		g_GTOCtrlObj.ReleaseGenObj();
	}
	catch(_com_error& e)
	{
        Trace(TRACE_ERROR, _T("Com Error Handling Pipe Server Message %s"), e.ErrorMessage());
        //g_GTOCtrlObj.Stop();
	}
	catch(...)
	{
        Trace(TRACE_ERROR, _T("Unknow Exception Handling Pipe Server Message"));
        //g_GTOCtrlObj.Stop();
	}
    return S_OK;
}
/// <summary>
/// Sets the GTO object.
/// </summary>
/// <param name="pGenObj">GTO object.</param>
void CGTOControlObject::setGenObj( IGTGenObjPtr pGenObj)
{
	try
	{
		EnterCriticalSection(&m_CSvariable);
		if (m_pGenObj != NULL)
		{
			m_pGenObj.Release();
			m_pGenObj = NULL;
		}
		m_pGenObj = pGenObj;
	}
	catch(_com_error& er)
	{
        Trace(TRACE_INFO, _T("Unable to assign the GTO Object,%s"), er.ErrorMessage());
    }

}
void CGTOControlObject::handlePostImageMessage()
{
    try
    {
        if (m_pGenObj != NULL)
        {
            bool bInvalidParam = false;

            CString csName = CString((LPCTSTR)m_pGenObj->GetAttribute(_T("/"), _T("_name")));

            Trace(TRACE_INFO, _T("PostImage recieved for %s"), csName.GetString());

            // _bstr_t bstrt = GetObjectValue(_T("base64Image"));

            std::string ssB64Image = (LPCSTR)GetObjectValue(_T("base64Image"));

            if (ssB64Image.size() > 0)
            {
                CBase64EncodeDecode oB64Util;

                std::vector<uint8_t> vecBinaryImage;

                oB64Util.Base64Decode(&ssB64Image[0], ssB64Image.size(), vecBinaryImage);

                std::string ssCurrentFilename = currentFilename();

                ssCurrentFilename += ".jpg";

                SaveImage(&vecBinaryImage, CString(ssCurrentFilename.c_str()));

                PostImageMsgToParent(vecBinaryImage);
            }
            else
            {
                ASSERT(0);
                PostStatusMsgToParent(_T("ERROR! Event PostImage\base64Image node is empty"));
                Trace(TRACE_ERROR, _T("ERROR! Event PostImage\base64Image node is empty"));
            }


            // CString csTimeout = CString((LPCTSTR)GetObjectValue(_T("base64Image")));

        }
    }
    catch (_com_error& e)
    {
        //g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Com Error Handling Pipe Server Message %s"), e.ErrorMessage());
        Trace(TRACE_ERROR, _T("Com Error Handling Pipe Server Message %s"), e.ErrorMessage());
    }
    catch (...)
    {
        //g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Unknown Exception Handling Pipe Server Message"));
        Trace(TRACE_ERROR, _T("Unknown Exception Handling Pipe Server Message"));
    }


}

//void CGTOControlObject::handleConfiguartionMessage()
//{
//	try
//	{
//		if(m_pGenObj != NULL)
//		{
//			bool bInvalidParam = false;
//
//            CString csName = CString((LPCTSTR)m_pGenObj->GetAttribute(_T("/"),_T("_name")));
//
//            Trace(TRACE_INFO, _T("Configuration recieved for %s"), csName.GetString());
//
//            m_vCameraIdList.clear();
//
//            //g_cmDc.DoDCPrintf(TRACE_ALWAYS, _T("Configuration recieved for %s"), csName.GetString());
//
//            if(csName.CompareNoCase(_T("PA")) == 0)
//			{
//				SSBPACONFIG stPAConfig = {false, //bEnable
//										  false, //bCenterDetection
//										  false, //bScaleMetric
//										  PA_MODE_0, //nMode
//										  0 //ltimeout
//										 };		
//
//				CString csTimeout = CString((LPCTSTR)GetObjectValue(_T("timeout")));
//				CString csCenterDetection = CString((LPCTSTR)GetObjectValue(_T("centerDetection")));
//				CString csEnable  = CString((LPCTSTR)GetObjectValue(_T("enable")));
//				CString csScaleMetric = CString((LPCTSTR)GetObjectValue(_T("scalemetric")));
//				CString csMode = CString((LPCTSTR)GetObjectValue(_T("mode")));
//				if(csTimeout.IsEmpty() || csCenterDetection.IsEmpty() || csEnable.IsEmpty() || csScaleMetric.IsEmpty() || csMode.IsEmpty())
//				{
//					sendConfigResponse(-1, stPAConfig);
//					return;
//				}
//				if(csTimeout.SpanIncluding(_T("0123456789")) == csTimeout)
//				{
//					//stPAConfig.ltimeout = std::atol(csTimeout.GetString());
//                    stPAConfig.ltimeout = _ttol(csTimeout.GetString());
//				}
//				else
//				{
//					bInvalidParam = true;
//				}
//				if(csCenterDetection.CompareNoCase(_T("on")) == 0)
//				{
//					stPAConfig.bCenterDetection = true;
//				}
//				else if(csCenterDetection.CompareNoCase(_T("off")) == 0)
//				{
//					stPAConfig.bCenterDetection = false;
//				}
//				else
//				{
//					bInvalidParam = true;
//				}
//
//				if(csEnable.CompareNoCase(_T("on")) == 0)
//				{
//					stPAConfig.bEnable = true;
//				}
//				else if(csEnable.CompareNoCase(_T("off")) == 0)
//				{
//					stPAConfig.bEnable = false;
//				}
//				else
//				{
//					bInvalidParam = true;
//				}
//
//				if(csScaleMetric.CompareNoCase(_T("on")) == 0)
//				{
//					stPAConfig.bScaleMetric = true;
//				}
//				else if(csScaleMetric.CompareNoCase(_T("off")) == 0)
//				{
//					stPAConfig.bScaleMetric = false;
//				}
//				else
//				{
//					bInvalidParam = true;
//				}
//				
//				if(csMode.CompareNoCase(_T("aggressive")) == 0)
//				{
//					stPAConfig.nMode = PA_MODE_3;
//				}
//				else if(csMode.CompareNoCase(_T("conservative")) == 0)
//				{
//					stPAConfig.nMode = PA_MODE_0;
//				}
//				else
//				{
//					if(csMode.SpanIncluding(_T("0123456789")) == csMode)
//					{
//						// stPAConfig.nMode = std::atol(csMode.GetString());
//                        stPAConfig.nMode = _ttol(csMode.GetString());
//                    }
//					else
//					{
//						bInvalidParam = true;
//					}
//					if(stPAConfig.nMode < 0 || stPAConfig.nMode > 4)
//						bInvalidParam = true;
//				}
//
//				if(bInvalidParam)
//				{
//					sendConfigResponse(-1, stPAConfig);
//					return;
//				}
//				//SSBRESSTATUS enRes = SetSSBPAConfig(stPAConfig);
//				//if(enRes == SUCCESS)
//				//{
//					sendConfigResponse(0, stPAConfig);
//				//}
//				//else
//				//{
//				//	sendConfigResponse(1, stPAConfig);
//				//}
//			}
//			else if(csName.CompareNoCase(_T("PLA")) == 0)
//			{
//				SSBPLACONFIG stPLAConfig = {0,		// no of items
//											false, //bEnable
//										    false, //bCenterDetection
//											false, // bValidation
//										    false, //bScaleMetric
//										    0 //ltimeout
//										 };
//				CString csTimeout = CString((LPCTSTR)GetObjectValue(_T("timeout")));
//				CString csCenterDetection = CString((LPCTSTR)GetObjectValue(_T("centerDetection")));
//				CString csEnable  = CString((LPCTSTR)GetObjectValue(_T("enable")));
//				CString csValidation = CString((LPCTSTR)GetObjectValue(_T("validation")));
//				CString csNumofItems = CString((LPCTSTR)GetObjectValue(_T("numberofItems")));
//				CString csScaleMetric = CString((LPCTSTR)GetObjectValue(_T("scalemetric")));
//				if(csTimeout.IsEmpty() || csCenterDetection.IsEmpty() || csEnable.IsEmpty() || csScaleMetric.IsEmpty() ||
//					csValidation.IsEmpty() || csNumofItems.IsEmpty())
//				{
//					sendConfigResponse(-1,stPLAConfig);
//					return;
//				}
//				
//				if(csTimeout.SpanIncluding(_T("0123456789")) == csTimeout)
//				{
//                    // stPLAConfig.ltimeout = std::atol(csTimeout.GetString());
//                    stPLAConfig.ltimeout = _ttol(csTimeout.GetString());
//				}
//				else
//				{
//					bInvalidParam = true;
//				}
//				
//				if(csCenterDetection.CompareNoCase(_T("on")) == 0)
//				{
//					stPLAConfig.bCenterDetection = true;
//				}
//				else if(csCenterDetection.CompareNoCase(_T("off")) == 0)
//				{
//					stPLAConfig.bCenterDetection = false;
//				}
//				else
//				{
//					bInvalidParam = true;
//				}
//				if(csEnable.CompareNoCase(_T("on")) == 0)
//				{
//					stPLAConfig.bEnable = true;
//				}
//				else if(csEnable.CompareNoCase(_T("off")) == 0)
//				{
//					stPLAConfig.bEnable = false;
//				}
//				else
//				{
//					bInvalidParam = true;
//				}
//
//				if(csScaleMetric.CompareNoCase(_T("on")) == 0)
//				{
//					stPLAConfig.bScaleMetric = true;
//				}
//				else if(csScaleMetric.CompareNoCase(_T("off")) == 0)
//				{
//					stPLAConfig.bScaleMetric = false;
//				}
//				else
//				{
//					bInvalidParam = true;
//				}
//
//				if(csNumofItems.SpanIncluding(_T("0123456789")) == csNumofItems)
//				{
//                    stPLAConfig.noofitems = _ttoi(csNumofItems.GetString());
//					// stPLAConfig.noofitems = std::atoi(csNumofItems.GetString());
//				}
//				else
//				{
//					bInvalidParam = true;
//				}
//				
//				if(csValidation.CompareNoCase(_T("on")) == 0)
//				{
//					stPLAConfig.bValEnable = true;
//				}
//				else if(csValidation.CompareNoCase(_T("off")) == 0)
//				{
//					stPLAConfig.bValEnable = false;
//				}
//				else
//				{
//					bInvalidParam = true;
//				}
//
//				if(bInvalidParam)
//				{
//					sendConfigResponse(-1, stPLAConfig);
//					return;
//				}
//
//    //            SSBRESSTATUS enRes = SetSSBPLAConfig(stPLAConfig);
//				//if(enRes == SUCCESS)
//				//{
//					sendConfigResponse(0, stPLAConfig);
//				//}
//				//else
//				//{
//				//	sendConfigResponse(1, stPLAConfig);
//				//}
//			}
//			else
//			{
//				sendConfigResponse();
//				//g_cmDc.DoDCPrintf(TRACE_ALWAYS, _T("Invalid Configuration recieved"));
//                TRACE(_T("Invalid Configuration recieved"));
//            }
//		}
//	}
//	catch(_com_error& e)
//	{
//		//g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Com Error Handling Pipe Server Message %s"), e.ErrorMessage());
//        Trace( TRACE_ERROR, _T("Com Error Handling Pipe Server Message %s"), e.ErrorMessage());
//    }
//	catch(...)
//	{
//		//g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Unknown Exception Handling Pipe Server Message"));
//        Trace(TRACE_ERROR, _T("Unknown Exception Handling Pipe Server Message"));
//    }
//}

//void CGTOControlObject::handlestateDataMessage()
//{
//	try
//	{
//		if(m_pGenObj != NULL)
//		{
//			if(m_pGenObj->MsgId == 0xC9)
//			{
//				CString csStateName = CString((LPCTSTR)GetObjectValue(_T("stateName")));
//				
//				if(csStateName.CompareNoCase(_T("Complete")) == 0)
//				{
//					//g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Pipe Server State Message Received"));
//                    Trace(TRACE_INFO, _T("Pipe Server State Message Received"));
//                }
//			}
//			else 
//			{
//				// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Invalid Message ID"));
//                Trace(TRACE_ERROR, _T("Invalid Message ID"));
//            }
//		}
//	}
//	catch(_com_error& e)
//	{
//		//g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Com Error Handling Pipe Server Message %s"), e.ErrorMessage());
//        Trace(TRACE_ERROR, _T("Com Error Handling Pipe Server Message %s"), e.ErrorMessage());
//    }
//	catch(...)
//	{
//		//g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Unknow Exception Handling Pipe Server Message"));
//        Trace(TRACE_ERROR, _T("Unknow Exception Handling Pipe Server Message"));
//    }
//}

//BOOL CGTOControlObject::GetDeviceImage(int a_nCameraNum, BOOL a_bIsRefImage)
//{
//    //UpdateUiStatus(eBusy);
//
//    //m_ctlStaticPicture.Invalidate();
//    //m_ctlStaticPicture.UpdateWindow();
//
//    Trace(TRACE_INFO, _T("Initiating GetDeviceImage(camera(%d), ref(%s))"), a_nCameraNum, a_bIsRefImage?_T("true"):_T("false"));
//
//    IMAGEDATA imaImageData;
//    CDeviceConnector* pDevice = CDeviceConnector::GetInstance();
//    if (pDevice)
//    {
//        // Init Scanwatch Processing : Prepares the Item and reference
//        // images on the Scanner
//        BOOL bOPOSResult = pDevice->InitScanWatchProcessing();
//        if (!bOPOSResult)
//        {
//            //UpdateUiStatus(eIdle);
//            // MessageBox(_T("InitScanWatchProcessing failed"), _T("Error!"), MB_OK | MB_ICONERROR);
//            Trace(TRACE_ERROR, _T("InitScanWatchProcessing failed"));
//            // SSBLOG(_T("Initialize Scanwatch Failed: %ld\n"), pDevice->GetErrorInfo().nErrorCode);
//            // m_stErrorInfo = pDevice->GetErrorInfo();
//            return FALSE;
//        }
//
//        // TODO: fix the need for conversion here
//        //
//        bOPOSResult = (pDevice->SetImageType(a_bIsRefImage ? true : false) ? TRUE : FALSE);
//        if (!bOPOSResult)
//        {
//            Trace(TRACE_ERROR, _T("Unable to Set Image Type to Device\n"));
//            //SSBLOG(_T("Unable to Set Image Type to Device\n"));
//            //m_stErrorInfo = pDevice->GetErrorInfo();
//            bOPOSResult = pDevice->ResetScanWatchProcessing();
//            return FALSE;
//        }
//
//        //Sets the Camera Num to Device, Values should be 0-3.
//        bOPOSResult = pDevice->SetCameraNum(a_nCameraNum);
//        if (!bOPOSResult)
//        {
//            Trace(TRACE_ERROR, _T("Unable to Set Camera Number to Device\n"));
//            //SSBLOG(_T("Unable to Set Camera Number to Device\n"));
//            //m_stErrorInfo = pDevice->GetErrorInfo();
//            bOPOSResult = pDevice->ResetScanWatchProcessing();
//            return FALSE;
//        }
//
//        // StartSession captures the image and Dataevent is raised
//        bOPOSResult = pDevice->StartSession();
//        if (!bOPOSResult)
//        {
//            Trace(TRACE_ERROR, _T("Unable to Start the Scanwatch Session\n"));
//            // SSBLOG(_T("Unable to Start the Scanwatch Session\n"));
//            //m_stErrorInfo = pDevice->GetErrorInfo();
//            bOPOSResult = pDevice->ResetScanWatchProcessing();
//            return FALSE;
//        }
//
//        // pDevice->WaitForDataEvent();
//
//        // Check if the dataevent is raised, If raised save the image data to the structure
//        //BOOL bstatus = pDevice->CheckforDataEvent(imaImageData);
//        //if (!bstatus)
//        //{
//        //    Trace(TRACE_ERROR,  _T("Failed to get Scanwatch Image Data\n"));
//        //    //SSBLOG(_T("Failed to get Scanwatch Image Data\n"));
//        //    //m_stErrorInfo = pDevice->GetErrorInfo();
//        //    bOPOSResult = pDevice->ResetScanWatchProcessing();
//        //    return FALSE;
//        //}
//        //Trace(TRACE_ERROR, _T(" ItemType %d, CamNum %d, Image Name : %s  \n"), a_bIsRefImage, a_nCameraNum, CString(imaImageData.cImgePath.c_str()).GetString());
//        //SSBLOG(_T(" ItemType %d, CamNum %d, Image Name : %s  \n"), i, j, imaImageData.cImgePath.c_str());
//        // csImageNames.push_back(imaImageData.cImgePath);
//        // LogImageInfo(imaImageData);
//
//        // 5. Reset the Scanwatch Processing
//        bOPOSResult = pDevice->ResetScanWatchProcessing();
//        if (!bOPOSResult)
//        {
//            Trace(TRACE_ERROR, _T("Unable to reset the Scanwatch Processing\n"));
//            // SSBLOG(_T("Unable to reset the Scanwatch Processing\n"));
//        }
//    }
//    //LOGTIME(_T("Get Image from Devices"));
//    return TRUE;
//}

//void CGTOControlObject::LogImageInfo(IMAGEDATA imaData)
//{
//    //Trace(TRACE_ERROR, _T(" ItemType %d, CamNum %d, Image Name : %s  \n"), a_bIsRefImage, a_nCameraNum, CString(imaImageData.cImgePath.c_str()).GetString());
//
//    Trace(TRACE_INFO, _T(" Camera %d info:"), m_vCameraIdList.back());
//    Trace(TRACE_INFO, _T("   IMAGEDATA.cImgePath     = %s"), CString(imaData.cImgePath.c_str()) );
//    Trace(TRACE_INFO, _T("   IMAGEDATA.nBitsPerPixel = %d"), imaData.nBitsPerPixel);
//    Trace(TRACE_INFO, _T("   IMAGEDATA.nFrameType    = %d"), imaData.nFrameType);
//    Trace(TRACE_INFO, _T("   IMAGEDATA.nImageHeight  = %d"), imaData.nImageHeight);
//    Trace(TRACE_INFO, _T("   IMAGEDATA.nImageLength  = %d"), imaData.nImageLength);
//    Trace(TRACE_INFO, _T("   IMAGEDATA.nImageType    = %d"), imaData.nImageType);
//    Trace(TRACE_INFO, _T("   IMAGEDATA.nImageWidth   = %d"), imaData.nImageWidth);
//
//    CGTOControlObject::SendImage(imaData);
//
//    if (!m_vCameraIdList.empty())
//    {
//        GetDeviceImage(m_vCameraIdList.back() - 1, FALSE);
//    }
//    else
//    {
//        mapMetaData.clear();
//    }
//}


//void CGTOControlObject::handlegetMatchingItems()
//{
//	try
//	{
//		if(m_pGenObj != NULL)
//		{
//			CString csWeight = CString((LPCTSTR)GetObjectValue(_T("weight")));
//			CString  csRequestID = CString((LPCTSTR)GetObjectValue(_T("hdr/sequenceID")));
//			if(csWeight.IsEmpty())
//			{
//				//g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("PS: Invalid PLA Request ") );
//                TRACE(_T("PS: Invalid PLA Request "));
//            }
//			int nWeight = _ttoi(csWeight.GetString());
//			int nRequestID = _ttoi(csRequestID.GetString());
//
//			//g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("PS: Processing PLA Request Weight: %d, Request ID: %d "),nWeight, nRequestID);
//            Trace(TRACE_INFO, _T("PS: Processing PLA Request Weight: %d, Request ID: %d "), nWeight, nRequestID);
//
//			SSBPLAREQUEST PLreq;
//			PLreq.nRequestID = nRequestID;
//			PLreq.nWeight = nWeight;
//			// PSSBPLARESPONSE pRes = ProcessPicklistFlow(PLreq);
//
//            SSBPLARESPONSE oPlaResponse;
//
//            oPlaResponse.enStatus = SUCCESS;
//            oPlaResponse.number = 0;
//
//            CGTOControlObject::SendPLA(&oPlaResponse);
//
//			//if(pRes != NULL) 
//			//{
//			//	if(pRes->enStatus != FAILED || pRes->enStatus != DISABLED)
//			//	{
//			//		if(pRes->number == 0)
//			//		{
//			//			//g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("No Items retrieved from PLA Library"));
//            //          TRACE(_T("No Items retrieved from PLA Library"));
//            //      }
//			//		CGTOControlObject::SendPLA(pRes);
//			//	}
//			//	delete pRes;
//			//}
//			//else
//			//{
//			//	g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Unable to retrive the list, Get matching items failed"));
//			//}
//		}		
//	}
//	catch(_com_error& e)
//	{
//		Trace(TRACE_ERROR,_T("Com Error Handling Pipe Server Message %s"), e.ErrorMessage());
//	}
//	catch(...)
//	{
//		Trace(TRACE_ERROR,_T("UnHandled exception in Pipe Server Message"));
//	}
//
//    //
//    // Take pictures and post images to pipeserver
//    //
//    //
//    m_vCameraIdList = GetConfig()->m_vCameras;
//
//    std::reverse(m_vCameraIdList.begin(), m_vCameraIdList.end());
//
//    GetDeviceImage(m_vCameraIdList.back() - 1, FALSE);
//}

//void CGTOControlObject::handleitemDetail()
//{   
//	try
//	{
//		if(m_pGenObj != NULL)
//		{
//			CString csitemUPC = CString((LPCTSTR)GetObjectValue(_T("itemUPC")));
//
//            mapMetaData[_T("itemUPC")] = csitemUPC;
//
//			CString csitemDesc = CString((LPCTSTR)GetObjectValue(_T("itemDesc")));
//
//            mapMetaData[_T("itemDesc")] = csitemDesc;
//
//			CString csWeight  = CString((LPCTSTR)GetObjectValue(_T("weight")));
//
//            mapMetaData[_T("weight")] = csWeight;
//
//			//LB185115 - SSPSW-1142: SSB 2.0 need to support the Quantity Items Phase II
//			CString csQuantity  = CString((LPCTSTR)GetObjectValue(_T("quantity")));
//
//            mapMetaData[_T("quantity")] = csQuantity;
//
//            CString  csRequestID = CString((LPCTSTR)GetObjectValue(_T("hdr/sequenceID")));
//
//			//LB185115 - SSPSW-1143: Support Keyed-In Items for PA/PLA SSB 2.0
//			if(csitemDesc.IsEmpty())
//			{
//				csitemDesc = _T(" ");
//			}
//			if(csQuantity.IsEmpty())
//			{
//				csQuantity = _T("no");
//			}
//			if(csitemUPC.IsEmpty() || csWeight.IsEmpty() || csRequestID.IsEmpty())
//			{
//				Trace(TRACE_INFO,_T("Invalid Validation Request"));
//				//return;
//			}
//			int nWeight; 
//			if(csWeight.SpanIncluding(_T("0123456789")) == csWeight)
//			{
//				nWeight = _ttoi(csWeight.GetString());
//			}
//			else
//			{
//				nWeight = 0;
//			}
//
//			int nRequestID = _ttoi(csRequestID.GetString());
//			SSBVALREQUEST cRequest;
//			cRequest.nRequestID = nRequestID;
//			cRequest.csItemUPC = (char*)csitemUPC.GetString();
//			cRequest.cDescription = (char*)csitemDesc.GetString();
//			cRequest.nWeight = nWeight;
//			Trace(TRACE_INFO,_T("Validation for ItemUPC: %s, ItemDesc: %s, Item Weight: %d, Quantity: %s"), cRequest.csItemUPC, cRequest.cDescription, cRequest.nWeight, csQuantity);
//			bool bInvalid = false;
//
//            SSBVALRESPONSE oResponse = { 0 };
//
//            oResponse.enStatus = SUCCESS;
//
//            SendAlertMessage(&oResponse);
//
//			//PSSBVALRESPONSE pResponse = ProcessValidationFlow(cRequest);
//			//if(pResponse != NULL)
//			//{
//			//	if(pResponse->enStatus != DISABLED)
//			//	{
//			//		SendAlertMessage(pResponse);
//			//		SendStatus(SSB_COMM_STATUS_PIPESER_RES_SENT);
//			//	}
//			//	delete pResponse;
//			//}
//			//else
//			//{
//			//	g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Failed to Process Validation flow."));
//			//}
//			//g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Completed Handling Item Details."));
//		}
//	}
//	catch(_com_error& e)
//	{
//		Trace(TRACE_ERROR,_T("Com Error Handling Pipe Server Message %s"), e.ErrorMessage());
//	}
//	catch(...)
//	{
//		Trace(TRACE_ERROR,_T("Unknown Exception in Pipe Server Message Handling"));
//	}
//
//    //
//    // Take pictures and post images to pipeserver
//    //
//    //
//    m_vCameraIdList = GetConfig()->m_vCameras;
//
//    std::reverse(m_vCameraIdList.begin(), m_vCameraIdList.end());
//
//    GetDeviceImage(m_vCameraIdList.back() - 1, FALSE);
//
//}

//void CGTOControlObject::handledbManagement()
//{   /*
//	_bstr_t xmlStr = m_pGenObj->Getxml();
//	g_cmDc.DoDCPrintf(TRACE_ALWAYS,(LPCSTR) xmlStr);
//	CString csAction = CString((LPCTSTR)GetObjectValue(_T("action"))); 
//	CString csUPC    = CString((LPCTSTR)GetObjectValue(_T("item")));
//	SSBDBITEMS stSSBDBItems;
//	bool bRet = false;
//	if(csAction.Compare("get") == 0)
//		stSSBDBItems.enAction = GET;
//	else if(csAction.Compare("delete") == 0)
//		stSSBDBItems.enAction = DEL;
//	if((csUPC.Compare("*") == 0) && (csAction.Compare("get")==0))
//	{
//		stSSBDBItems.nItemCount = 100;
//		bRet = GetAllItems(stSSBDBItems);
//	}
//	else if((csUPC.Compare("*") != 0) && (csAction.Compare("get")==0))
//	{
//		stSSBDBItems.nItemCount = 1;
//		stSSBDBItems.pItems = new SSBDBITEM;
//		strcpy(stSSBDBItems.pItems->cItemUPC,csUPC.GetString());
//		bRet = GetItem(stSSBDBItems);
//	}
//	else if(csAction.Compare("delete")==0)
//	{
//		bRet = DeleteItem(csUPC.GetString());
//	}
//	
//	sendDBResponse(bRet, stSSBDBItems);
//    */
//}

//HRESULT CGTOControlObject::sendConfigResponse(int nCode, SSBPLACONFIG& stSSBPLAConfig)
//{
//	HRESULT hr = S_OK;
//	IGTGenObjPtr ConfigResp;
//	try
//	{
//		 //setup xml for GTO object
//		hr = ConfigResp.CreateInstance(__uuidof( CGTGenObj));
//		if(FAILED(hr))
//			throw _com_error(hr);
//  		ConfigResp->Name = _T("configResponse");
//		ConfigResp->IsXMLOutput = true;
//		ConfigResp->Validate = false;
//		ConfigResp->MsgId = 0xCF;
//		ConfigResp->IsNamedElementsXML = true;
//		if(nCode == 0)
//			ConfigResp->SetValue(_T("status"),_T("success"));
//		else if(nCode == -1)
//		{
//			ConfigResp->SetValue(_T("status"),_T("failure"));
//			ConfigResp->SetValue(_T("error"),_T("Parameter value is empty/not as expected"));
//		}
//		else
//		{
//			ConfigResp->SetValue(_T("status"),_T("failure"));
//			//PSSBERRORINFO pError = GetSSBErrorInfo();
//			//if(pError != NULL)
//			//{
//			//	CString cString = _T("");
//			//	cString.Format(_T("Error code: %d, %s, Subsystem Error Code: %d, %s, Additional Information : %s"), pError->stError.nErrorCode, pError->csErrorTxt, pError->stError.nSubSystemError, pError->csSubSytemErrorTxt, pError->stError.csAdditionInfo);
//			//	ConfigResp->SetValue(_T("error"), cString.GetString());
//			//	delete pError;
//			//}
//			//else
//			//{
//				ConfigResp->SetValue(_T("error"), _T("Unknown SSB Error"));
//			//}
//		}
//		
//		_bstr_t _name = m_pGenObj->GetAttribute(_T("/"),_T("_name"));
//		ConfigResp->SetAttribute(_T("/"),_T("_name"),_name);
//		
//		_bstr_t hdr = m_pGenObj->GetXml(_T("/hdr"));
//		ConfigResp->SetXml(_T("/hdr"),hdr);
//		ConfigResp->SetValue(_T("hdr/transID"), currentDateTime().c_str());
//		if(nCode != -1)
//		{
//			ConfigResp->SetValue(_T("timeout"), stSSBPLAConfig.ltimeout);
//			ConfigResp->SetValue(_T("centerDetection"), stSSBPLAConfig.bCenterDetection?_T("on"):_T("off"));
//			ConfigResp->SetValue(_T("enable"), stSSBPLAConfig.bEnable?_T("on"):_T("off"));
//			ConfigResp->SetValue(_T("scalemetric"), stSSBPLAConfig.bScaleMetric?_T("on"):_T("off"));
//			ConfigResp->SetValue(_T("validation"), stSSBPLAConfig.bValEnable?_T("on"):_T("off"));
//			ConfigResp->SetValue(_T("numberofItems"), stSSBPLAConfig.noofitems);
//		}
//		
//		_bstr_t xmlStr = ConfigResp->Getxml();
//		Trace(TRACE_INFO,(LPCTSTR) xmlStr);
//        PostStatusMsgToParent(_T("Sending PLA ConfigResponse."));
//        sendStream(ConfigResp);
//		ConfigResp.Release();
//	}
//	catch(_com_error& er)
//	{
//		Trace(TRACE_ERROR, _T("Unable to send Picklist Assist Message over pipe server,%s"), er.ErrorMessage() );
//	}
//    return hr;
//}

//HRESULT CGTOControlObject::sendConfigResponse(int nCode, SSBPACONFIG& stSSBPAConfig)
//{
//	HRESULT hr = S_OK;
//	IGTGenObjPtr ConfigResp;
//	try
//	{
//		 //setup xml for GTO object
//		hr = ConfigResp.CreateInstance(__uuidof( CGTGenObj));
//		if(FAILED(hr))
//			throw _com_error(hr);
//  		ConfigResp->Name = _T("configResponse");
//		ConfigResp->IsXMLOutput = true;
//		ConfigResp->Validate = false;
//		ConfigResp->MsgId = 0xCF;
//		ConfigResp->IsNamedElementsXML = true;
//		if(nCode == 0)
//			ConfigResp->SetValue(_T("status"),_T("success"));
//		else if(nCode == -1)
//		{
//			ConfigResp->SetValue(_T("status"),_T("failure"));
//			ConfigResp->SetValue(_T("error"),_T("Parameter value is empty/not as expected"));
//		}
//		else
//		{
//			ConfigResp->SetValue(_T("status"),_T("failure"));
//			//PSSBERRORINFO pError = GetSSBErrorInfo();
//			//if(pError != NULL)
//			//{
//			//	CString cString = _T("");
//			//	cString.Format(_T("Error code: %d, %s, Subsystem Error Code: %d, %s, Additional Information : %s"), pError->stError.nErrorCode, pError->csErrorTxt, pError->stError.nSubSystemError, pError->csSubSytemErrorTxt, pError->stError.csAdditionInfo);
//			//	ConfigResp->SetValue(_T("error"), cString.GetString());
//			//	delete pError;
//			//}
//			//else
//			//{
//				ConfigResp->SetValue(_T("error"), _T("Unknown SSB Error"));
//			//}
//		}
//		
//		_bstr_t _name = m_pGenObj->GetAttribute(_T("/"),_T("_name"));
//		ConfigResp->SetAttribute(_T("/"),_T("_name"),_name);
//		
//		_bstr_t hdr = m_pGenObj->GetXml(_T("/hdr"));
//		ConfigResp->SetXml(_T("/hdr"),hdr);
//		ConfigResp->SetValue(_T("hdr/transID"), currentDateTime().c_str());
//
//		if(nCode != -1)
//		{
//			ConfigResp->SetValue(_T("timeout"), stSSBPAConfig.ltimeout);
//			ConfigResp->SetValue(_T("centerDetection"), stSSBPAConfig.bCenterDetection?_T("on"):_T("off"));
//			ConfigResp->SetValue(_T("enable"), stSSBPAConfig.bEnable?_T("on"):_T("off"));
//			ConfigResp->SetValue(_T("scalemetric"), stSSBPAConfig.bScaleMetric?_T("on"):_T("off"));
//			ConfigResp->SetValue(_T("mode"), stSSBPAConfig.nMode);
//		}
//
//		_bstr_t xmlStr = ConfigResp->Getxml();
//		Trace(TRACE_INFO,(LPCTSTR) xmlStr);
//        PostStatusMsgToParent(_T("Sending PA ConfigResponse."));
//        sendStream(ConfigResp);
//		ConfigResp.Release();
//	}
//	catch(_com_error& er)
//	{
//		Trace(TRACE_ERROR, _T("Unable to send Picklist Assist Message over pipe server,%s"), er.ErrorMessage() );
//        PostStatusMsgToParent(_T("Failed to send PA ConfigResponse."));
//    }
//    return hr;
//}

//HRESULT CGTOControlObject::sendConfigResponse()
//{
//	HRESULT hr = S_OK;
//	IGTGenObjPtr ConfigResp;
//	try
//	{
//		 //setup xml for GTO object
//		hr = ConfigResp.CreateInstance(__uuidof( CGTGenObj));
//		if(FAILED(hr))
//			throw _com_error(hr);
//  		ConfigResp->Name = _T("configResponse");
//		ConfigResp->IsXMLOutput = true;
//		ConfigResp->Validate = false;
//		ConfigResp->MsgId = 0xCF;
//		ConfigResp->IsNamedElementsXML = true;
//		ConfigResp->SetValue(_T("status"),_T("failure"));
//		ConfigResp->SetValue(_T("error"),_T("Invalid Configuration name"));
//
//		_bstr_t _name = m_pGenObj->GetAttribute(_T("/"),_T("_name"));
//		ConfigResp->SetAttribute(_T("/"),_T("_name"),_name);
//		
//		_bstr_t hdr = m_pGenObj->GetXml(_T("/hdr"));
//		ConfigResp->SetXml(_T("/hdr"),hdr);
//		ConfigResp->SetValue(_T("hdr/transID"), currentDateTime().c_str());
//
//		_bstr_t xmlStr = ConfigResp->Getxml();
//        Trace(TRACE_INFO,(LPCTSTR) xmlStr);
//		sendStream(ConfigResp);
//		ConfigResp.Release();
//	}
//	catch(_com_error& er)
//	{
//		Trace(TRACE_ERROR, _T("Unable to send Picklist Assist Message over pipe server,%s"), er.ErrorMessage() );
//	}
//	return hr;
//}

/*
HRESULT CGTOControlObject::sendDBResponse(bool bStatus,SSBDBITEMS& stSSBDBItems)
{
	HRESULT hr = S_OK;
	IGTGenObjPtr DBMngtResp;
	IGTGenObjPtr ItemsList;
	try
	{
		 //setup xml for GTO object
		hr = DBMngtResp.CreateInstance(__uuidof( CGTGenObj));
		if(FAILED(hr))
			throw _com_error(hr);
		hr = ItemsList.CreateInstance(__uuidof( CGTGenObj));
		 if(FAILED(hr))
			 throw _com_error(hr);
  		DBMngtResp->Name = _T("dbManagementResponse");
		DBMngtResp->IsXMLOutput = true;
		DBMngtResp->Validate = false;
		DBMngtResp->MsgId = 0xCF;
		DBMngtResp->IsNamedElementsXML = true;
		ItemsList->IsXMLOutput = true;
		ItemsList->Validate = false;
		
		_bstr_t hdr = m_pGenObj->GetXml(_T("/hdr"));
		DBMngtResp->SetXml(_T("/hdr"),hdr);
		DBMngtResp->SetValue(_T("hdr/transID"), currentDateTime().c_str());
		if(stSSBDBItems.pItems != NULL)
		{
			if((stSSBDBItems.enAction == GET) && (stSSBDBItems.nItemCount > 0)&& (bStatus == true))
			{
				ItemsList->Name = _T("items");
				ItemsList->IsNamedElementsXML = true;
				for(int i =0; i< stSSBDBItems.nItemCount; i++)
				{
					_variant_t varDesc(stSSBDBItems.pItems[i].cDescription);
					_variant_t varCode(stSSBDBItems.pItems[i].cItemUPC);
					CString csUPC = stSSBDBItems.pItems[i].cItemUPC;
					ItemsList->SetCurrentNode(_T("/item"),i);
					ItemsList->SetValue(_T("code"),varCode);
					ItemsList->SetValue(_T("description"),varDesc);
				}
				DBMngtResp->SetXml(_T("/dbManagementResponse"),ItemsList->xml);
			}
		}
		CString csStatus = _T("failure");
		if(bStatus == true)
		{
			csStatus = _T("success");
		}
		else if(bStatus == false)
		{
			csStatus = _T("failure");
		}
		_variant_t varStatus(csStatus);
		DBMngtResp->SetValue(_T("status"),varStatus);
		_bstr_t xmlStr = DBMngtResp->Getxml();
		g_cmDc.DoDCPrintf(TRACE_ALWAYS,(LPCSTR) xmlStr);
		sendStream(DBMngtResp);
		DBMngtResp.Release();
	}
	catch(_com_error& er)
	{
		g_cmDc.DoDCPrintf(TRACE_ALWAYS, _T("Unable to send Picklist Assist Message over pipe server,%s"), er.ErrorMessage() );
	}
	return hr;
}
*/

/// <summary>
/// Gets the object value.
/// </summary>
/// <param name="csData">value of the object</param>
/// <returns>Value as BString</returns>
_bstr_t CGTOControlObject::GetObjectValue(CString csData)
{
    return GetObjectValue(m_pGenObj, csData);
}

/// <summary>
/// Gets the object value.
/// </summary>
/// <param name="pGenObj">GTO Control object.</param>
/// <param name="csData">value of the object</param>
/// <returns>Value as BString</returns>
_bstr_t CGTOControlObject::GetObjectValue( IGTGenObjPtr pGenObjPtr, CString csData)
{	
	_variant_t var;
	_bstr_t str = _T("");
	try
	{
		var = pGenObjPtr->GetValue2(_bstr_t(csData));
		str = var.bstrVal;
	}
	catch(_com_error& ex)
	{
		//g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Unable to get object value , %s"), ex.ErrorMessage());
        Trace(TRACE_ERROR, _T("Unable to get object value , %s"), ex.ErrorMessage());
    }
	catch(...)
	{
		//g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Unable to get object value , Unknow Exception"));
        Trace(TRACE_ERROR, _T("Unable to get object value , Unknow Exception"));
    }
    return str;
}
inline void CGTOControlObject::SendStatus(int nStatusCode)
{
	CWinApp* pApp = AfxGetApp();
	if(pApp)
	{
		//pApp->m_pMainWnd->PostMessage(WM_STATUSMESSAGE, 0, nStatusCode);
	}
}

//bool CGTOControlObject::SendStatusAlert(PSSBERRORINFO pErrorInfo)
//{
//	g_GTOCtrlObj.sendStatusMessage(pErrorInfo);
//	return true;
//}
//<status>
//  <name></name>
//  <status>on/off/error</status>
//  <log></log>
//  <errorLevel></errorLevel>
//  <currentTimeUTC></currentTimeUTC>
//</status>

//HRESULT CGTOControlObject::sendStatusMessage(PSSBERRORINFO pError)
//{
//	g_cmDc.DoDCPrintf(TRACE_ALWAYS, _T("Sending SSB Status to Client Over Pipe Server"));
//	HRESULT hr =S_OK;
//	IGTGenObjPtr StatusMessage;
//	try
//	{
//		
//			//setup xml for GTO object
//			hr = StatusMessage.CreateInstance(__uuidof( CGTGenObj));
//			if(FAILED(hr))
//				throw _com_error(hr);
//
//			StatusMessage->Name = _T("status");
//			StatusMessage->IsXMLOutput = true;
//			StatusMessage->Validate = false;
//			StatusMessage->MsgId = 0xCF;
//			StatusMessage->IsNamedElementsXML = true;
//			StatusMessage->SetValue(_T("hdr/transID"), currentDateTime().c_str());
//			
//			if(pError != NULL)
//			{
//				CString csErrorInfo;
//				csErrorInfo.Format(_T("Error Code: %d, %s, SubSystem Error Code: %d, %s"), pError->stError.nErrorCode, pError->csErrorTxt, pError->stError.nSubSystemError, pError->csSubSytemErrorTxt);
//				StatusMessage->SetValue(_T("log"), csErrorInfo.GetString());
//				
//				if(pError->stError.enErroLevel == SSB_ERR)
//					StatusMessage->SetValue(_T("errorLevel"), _T("error"));
//				else if(pError->stError.enErroLevel == SSB_WARN)
//					StatusMessage->SetValue(_T("errorLevel"), _T("warn"));
//				else
//					StatusMessage->SetValue(_T("errorLevel"), _T("info"));
//
//				if(pError->stError.enErrorSatus == ONLINE)
//				{
//					StatusMessage->SetValue(_T("status"), _T("on"));
//				}
//				else
//				{
//					StatusMessage->SetValue(_T("status"), _T("off"));
//				}
//				delete pError;
//			}
//			sendStream(StatusMessage);
//			_bstr_t xmlStr = StatusMessage->Getxml();
//			g_cmDc.DoDCPrintf(TRACE_ALWAYS,(LPCSTR) xmlStr);
//			StatusMessage.Release();
//	}
//	catch(_com_error& er)
//	{
//		g_cmDc.DoDCPrintf(TRACE_ALWAYS, _T("Unable to send Picklist Assist Message over pipe server,%s"), er.ErrorMessage() );
//	}
//	return hr;
//}

//BOOL CGTOControlObject::GetBase64String(std::string a_ssFilename, std::string & a_str)
//{
//    BOOL bSuccess = TRUE;
//
//    // open the file:vec
//    std::ifstream file(a_ssFilename, std::ios::binary);
//
//    // Stop eating new lines in binary mode!!!
//    file.unsetf(std::ios::skipws);
//
//    // get its size:
//    std::streampos fileSize;
//
//    file.seekg(0, std::ios::end);
//    fileSize = file.tellg();
//    file.seekg(0, std::ios::beg);
//
//    // reserve capacity
//    std::string str;
//    str.reserve(fileSize);
//
//    // read the data:
//    str.insert(str.begin(),
//        std::istreambuf_iterator<char>(file),
//        std::istreambuf_iterator<char>());
//
//    std::string ssEncodedOutput;
//    std::string ssDecodedOutput;
//
//    CBase64EncodeDecode oBase64;
//
//    oBase64.Base64Encode(str.c_str(), str.length(), a_str);
//
//    //oBase64.Base64Decode(ssEncodedOutput.c_str(), ssEncodedOutput.length(), ssDecodedOutput);
//
//    //std::ofstream ofs;
//
//    //ofs.open(a_ssFilename.replace(a_ssFilename.find(".jpeg"), 5, "_out.jpeg"), std::ofstream::binary);
//
//    //ofs << ssDecodedOutput;
//    //ofs.close();
//
//    return bSuccess;
//}

void CGTOControlObject::PostStatusMsgToParent(const CString a_csStatusMsg)
{
    ::PostMessage(m_ParentHwnd, WM_USER_UPDATE_STATUS, 0, reinterpret_cast<LPARAM>(new CString(a_csStatusMsg))); \
}

void CGTOControlObject::PostImageMsgToParent(const std::vector<uint8_t> a_vecImage)
{
    ::PostMessage(m_ParentHwnd, WM_USER_UPDATE_IMAGE, 0, reinterpret_cast<LPARAM>(new std::vector<uint8_t>(a_vecImage)));
}


#include <fstream>      // std::ofstream

int CGTOControlObject::WriteToFile( const char * szFilename, _bstr_t a_bstrtData )
{
    std::ofstream ofs;

    ofs.open(szFilename, std::ofstream::out | std::ofstream::app);

    ofs << a_bstrtData;

    ofs.close();

    return 0;
}

void CGTOControlObject::SaveImage(std::vector<uint8_t>* imageBuffer, CString file_name)
{
    try
    {
        CFile saveImagefile;
        saveImagefile.Open(file_name, CFile::modeCreate | CFile::modeWrite);
        saveImagefile.SeekToBegin();
        saveImagefile.Write(imageBuffer->data(), (UINT)imageBuffer->size());
        saveImagefile.Close();
    }
    catch (...)
    {
        // check if error occurs due to save location issue
        //if (!CheckSaveImageLocation())
        //{
        //    m_check_autosaveimage_value_ = FALSE;
        //    m_button_save_.EnableWindow(TRUE);
        //    m_button_browse_.EnableWindow(TRUE);
        //}
    }
}

