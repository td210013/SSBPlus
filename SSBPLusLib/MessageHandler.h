#pragma once

#include "PipeServer.h"

class CMessageHandler :
    public ICallback
{
public:
    CMessageHandler();
    virtual ~CMessageHandler();

    void Open(HWND a_hwndParent);
    void Close();


    virtual void OnRecieveMessage(CPipeServer *pPipeServer, IGTGenObjPtr pGenObjPtr);

    void OnPaConfigMsg(CPipeServer * pPipeServer, IGTGenObjPtr pGenObjPtr);
    void OnPlaConfigMsg(CPipeServer * pPipeServer, IGTGenObjPtr pGenObjPtr);
    void OnItemDetail(IGTGenObjPtr pGenObjPtr);
    void OnGetMatchingItems(IGTGenObjPtr pGenObjPtr);
    void OnStateData(IGTGenObjPtr pGenObjPtr);
    void OnDbManagementStateData(IGTGenObjPtr pGenObjPtr);


protected:

    void PostStatusMsgToParent(const CString a_csStatusMsg);

    HWND m_hwndParent = NULL;
    IGTGenObjPtr m_ptrGenOb = NULL;

};

