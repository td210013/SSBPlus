#pragma once

#import "_GTO.tlb" no_namespace

class CPipeServer;

class ICallback
{
public:

    ICallback() {};

    virtual ~ICallback() {};

    virtual void OnRecieveMessage(CPipeServer *pPipeServer, IGTGenObjPtr pGenObjPtr ) = 0;
};

class CPipeServer
{
public:
    CPipeServer();

    virtual ~CPipeServer();

    BOOL Open(HWND a_hwndParent, ICallback * a_pCallback );

    BOOL Connect();
    void Disconnect();

    BOOL IsConnected();

    ICallback * GetCallback() { return m_pICallBack; }

    void Close();

    int Send( IGTGenObjPtr pGenObjPtr);

protected:

    static HRESULT WINAPI Callback(VARIANT vUser, GTConnectReceiveEvent tbcre, IDispatch *pObj);

    ICallback *m_pICallBack;

    HRESULT SetupConnection();

    IGTConnectPtr m_ptrConnection; //m_gPosConnection
    IGTGenObjPtr  m_ptrGenObj; // m_gpGenObj

    LONG m_lReadPort;
    LONG m_lWritePort;
    _bstr_t  m_bstrtServerName;

    BOOL m_bIsConnected;

    HWND m_hwndParent;
};

