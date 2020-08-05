#pragma once
// #include "ScanWatchSecurityBrokerAPI.h"
//SSCOB-14486 Decoupled the IMA from Fastlane Appplication code.
#import "_GTO.tlb" no_namespace
typedef void (WINAPI *LPEVENTCALLBACKPROC)(DWORD dwUser, GTConnectReceiveEvent tbcre, IDispatch * pObj);
// Class communicates with the pipeserver and handles the messages from the Pipe Server

// #include "DeviceConnector.h"
#include "NcrZebraCamera.h"
#include "SSBConstants.h"
#include "vector"
#include "string"

class CGTOControlObject
{
public:
	CGTOControlObject(void);
	~CGTOControlObject(void);
	static BOOL InitializeGTOCO(CWnd& hWnd, NcrZebraCamera * pNcrZebra );
	static BOOL UnintializeGTOCO();
    static void PostStatusMsgToParent(const CString a_csStatusMsg);
	void setPosConnection( IGTConnectPtr posConnection);
	void setGenObj( IGTGenObjPtr pGenObj);
    void sendStream( IGTGenObjPtr pGenObj);

    // TMD commented out
    HRESULT SendPLAMessage(PSSBPLARESPONSE pRes);
	HRESULT SendAlertMessage(PSSBVALRESPONSE pRes);
    static HRESULT SendImage(NcrImageData imaData);
    static BOOL GetBase64String(std::string a_ssFilename, std::string& a_str);


	//HRESULT sendStatusMessage(PSSBERRORINFO pError);

    //HRESULT SLHeartBeatMessage();	
	//long GetFrequency(){return m_hlFrequency;}
	//HANDLE& GetShutdownEvent();
protected:
	 IGTConnectPtr m_posConnection;
	 IGTGenObjPtr m_pGenObj;
	 IGTGenObjPtr m_pHDRGenObj;
	 void ReleaseGenObj();
public:
    // TMD commented out

 //   static void LogImageInfo(IMAGEDATA imaData);

    bool SendPLA(PSSBPLARESPONSE pRes);
	bool SendAlert(PSSBVALRESPONSE pRes);
    static bool SendStatusAlert(PSSBERRORINFO pError);
private:
	static HRESULT SetupConnection();
	static  IGTGenObjPtr m_gpGenObj;
	inline void CGTOControlObject::SendStatus(int nStatusCode);

private:
	static IGTConnectPtr m_gPosConnection;
	static HRESULT WINAPI Callback(VARIANT vUser, GTConnectReceiveEvent tbcre, IDispatch *pObj);
	static CRITICAL_SECTION m_cs;
	_bstr_t GetObjectValue(CString csData);
	_bstr_t GetObjectValue( IGTGenObjPtr pGenObjPtr, CString csData);
	CRITICAL_SECTION m_CSvariable;
private:
	void handleConfiguartionMessage();
	void handlestateDataMessage();
	void handlegetMatchingItems();
	void handleitemDetail();
	void handledbManagement();

    static BOOL GetDeviceImage(int a_nCameraNum, BOOL a_bIsRefImage);


	HRESULT sendConfigResponse(int nCode, SSBPACONFIG& stSSBPAConfig);
	HRESULT sendConfigResponse(int nCode, SSBPLACONFIG& stSSBPLAConfig);
	HRESULT sendConfigResponse();

    static HWND m_ParentHwnd;
    static CString sm_csItemWeight;
    static CString sm_csItemUPC;
    static CString sm_csItemQty;


	//HRESULT sendDBResponse(bool bstatus, SSBDBITEMS& stSSBDBItems);

    static std::vector<int> m_vCameraIdList;
	
    static NcrZebraCamera * m_pNcrZebra;
};

static CGTOControlObject g_GTOCtrlObj;
