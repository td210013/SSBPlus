#pragma once

#include <vector>

#import "_GTO.tlb" no_namespace

typedef void (WINAPI *LPEVENTCALLBACKPROC)(DWORD dwUser, GTConnectReceiveEvent tbcre, IDispatch * pObj);
// Class communicates with the pipeserver and handles the messages from the Pipe Server


class CGTOControlObject
{
public:
	CGTOControlObject(void);
	~CGTOControlObject(void);
	static BOOL InitializeGTOCO(HWND a_ParentHwnd);
	static BOOL UnintializeGTOCO();
    static void PostStatusMsgToParent(const CString a_csStatusMsg);
    static void PostImageMsgToParent(const std::vector<uint8_t> a_vecImage);

	void setPosConnection( IGTConnectPtr posConnection);
	void setGenObj( IGTGenObjPtr pGenObj);
    void sendStream( IGTGenObjPtr pGenObj);
    void sendStream(_bstr_t a_bstrtName, _bstr_t a_bstrtXml);

    //static BOOL GetBase64String(std::string a_ssFilename, std::string & a_str);

    //HRESULT SLHeartBeatMessage();	
	//long GetFrequency(){return m_hlFrequency;}
	//HANDLE& GetShutdownEvent();
protected:
	 IGTConnectPtr m_posConnection;
	 IGTGenObjPtr m_pGenObj;
	 IGTGenObjPtr m_pHDRGenObj;
	 void ReleaseGenObj();
public:

private:
	static HRESULT SetupConnection();
	static  IGTGenObjPtr m_gpGenObj;
	inline void CGTOControlObject::SendStatus(int nStatusCode);
    _bstr_t GetObjectValue(CString csData);
    _bstr_t GetObjectValue(IGTGenObjPtr pGenObjPtr, CString csData);

private:
	static IGTConnectPtr m_gPosConnection;
	static HRESULT WINAPI Callback(VARIANT vUser, GTConnectReceiveEvent tbcre, IDispatch *pObj);
	static CRITICAL_SECTION m_cs;
	CRITICAL_SECTION m_CSvariable;
private:
    void handlePostImageMessage();
    //void handleConfiguartionMessage();
	//void handlestateDataMessage();
	//void handlegetMatchingItems();
	//void handleitemDetail();
	//void handledbManagement();

    static int WriteToFile(const char * szFilename, _bstr_t a_bstrtData);
    void SaveImage(std::vector<uint8_t>* imageBuffer, CString file_name);


    static HWND m_ParentHwnd;
};

static CGTOControlObject g_GTOCtrlObj;
