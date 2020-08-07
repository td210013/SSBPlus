#include "stdafx.h"
#include "PipeServer.h"
#include "Logging.h"
#include "Config.h"

typedef void (WINAPI *LPEVENTCALLBACKPROC)(DWORD dwUser, GTConnectReceiveEvent tbcre, IDispatch * pObj);


HRESULT WINAPI CPipeServer::Callback(VARIANT vUser, GTConnectReceiveEvent /*tbcre*/, IDispatch *pObj)
{
    if (vUser.byref != NULL)
    {
        CPipeServer * pServer = (CPipeServer *)vUser.byref;

        pServer->GetCallback()->OnRecieveMessage(pServer, pObj);
    }
    else
    {
        Trace(TRACE_ERROR, _T("Error in PipeServer callback, vUser is NULL") );
    }

    return S_OK;
}

CPipeServer::CPipeServer() : 
    m_ptrConnection(NULL), 
    m_bIsConnected(FALSE)
{
}

CPipeServer::~CPipeServer()
{
}

BOOL CPipeServer::Open(HWND a_hwndParent, ICallback *a_pICallBack)
{
    BOOL bResult = TRUE;
    try
    {
        CoInitialize(NULL);

        HRESULT hr;
        IGTGenObjPtr item;
        // Create the GTO control instance
        hr = item.CreateInstance(__uuidof(CGTGenObj));
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

        m_hwndParent = a_hwndParent;
        m_pICallBack = a_pICallBack;
    }
    catch (_com_error& er)
    {
        bResult = FALSE;
        // g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Unable to Initialize the GTO Control %s"), er.ErrorMessage());
        Trace(TRACE_ERROR, _T("Unable to Initialize the GTO Control %s"), er.ErrorMessage());
    }
    catch (...)
    {
        bResult = FALSE;
        // g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Unable to Initialize the GTO Control Unknow Exception"));
        TRACE("Unable to Initialize the GTO Control Unknow Exception");
    }
    return bResult;
}

BOOL CPipeServer::Connect()
{
    try
    {
        HRESULT hr;

        // Connect to the Pipeserver
        hr = m_ptrConnection->ConnectEx(m_bstrtServerName, m_lReadPort, m_lWritePort, _bstr_t(_T("")));
        if (hr != S_OK)
        {
            Trace(TRACE_ERROR, _T("Failed to connect to pipeserver, error code: %x"), hr);
            // Close();
            //UnintializeGTOCO();
            return FALSE;
        }

        // g_GTOCtrlObj.setPosConnection(m_ptrConnection);

        LPEVENTCALLBACKPROC pCallback = (LPEVENTCALLBACKPROC)Callback;
        _variant_t vtCallback;
        vtCallback.byref = (void*)pCallback;
        vtCallback.vt = VT_BYREF | VT_I4;
        _variant_t vtUser;
        // vtUser.byref = (void*)NULL;
        // vtUser.vt = VT_BYREF | VT_I4;
        vtUser.byref = (void*)this;
        vtUser.vt = VT_BYREF | VT_PTR;
        // Set the Call back Function
        hr = m_ptrConnection->SetCallback(vtCallback, vtUser);
        if (hr != S_OK)
        {
            Trace(TRACE_ERROR, _T("Failed to connect to pipeserver, error code: %x"), hr);
            Close();
            m_bIsConnected = FALSE;
        }
        else
        {
            m_bIsConnected = TRUE;
        }
    }
    catch (_com_error& er)
    {
        m_bIsConnected = FALSE;
        // g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Unable to Initialize the GTO Control %s"), er.ErrorMessage());
        Trace(TRACE_ERROR, _T("Unable to Initialize the GTO Control %s"), er.ErrorMessage());
    }
    catch (...)
    {
        m_bIsConnected = FALSE;
        // g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Unable to Initialize the GTO Control Unknow Exception"));
        TRACE("Unable to Initialize the GTO Control Unknow Exception");
    }

    return m_bIsConnected;
}

void CPipeServer::Disconnect()
{
    if (m_ptrConnection != NULL)
    {
        HRESULT hr; 

        hr = m_ptrConnection->Disconnect();

        if (hr != S_OK)
        {
            Trace(TRACE_ERROR, _T("Disconnect failed - error code: %x"), hr);
        }
    }
    else
    {
        Trace(TRACE_ERROR, _T("ERROR! Disconnect() - non connetion instance"));
    }
}

BOOL CPipeServer::IsConnected()
{
    return m_bIsConnected;
}



void CPipeServer::Close()
{
    if (m_ptrConnection != NULL)
    {
        m_ptrConnection->Disconnect();
        m_ptrConnection.Release();
        m_ptrConnection = NULL;
    }
}

int CPipeServer::Send(IGTGenObjPtr pGenObjPtr)
{
    IGTStreamPtr stream = pGenObjPtr;
    try
    {
        m_ptrConnection->Send(stream);
    }
    catch (_com_error& er)
    {
        Trace(TRACE_ERROR, _T("Unable to send the stream over pipe server,%s, Please restart IMA"), er.ErrorMessage());
    }
    catch (...)
    {
        Trace(TRACE_ERROR, _T("Unable to send the stream over pipe server, Unhandled Exception. Please restart IMA"));
    }
    return 0;
}

HRESULT CPipeServer::SetupConnection()
{
    HRESULT hr = S_OK;

    hr = m_ptrConnection.CreateInstance(__uuidof(CGTConnect));
    if (hr != S_OK)
        throw _com_error(hr);


    //Read Pipeserver details from the registry
    _TCHAR szServerName[_MAX_PATH + 1];
    _TCHAR szReadPort[_MAX_PATH + 1];
    _TCHAR szWritePort[_MAX_PATH + 1];

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
        Close();
        // UnintializeGTOCO();
        return E_FAIL;

    }
    lRC = ::RegQueryValueEx(hKey,
        _T("ServerName"),
        NULL,
        NULL,
        (LPBYTE)szServerName,
        &dwDataSize);
    if (lRC != ERROR_SUCCESS)
    {
        Trace(TRACE_ERROR, _T("Failed to open registry, error code: %ld"), lRC);
        Close();
        //UnintializeGTOCO();
        return E_FAIL;

    }
    dwDataSize = _MAX_PATH;

    lRC = ::RegQueryValueEx(hKey,
        _T("SocketReadPort"),
        NULL,
        NULL,
        (LPBYTE)szReadPort,
        &dwDataSize);

    if (lRC != ERROR_SUCCESS)
    {
        Trace(TRACE_ERROR, _T("Failed to open registry, error code: %ld"), lRC);
        Close();
        //UnintializeGTOCO();
        return E_FAIL;

    }

    dwDataSize = _MAX_PATH;

    lRC = ::RegQueryValueEx(hKey,
        _T("SocketWritePort"),
        NULL,
        NULL,
        (LPBYTE)szWritePort,
        &dwDataSize);

    if (lRC != ERROR_SUCCESS)
    {
        Trace(TRACE_ERROR, _T("Failed to open registry, error code: %ld"), lRC);
        Close();
        //UnintializeGTOCO();
        return E_FAIL;

    }

    RegCloseKey(hKey);

    m_bstrtServerName = szServerName;
    m_lReadPort = _tstol(szReadPort);
    m_lWritePort = _tstol(szWritePort);

    //Subscribe to the Messages
    hr = m_ptrConnection->SubscribeTo(GetConfig()->csMessageSubscriptionList.GetString());

    if (hr != S_OK)
    {
        Trace(TRACE_ERROR, _T("Failed to subscribe to pipeserver messages: : %s"), GetConfig()->csMessageSubscriptionList.GetString());
        Close();
        //UnintializeGTOCO();
        return E_FAIL;
    }

    return hr;
}

