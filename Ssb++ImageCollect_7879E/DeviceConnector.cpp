
#include "stdafx.h"
#include "DeviceConnector.h"
#include "OPOS.h"
#include "Resource.h"
#include "DeviceException.h"
#include "SSBErrors.h"
#include "Logging.h"

#define DEVICE_NOT_CLAIMED -1
#define EXTERNAL_CHECK_HEALTH 2

CDeviceConnector* CDeviceConnector::m_pDevice = NULL;

#define DEFAULT_DEVICE_PROFILE _T("NCRImageScanner.Scanwatch")

#define CATCH_DEVICE_EXCEPTION \
catch (CDeviceException ex) \
{ \
    CString * pMessage = new CString(); /*string is release on message handler */  \
    pMessage->Format(_T("%s, error code: %ld, lib code: %ld"), CString(ex.GetErrorDesc().c_str()), ex.GetErrorCode(), ex.GetLibErrorCode()); \
                                    \
    Trace(TRACE_ERROR, *pMessage);  \
                                    \
    ::PostMessage(m_ParentHwnd , WM_USER_UPDATE_STATUS, 0, reinterpret_cast<LPARAM>(pMessage)); \
        \
    return false;                   \
}                                   \
catch (...)                             \
{                                       \
    CString * pMessage = new CString(); \
    pMessage->Format(_T("General exception caught, get last error returned: %ld"), ::GetLastError()); \
    \
    Trace(TRACE_ERROR, *pMessage);  \
    \
    ::PostMessage(m_ParentHwnd , WM_USER_UPDATE_STATUS, 0, reinterpret_cast<LPARAM>(pMessage)); \
        \
    return false; \
} 


/*
#define CATCH_GENERAL_EXCEPTION  \
catch (...)                             \
{                                       \
    CString * pMessage = new CString(); \
    pMessage->Format(_T("General exception caught, get last error returned: %ld"), ::GetLastError()); \
    \
    Trace(TRACE_ERROR, *pMessage);  \
    \
    ::PostMessage(hWnd, WM_USER_UPDATE_STATUS, 0, reinterpret_cast<LPARAM>(pMessage)); \
        \
    return false; \
} 
*/



/**
* @brief: Constructor
*/
CDeviceConnector::CDeviceConnector(void)  : CDCInterface(), m_bDeviceOpen(FALSE), m_ParentHwnd(NULL)
{
	m_pDataRecived = new CEvent();
	//m_hEventLog = RegisterEventSource(NULL, "ScanWatch Security Broker");
}

/**
* @brief Destructor
*/
CDeviceConnector::~CDeviceConnector(void)
{
	if(m_pDataRecived != NULL)
	{
		CloseHandle(m_pDataRecived->m_hObject);
        m_pDataRecived->m_hObject = NULL;
		delete m_pDataRecived;
		m_pDataRecived = NULL;
	}
}

/**
* @brief: It will create and open the device.
*/
bool CDeviceConnector::InitializeDM(CWnd& hWnd, CString& csStatus, CString csProfile)
{
    m_ParentHwnd = hWnd.m_hWnd;

	/*---------------------------------------------------------
	// 1. Create device
	// 2. Open
	// 3. Claim
	// 4. SetEnable
	// 5. CheckHealth
	// 6. GetCameraInfo
	// 7. Check for Device Capabilites
	-----------------------------------------------------------*/
	// // g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("[DC] Info: Intializing the Device"));
    LPCTSTR ltSuccess = _T("Initialized Image Scanner successfully");

	bool bResult = true;
	long lResult = OPOS_SUCCESS;

	if(csProfile.IsEmpty())
	{
        csProfile = DEFAULT_DEVICE_PROFILE;
	}
	// Create Device
	try
	{
        //throw CDeviceException(SSB_DM_ERROR_INITDEVICE, 123456, "SSB_DM_ERROR_UNABLE_TO_ENABLE_STR");

		if(!m_oDevCaps.CheckUVCDeviceEnabled())
		{
			throw CDeviceException(SSB_DM_ERROR_INITDEVICE,0,"SSB_DM_EEROR_UVC_NOT_ENABLED_STR");
		}
		BOOL bStatus = m_devImagerScanner.Create(_T("OPOS.ImagerScanner"), NULL, NULL, CRect(0,0,0,0), &hWnd, IDC_OPOSIMAGERSCANNER);

		if(!bStatus)
			throw CDeviceException(SSB_DM_ERROR_INITDEVICE,0,"SSB_DM_ERROR_UNABLE_TO_CREATE_STR");

		// Open Device
		lResult = m_devImagerScanner.Open(csProfile);
		if(lResult != 0)
			throw CDeviceException(SSB_DM_ERROR_INITDEVICE,lResult, "SSB_DM_ERROR_UNABLE_TO_OPEN_STR");

		m_bDeviceOpen = TRUE;
		// Claim Device
		lResult = m_devImagerScanner.ClaimDevice(0);
		if(lResult != 0)
		{
			// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Retry to claim the device again...."));

			if(m_bDeviceOpen)
			{
				lResult = m_devImagerScanner.Close();
				if(lResult != OPOS_SUCCESS)
					throw CDeviceException(SSB_DM_ERROR_INITDEVICE,lResult,"SSB_DM_ERROR_CLOSED_STR");
				m_bDeviceOpen = false;
			}

			CEvent*  hWaitEvent = new CEvent();
			hWaitEvent->ResetEvent();
			WaitForSingleObject(hWaitEvent->m_hObject, 2000);
		

			// Open Device	
			lResult = m_devImagerScanner.Open(csProfile);
			if(lResult != 0)
				throw CDeviceException(SSB_DM_ERROR_INITDEVICE,lResult, "SSB_DM_ERROR_UNABLE_TO_OPEN_STR");

			m_bDeviceOpen = true;

			lResult = m_devImagerScanner.ClaimDevice(0);
			if(lResult != 0)
				throw CDeviceException(SSB_DM_ERROR_INITDEVICE,lResult,"SSB_DM_ERROR_UNABLE_TO_CLAIM_STR");
		}
		// Set Power/Reset Notify
		m_devImagerScanner.put_PowerNotify(OPOS_PN_ENABLED);

		// Enable device
		lResult = m_devImagerScanner.get_DeviceEnabled();
		if(lResult != 0)
			throw CDeviceException(SSB_DM_ERROR_INITDEVICE,lResult,"SSB_DM_ERROR_UNABLE_TO_ENABLE_STR");

		m_devImagerScanner.put_DeviceEnabled(TRUE);

		//Check Health
		lResult = m_devImagerScanner.CheckHealth(EXTERNAL_CHECK_HEALTH);
		if(lResult != OPOS_SUCCESS)
			throw CDeviceException(SSB_DM_ERROR_INITDEVICE,lResult,"SSB_DM_ERROR_CHECKHEALTH_FAILED_STR");

		// Get Camera Info.
		bool isCamInfoOk = GetCameraInfo(m_ucCameraInfoData);
		if (!isCamInfoOk)
		{
			throw CDeviceException(SSB_DM_ERROR_INITDEVICE,lResult,"SSB_DM_ERROR_GETCAMERAINFO_STR");
		}
		bool  breturn  = m_oDevCaps.SetDevCapsfromCamInfo(m_ucCameraInfoData);
		if(breturn)
		{
			if(!m_oDevCaps.IsModelSupported())
			{
				throw CDeviceException(SSB_DM_ERROR_MODEL_NOTSUPPORTED,0,"SSB_DM_ERROR_INITDEVICE_STR");
			}
			if(!m_oDevCaps.IsScaleOnline())
			{
				throw CDeviceException(SSB_DM_ERROR_SCALE_OFFLINE,0,"SSB_DM_ERROR_INITDEVICE_STR");
			}
			if(!m_oDevCaps.IsFirmwareSupported())
			{
				throw CDeviceException(SSB_DM_ERROR_FW_NOTSUPPORTED,0,"SSB_DM_ERROR_INITDEVICE_STR");
			}
			if(!m_oDevCaps.IsScanwatchEnabled())
			{
				throw CDeviceException(SSB_DM_ERROR_SCANWATCH_DISABLED,0,"SSB_DM_ERROR_INITDEVICE_STR");
			}
		}
		else
		{
			throw CDeviceException(SSB_DM_ERROR_INITDEVICE,0,"SSB_DM_ERROR_INVAL_CAMERA_INFO_STR");
		}
		//m_stErrorDetails.enErrorSatus = ONLINE;
        /*
		int iRet = ReportEvent(m_hEventLog, EVENTLOG_SUCCESS, 0, SSB_SUCCESS, NULL, 1, 0,&ltSuccess, NULL);
		if(iRet == 0)
		{
			int iError = GetLastError();
			CString messageBuffer = _T("");
			FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, iError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
			CString csError = _T("");
			csError.Format("Report Event failed with Error code %i:%s",iError,messageBuffer);
			// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("[DM] Info: %s"),csError);
		}
        */
	}

    CATCH_DEVICE_EXCEPTION
        /*
	catch(CDeviceException ex)
	{
        CString * pMessage = new CString();
        pMessage->Format(_T("%s, error code: %ld, lib code: %ld"), CString(ex.GetErrorDesc().c_str()), ex.GetErrorCode(), ex.GetLibErrorCode());

        Trace(TRACE_ERROR, *pMessage);

        ::PostMessage(hWnd, WM_USER_UPDATE_STATUS, 0, reinterpret_cast<LPARAM>(pMessage));

		return false;
	}
	catch(...)
	{
        CString * pMessage = new CString();

        pMessage->Format(_T("General exception caught, get last error returned: %ld"), ::GetLastError() );
        Trace(TRACE_ERROR, *pMessage);

        ::PostMessage(hWnd, WM_USER_UPDATE_STATUS, 0, reinterpret_cast<LPARAM>(pMessage));

		return false;
	}
    */

	return bResult;
}

/*
* @brief: When window/dialog recieves the data event it will call this function.
*		  This function will get all information about the image.
*/
bool CDeviceConnector::setDataEvent()
{
	// g_cmDc.DoDCPrintf(TRACE_OUTPUT_LEVEL1,_T("++[DM] Info: SetEvent Called"));
	bool bResult = true;
	try
	{
		if(IsClaimed())
		{
			// retriving image information from Device
			m_stImageData.nImageHeight = m_devImagerScanner.get_ImageHeight();
			m_stImageData.nBitsPerPixel = m_devImagerScanner.get_BitsPerPixel();
			m_stImageData.nFrameType = m_devImagerScanner.get_FrameType();
			m_stImageData.nImageType = m_devImagerScanner.get_ImageType();
			m_stImageData.nImageLength = m_devImagerScanner.get_ImageLength();
			m_stImageData.nImageWidth = m_devImagerScanner.get_ImageWidth();
			m_stImageData.cImgePath = CStringA(m_devImagerScanner.get_FrameData());

			if(m_pDataRecived)
			{
                m_devImagerScanner.put_DataEventEnabled(TRUE);
				// bResult = SetEvent(m_pDataRecived->m_hObject)?true:false;
			}

			//if(bResult)
			//	 g_cmDc.DoDCPrintf(TRACE_OUTPUT_LEVEL1,_T("[DM] Info: SetEvent Successful"));
			//else
			//	 g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("[DM] Error!!! SetEvent failed"));
		}
		else
			bResult = false;
	}
    CATCH_DEVICE_EXCEPTION

	//catch(...)
	//{
	//	//CSSBException oSSBExp(SSB_SYSTEM_UNKNOWN_EXCEPTION,0,_T("Unhandled Exception in Device Manager SetDataEvent"));
	//	//m_stErrorDetails = oSSBExp.GetErrorInfo();
	//	//// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("%s"),oSSBExp.GetCompleteErrorDesc().c_str());
	//	return false;
	//}

	return bResult;
}

bool CDeviceConnector::CheckforDataEvent(IMAGEDATA &imaData)
{

	// g_cmDc.DoDCPrintf(TRACE_OUTPUT_LEVEL1,_T("[DM] Info: Check for Data Event"));
	bool bResult = true;
	try
	{
		// It will wait for device to send the data event.
		//DWORD dwRet = WaitForSingleObject(m_pDataRecived->m_hObject,500);
		//if(dwRet != WAIT_OBJECT_0)
		//{
		//	throw CDeviceException(SSB_DM_ERROR_DATA_EVENT,0,"SSB_DM_ERROR_GETTINGIMAGE_STR");
		//}
		// m_pDataRecived->ResetEvent();
		// g_cmDc.DoDCPrintf(TRACE_OUTPUT_LEVEL1,_T("[DM] Info: Data Event Recived"));
		imaData.nImageHeight=m_stImageData.nImageHeight;
		imaData.nBitsPerPixel=m_stImageData.nBitsPerPixel;
		imaData.nFrameType=m_stImageData.nFrameType;
		imaData.nImageType= m_stImageData.nImageType;
		imaData.nImageLength=m_stImageData.nImageLength;
		imaData.nImageWidth=m_stImageData.nImageWidth;
		imaData.cImgePath = m_stImageData.cImgePath;
		// g_cmDc.DoDCPrintf(TRACE_OUTPUT_LEVEL1,_T("[DM] Info: Image Received"));
		if(imaData.nImageType != IMG_TYPE_ITEM)
		{
			// bResult = false;
            bResult = true;
            // g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Unsupported Image Type, Change the image type to YUY2, Error Code: %d"),SSB_DM_ERROR_GETTINGIMAGE);
			// throw CDeviceException(SSB_DM_ERROR_GETTINGIMAGE,0,_T("Unsupported Image Type, Change the image type to YUY2"));
            TRACE1("Failed to get IMAGEDATA - Invalid image type: %d", imaData.nImageType);

		}
        m_pDataRecived->SetEvent();
	}
    CATCH_DEVICE_EXCEPTION

	//catch(CDeviceException ex)
	//{
	//	//if(ex.GetErrorInfo().nSubSystemError == OPOS_E_EXTENDED || ex.GetErrorInfo().nSubSystemError == OPOS_E_FAILURE)
	//	//{
	//	//	long lexcode = m_devImagerScanner.get_ResultCodeExtended();
	//	//	if(lexcode != 0)
	//	//		ex.SetExtErrorCode(lexcode);
	//	//}
	//	//m_stErrorDetails = ex.GetErrorInfo();
	//	//// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("%s"),ex.GetCompleteErrorDesc().c_str());
 //       return false;
	//}
	//catch(...)
	//{
	//	//CSSBException oSSBExp(SSB_SYSTEM_UNKNOWN_EXCEPTION,0,_T("Unhandled Exception in Device Manager CheckforDataEvent"));
	//	//m_stErrorDetails = oSSBExp.GetErrorInfo();
	//	// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("%s"),oSSBExp.GetCompleteErrorDesc().c_str());
	//	return false;
	//}
	return bResult;
}

/**
* @brief: Retrives the Image from the scanner
*/
bool CDeviceConnector::StartSession()
{
	// g_cmDc.DoDCPrintf(TRACE_OUTPUT_LEVEL1,_T("[DM] Info: Start Session"));
	bool bResult = true;
	long lResult = OPOS_SUCCESS;
	try
	{
		if(IsClaimed())
		{
			m_devImagerScanner.ClearInput();
			// Issue with Data event when start session fails - Clear the event
			m_pDataRecived->ResetEvent();
			// Enable the Data Event
			m_devImagerScanner.put_DataEventEnabled(TRUE);
			// Start Session
			lResult = 	m_devImagerScanner.StartSession();
			if(lResult != OPOS_SUCCESS)
				throw CDeviceException(SSB_DM_ERROR_STARTSESSION,lResult,"");
			// g_cmDc.DoDCPrintf(TRACE_OUTPUT_LEVEL1,_T("[DM] Info: Started Session Successfully"));
		}
		else
			bResult = false;
	}
    CATCH_DEVICE_EXCEPTION
 //   catch(CDeviceException ex)
	//{
	//	//if(ex.GetErrorInfo().nSubSystemError == OPOS_E_EXTENDED || ex.GetErrorInfo().nSubSystemError == OPOS_E_FAILURE)
	//	//{
	//	//	long lexcode = m_devImagerScanner.get_ResultCodeExtended();
	//	//	if(lexcode != 0)
	//	//		ex.SetExtErrorCode(lexcode);
	//	//}
	//	//m_stErrorDetails = ex.GetErrorInfo();
	//	//// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("%s"),ex.GetCompleteErrorDesc().c_str());
	//	return false;
	//}
	//catch(...)
	//{
	//	//CSSBException oSSBExp(SSB_SYSTEM_UNKNOWN_EXCEPTION,0,_T("Unhandled Exception in Device Manager StartSession"));
	//	//m_stErrorDetails = oSSBExp.GetErrorInfo();
	//	// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("%s"),oSSBExp.GetCompleteErrorDesc().c_str());
	//	return false;
	//}
	return bResult;
}
/**
* @brief: Ends the Session.
*/
bool CDeviceConnector::StopSession()
{
	// g_cmDc.DoDCPrintf(TRACE_OUTPUT_LEVEL1,_T("[DM] Info: Stop Session"));
	bool bResult = true;
	long lResult = OPOS_SUCCESS;
	try
	{
		if(IsClaimed())
		{
			m_devImagerScanner.put_DataEventEnabled(FALSE);
			lResult = 	m_devImagerScanner.StopSession();
			if(lResult != OPOS_SUCCESS)
				throw CDeviceException(SSB_DM_ERROR_STOPSESSION,lResult,"");
			// g_cmDc.DoDCPrintf(TRACE_OUTPUT_LEVEL1,_T("[DM] Info: Stopped Session Successfully"));
		}
		else
			bResult = false;
	}
    CATCH_DEVICE_EXCEPTION

	//catch(CDeviceException ex)
	//{
	//	//if(ex.GetErrorInfo().nSubSystemError == OPOS_E_EXTENDED || ex.GetErrorInfo().nSubSystemError == OPOS_E_FAILURE)
	//	//{
	//	//	long lexcode = m_devImagerScanner.get_ResultCodeExtended();
	//	//	if(lexcode != 0)
	//	//		ex.SetExtErrorCode(lexcode);
	//	//}
	//	//m_stErrorDetails = ex.GetErrorInfo();
	//	// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("%s"),ex.GetCompleteErrorDesc().c_str());
	//	return false;
	//}
	//catch(...)
	//{
	//	//CSSBException oSSBExp(SSB_SYSTEM_UNKNOWN_EXCEPTION,0,_T("Unhandled Exception in Device Manager StopSession"));
	//	//m_stErrorDetails = oSSBExp.GetErrorInfo();
	//	// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("%s"),oSSBExp.GetCompleteErrorDesc().c_str());
	//	return false;
	//}
	return bResult;
}
/*
* @brief: Closes and releases the device.
*/
bool CDeviceConnector::ReleaseDevice()
{
	// g_cmDc.DoDCPrintf(TRACE_OUTPUT_LEVEL1,_T("[DM] Info: Release Device"));
	bool bResult = true;
	long lResult = OPOS_SUCCESS;
	try
	{
		if(m_bDeviceOpen)
		{
			if(IsClaimed())
			{
				m_bDeviceOpen = FALSE;
				m_devImagerScanner.put_DeviceEnabled(FALSE);
				lResult = m_devImagerScanner.ReleaseDevice();
				if(lResult != OPOS_SUCCESS)
					throw CDeviceException(SSB_DM_ERROR_RELEASEDEVICE,lResult,"Unable to Release the Device");

				lResult = m_devImagerScanner.Close();
				if(lResult != OPOS_SUCCESS)
					throw CDeviceException(SSB_DM_ERROR_RELEASEDEVICE,lResult,"Unable to Close the Device");

				BOOL bDestroy = m_devImagerScanner.DestroyWindow();
				if(!bDestroy)
					throw CDeviceException(SSB_DM_ERROR_RELEASEDEVICE,0,"SSB_DM_ERROR_DESTROYWINDOW_STR");
				// g_cmDc.DoDCPrintf(TRACE_OUTPUT_LEVEL1,_T("[DM] Info: Released Device Successfully"));
			}
			else
			{
				lResult = m_devImagerScanner.Close();
				if(lResult != OPOS_SUCCESS)
					throw CDeviceException(SSB_DM_ERROR_RELEASEDEVICE,lResult,"SSB_DM_ERROR_CLOSED_STR");

				BOOL bDestroy = m_devImagerScanner.DestroyWindow();
				if(!bDestroy)
					throw CDeviceException(SSB_DM_ERROR_RELEASEDEVICE,0,"SSB_DM_ERROR_DESTROYWINDOW_STR");
				// g_cmDc.DoDCPrintf(TRACE_OUTPUT_LEVEL1,_T("[DM] Info: Released Device Successfully"));
				m_bDeviceOpen = false;
			}
		}
		else
		{
			BOOL bDestroy = m_devImagerScanner.DestroyWindow();
			if(!bDestroy)
				throw CDeviceException(SSB_DM_ERROR_RELEASEDEVICE,0,"SSB_DM_ERROR_DESTROYWINDOW_STR");
			// g_cmDc.DoDCPrintf(TRACE_OUTPUT_LEVEL1,_T("[DM] Info: Distroyed the device Window Successfully"));
			m_bDeviceOpen = false;
			bResult = false;
		}
	}
    CATCH_DEVICE_EXCEPTION

	//catch(CDeviceException ex)
	//{
	//	//if(ex.GetErrorInfo().nSubSystemError == OPOS_E_EXTENDED || ex.GetErrorInfo().nSubSystemError == OPOS_E_FAILURE)
	//	//{
	//	//	long lexcode = m_devImagerScanner.get_ResultCodeExtended();
	//	//	if(lexcode != 0)
	//	//		ex.SetExtErrorCode(lexcode);
	//	//}
	//	//m_stErrorDetails = ex.GetErrorInfo();
	//	// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("%s"),ex.GetCompleteErrorDesc().c_str());
	//	return false;
	//}
	//catch(...)
	//{
	//	//CSSBException oSSBExp(SSB_SYSTEM_UNKNOWN_EXCEPTION,0,_T("Unhandled Exception in Device Manager ReleaseDevice"));
	//	//m_stErrorDetails = oSSBExp.GetErrorInfo();
	//	// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("%s"),oSSBExp.GetCompleteErrorDesc().c_str());
	//	return false;
	//}
	return bResult;
}
/*
* @brief:  Checks whether the device is calimed or not.
*/
bool CDeviceConnector::IsClaimed()
{
	bool bResult = true;
	try
	{
		bResult = m_devImagerScanner.get_Claimed()?true:false;
		if(!bResult)
			throw CDeviceException(SSB_DM_ERROR_DEVICE_NOT_CLAIMED,0,"");
	}
    CATCH_DEVICE_EXCEPTION

	//catch(CDeviceException ex)
	//{
	//	//if(ex.GetErrorInfo().nSubSystemError == OPOS_E_EXTENDED || ex.GetErrorInfo().nSubSystemError == OPOS_E_FAILURE)
	//	//{
	//	//	long lexcode = m_devImagerScanner.get_ResultCodeExtended();
	//	//	if(lexcode != 0)
	//	//		ex.SetExtErrorCode(lexcode);
	//	//}
	//	//m_stErrorDetails = ex.GetErrorInfo();
	//	// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("%s"),ex.GetCompleteErrorDesc().c_str());
	//	return false;
	//}
	//catch(...)
	//{
	//	//CSSBException oSSBExp(SSB_SYSTEM_UNKNOWN_EXCEPTION,0,_T("Unhandled Exception in Device Manager IsClaimed"));
	//	//m_stErrorDetails = oSSBExp.GetErrorInfo();
	//	// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("%s"),oSSBExp.GetCompleteErrorDesc().c_str());
	//	return false;
	//}
	return bResult;
}


/*
void ReadFileB64b(std::string a_ssFilename)
{
    // open the file:vec
    std::ifstream file(a_ssFilename, std::ios::binary);

    // Stop eating new lines in binary mode!!!
    file.unsetf(std::ios::skipws);

    // get its size:
    std::streampos fileSize;

    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // reserve capacity
    std::string vec;
    vec.reserve(fileSize);

    // read the data:
    vec.insert(vec.begin(),
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>());

    std::string ssEncodedOutput;
    std::string ssDecodedOutput;

    CBase64EncodeDecode oBase64;

    oBase64.Base64Encode(vec.c_str(), vec.length(), ssEncodedOutput);

    oBase64.Base64Decode(ssEncodedOutput.c_str(), ssEncodedOutput.length(), ssDecodedOutput);

    std::ofstream ofs;

    ofs.open(a_ssFilename.replace(a_ssFilename.find(".jpeg"), 5, "_out.jpeg"), std::ofstream::binary);

    ofs << ssDecodedOutput;
    ofs.close();
}
*/

/**
* @brief Gets camera setup information from the scanner.
*/
bool CDeviceConnector::GetCameraInfo(unsigned char *camInfo)
{
	bool bResult = true;
	BSTR bstrCommand = NULL;
	long getmetadata = 0;
	CString msg = _T("");
	char camInfoData[SCANWATCH_CAMERAINFO_SIZE];
	memset(camInfoData, 0, sizeof(camInfoData));
	// g_cmDc.DoDCPrintf(TRACE_ALWAYS, _T("[DM] Info: Get Camera Info\n"));

	try
	{
		if(IsClaimed())
		{
			getmetadata = SCANWATCH_GET_CAMERAINFO;
			m_devImagerScanner.DirectIO(NCRDIO_SCANWATCH, &getmetadata, &bstrCommand);
			// Check for the DIO metadata size.
			// g_cmDc.DoDCPrintf(TRACE_ALWAYS, _T("Camera Info Length = %d\n"), getmetadata);
			if (getmetadata == SCANWATCH_CAMERAINFO_SIZE)
			{
				// Save camera info data.
				msg += _T("Camera Info Data:\n");
				for (int i = 0,j=0; i < getmetadata; ++i)
				{
					camInfo[j++] = (unsigned char)bstrCommand[i];
					sprintf_s(camInfoData, "%02X ", camInfo[j-1]);
					msg += camInfoData;
				}

				msg += _T("\n");
				msg += _T("[DM] Info: Get Camera Info Successful\n");
			}
			else
			{
				throw CDeviceException(SSB_DM_ERROR_INVAL_CAMERA_INFO, 0, "Invalid Camera Info Length");
			}
		}
		else
		{
			throw CDeviceException(SSB_DM_ERROR_DEVICE_NOT_CLAIMED, 0, "Unable to process Camera Info Requet");
		}
		// g_cmDc.DoDCPrintf(TRACE_ALWAYS, msg);
	}
    CATCH_DEVICE_EXCEPTION

	//catch(CDeviceException ex)
	//{
	//	//if(ex.GetErrorInfo().nSubSystemError == OPOS_E_EXTENDED || ex.GetErrorInfo().nSubSystemError == OPOS_E_FAILURE)
	//	//{
	//	//	long lexcode = m_devImagerScanner.get_ResultCodeExtended();
	//	//	if(lexcode != 0)
	//	//		ex.SetExtErrorCode(lexcode);
	//	//}
	//	//m_stErrorDetails = ex.GetErrorInfo();
	//	// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("%s"),ex.GetCompleteErrorDesc().c_str());
	//	return false;
	//}
	//catch(...)
	//{
	//	//CSSBException oSSBExp(SSB_SYSTEM_UNKNOWN_EXCEPTION,0,_T("Unhandled Exception in Device Manager GetCameraInfo"));
	//	//m_stErrorDetails = oSSBExp.GetErrorInfo();
	//	// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("%s"),oSSBExp.GetCompleteErrorDesc().c_str());
	//	return false;
	//}
	return bResult;
}

/*
* @brief:  Gets the metadata from the device and retries for metadata if device reports refreshing metadata.
*/

//bool CDeviceConnector::GetDeviceMetaData(CDeviceMetaData& objSWMeta)
//{
//	bool bResult = true;
//	long lResult = OPOS_SUCCESS;
//	BSTR bstrCommand = NULL;
//	BYTE* cDataBuffer = NULL;
//	// g_cmDc.DoDCPrintf(TRACE_OUTPUT_LEVEL1,_T("[DM] Info: Get Device Metadata"));
//	try
//	{
//		bool bRetry = true;
//		int nretry = 0;
//		long getmetadata;
//		if(IsClaimed())
//		{
//			while(bRetry)
//			{
//				getmetadata = SCANWATCH_GET_METADATA;
//				bstrCommand = NULL;
//				// DirectIO call to get the metadata
//				lResult = m_devImagerScanner.DirectIO(NCRDIO_SCANWATCH, &getmetadata, &bstrCommand);
//				if((lResult==OPOS_SUCCESS))
//				{
//					bRetry =false;
//					// check for the metadata size.
//					if(getmetadata > 0)
//					{
//						cDataBuffer = new BYTE[getmetadata];
//						::WideCharToMultiByte(0,0,bstrCommand,getmetadata,(LPSTR)cDataBuffer,getmetadata,0,0);
//						objSWMeta.ParseMetadata(cDataBuffer, getmetadata);
//						delete[] cDataBuffer;
//						SysFreeString(bstrCommand);
//						// g_cmDc.DoDCPrintf(TRACE_OUTPUT_LEVEL1,_T("[DM] Info: Get Device Metadata Successful"));
//					}
//					else
//						throw CDeviceException(SSB_DM_ERROR_METADATAERROR,lResult,_T("Device Manager: Invalid Metadata size"));
//				}
//				else if(lResult == OPOS_E_EXTENDED)
//				{
//					if(bstrCommand)
//						SysFreeString(bstrCommand);
//					// If device sends refreshing metadata retry for 30 times (3 secs).
//					if(m_devImagerScanner.get_ResultCodeExtended() == NCR_EIMGSCAN_REFRESHING_METADATA)
//					{
//						// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Device is busy refreshing metadata, trying again"));
//						Sleep(100);
//						if(nretry > 30)
//						{
//							throw CDeviceException(SSB_DM_ERROR_METADATAERROR,lResult,_T("Device Manager:: Error refreshing Metadata"));
//						}
//						// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Waited for metadata for %d milliseconds"),100*nretry);
//						nretry++;
//					}
//					else
//					{
//						if(bstrCommand)
//							SysFreeString(bstrCommand);
//						throw CDeviceException(SSB_DM_ERROR_METADATAERROR,lResult,_T("Device is not refreshing Metadata"));
//					}
//				}
//				else
//				{
//					throw CDeviceException(SSB_DM_ERROR_METADATAERROR,lResult,_T("Error getting Metadata"));
//				}
//			}
//		}
//		else
//			bResult = false;
//	}
//	catch(CDeviceException ex)
//	{
//		//if(ex.GetErrorInfo().nSubSystemError == OPOS_E_EXTENDED || ex.GetErrorInfo().nSubSystemError == OPOS_E_FAILURE)
//		//{
//		//	long lexcode = m_devImagerScanner.get_ResultCodeExtended();
//		//	if(lexcode != 0)
//		//		ex.SetExtErrorCode(lexcode);
//		//}
//		//m_stErrorDetails = ex.GetErrorInfo();
//		// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("%s"),ex.GetCompleteErrorDesc().c_str());
//		return false;
//	}
//	catch(...)
//	{
//		//CSSBException oSSBExp(SSB_SYSTEM_UNKNOWN_EXCEPTION,0,_T("Unhandled Exception in Device Manager GetDeviceMetaData"));
//		//m_stErrorDetails = oSSBExp.GetErrorInfo();
//		// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("%s"),oSSBExp.GetCompleteErrorDesc().c_str());
//		return false;
//	}
//	return bResult;
//}
/*
* @brief:  Gets the camera info data bytes.
*/
void CDeviceConnector::GetCameraInfoBytes(unsigned char *camInfoOut, int nModel)
{
	memcpy(camInfoOut, this->m_ucCameraInfoData, sizeof(this->m_ucCameraInfoData));
}
/**
* @brief: Initializes the  Scanwatch processing.
*/
bool CDeviceConnector::InitScanWatchProcessing()
{
	// g_cmDc.DoDCPrintf(TRACE_OUTPUT_LEVEL1,_T("[DM] Info: Init Scanwatch Processing"));
	bool bResult = true;
	long lResult = OPOS_SUCCESS;
	CString str_Command;
	try
	{
		BSTR bstrCommand = str_Command.AllocSysString();
		long initCommand = SCANWATCH_INIT;
		long camInfoCommand = SCANWATCH_GET_CAMERAINFO;
		if(IsClaimed())
		{
			// DirectIO call to Initialize the Scanwatch Processing.
			lResult = m_devImagerScanner.DirectIO(NCRDIO_SCANWATCH, &initCommand, &bstrCommand);
			if(lResult != OPOS_SUCCESS)
				throw CDeviceException(SSB_DM_ERROR_SCANWATCHINIT,lResult,"DirectIO command failed");
			// g_cmDc.DoDCPrintf(TRACE_OUTPUT_LEVEL1,_T("[DM] Info: Successfully Initialized Scanwatch Processing"));
		}
		else
			bResult = false;

	}
    CATCH_DEVICE_EXCEPTION

	//catch(CDeviceException ex)
	//{
	//	//if(ex.GetErrorInfo().nSubSystemError == OPOS_E_EXTENDED || ex.GetErrorInfo().nSubSystemError == OPOS_E_FAILURE)
	//	//{
	//	//	long lexcode = m_devImagerScanner.get_ResultCodeExtended();
	//	//	if(lexcode != 0)
	//	//		ex.SetExtErrorCode(lexcode);
	//	//}
	//	//m_stErrorDetails = ex.GetErrorInfo();
	//	// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("%s"),ex.GetCompleteErrorDesc().c_str());
	//	return false;
	//}
	//catch(...)
	//{
	//	//CSSBException oSSBExp(SSB_SYSTEM_UNKNOWN_EXCEPTION,0,_T("Unhandled Exception in Device Manager InitScanWatchProcessing"));
	//	//m_stErrorDetails = oSSBExp.GetErrorInfo();
	//	// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("%s"),oSSBExp.GetCompleteErrorDesc().c_str());
	//	return false;
	//}
	return bResult;
}
/**
* @brief: Resets the Scanwatch processing.
*/
bool CDeviceConnector::ResetScanWatchProcessing()
{
	// g_cmDc.DoDCPrintf(TRACE_OUTPUT_LEVEL1,_T("[DM] Info: Reset Scanwatch Processing"));
	long lResult = OPOS_SUCCESS;
	bool bResult =  true;
	CString str_Command;
	try
	{
		BSTR bstrCommand = str_Command.AllocSysString();
		long initCommand = SCANWATCH_RESET_METADATA_STATUS;
		if(IsClaimed())
		{
			// DirectIO Call to reset the Scanwatch Processing
			lResult = m_devImagerScanner.DirectIO(NCRDIO_SCANWATCH, &initCommand, &bstrCommand);
			if(lResult != OPOS_SUCCESS)
				throw CDeviceException(SSB_DM_ERROR_SCANWATCHRESET,lResult,"SSB_DM_ERROR_SCANWATCHRESET_STR");
			// g_cmDc.DoDCPrintf(TRACE_OUTPUT_LEVEL1,_T("[DM] Info: Reset Scanwatch Processing Success"));
		}
		else
			bResult = false;
	}
    CATCH_DEVICE_EXCEPTION

	//catch(CDeviceException ex)
	//{
	//	//if(ex.GetErrorInfo().nSubSystemError == OPOS_E_EXTENDED || ex.GetErrorInfo().nSubSystemError == OPOS_E_FAILURE)
	//	//{
	//	//	long lexcode = m_devImagerScanner.get_ResultCodeExtended();
	//	//	if(lexcode != 0)
	//	//		ex.SetExtErrorCode(lexcode);
	//	//}
	//	//m_stErrorDetails = ex.GetErrorInfo();
	//	// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("%s"),ex.GetCompleteErrorDesc().c_str());
	//	return false;
	//}
	//catch(...)
	//{
	//	//CSSBException oSSBExp(SSB_SYSTEM_UNKNOWN_EXCEPTION,0,_T("Unhandled Exception in Device Manager ResetScanWatchProcessing"));
	//	//m_stErrorDetails = oSSBExp.GetErrorInfo();
	//	// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("%s"),oSSBExp.GetCompleteErrorDesc().c_str());
	//	return false;
	//}
	return bResult;
}
/**
* @brief Before StartSession Application should set the camera number to
*		 retrive the image from specifed camera number.
*/
bool CDeviceConnector::SetCameraNum(int nCameraNum)
{
	long lResult = OPOS_SUCCESS;
	bool bResult = true;
	// g_cmDc.DoDCPrintf(TRACE_OUTPUT_LEVEL1,_T("[DM] Info: Setting Camera Number"));
	CString str_Command;
	try
	{
		BSTR bstrCommand = str_Command.AllocSysString();
		long initCommand = nCameraNum;
		if(IsClaimed())
		{
			// DirectIO call to set the camera number
			lResult = m_devImagerScanner.DirectIO(NCRDIO_SCANWATCH_SET_CAMERA_NUM, &initCommand, &bstrCommand);
			if(lResult != OPOS_SUCCESS)
				throw CDeviceException(SSB_DM_ERROR_SET_CAMERA_NUMBER,lResult, "DirectIO command failed");
			// g_cmDc.DoDCPrintf(TRACE_OUTPUT_LEVEL1,_T("[DM] Info: Set Camera Number Successful"));
		}
		else
			bResult = false;
	}
    CATCH_DEVICE_EXCEPTION

	//catch(CDeviceException ex)
	//{
	//	//if(ex.GetErrorInfo().nSubSystemError == OPOS_E_EXTENDED || ex.GetErrorInfo().nSubSystemError == OPOS_E_FAILURE)
	//	//{
	//	//	long lexcode = m_devImagerScanner.get_ResultCodeExtended();
	//	//	if(lexcode != 0)
	//	//		ex.SetExtErrorCode(lexcode);
	//	//}
	//	//m_stErrorDetails = ex.GetErrorInfo();
	//	// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("%s"),ex.GetCompleteErrorDesc().c_str());
	//	return false;
	//}
	//catch(...)
	//{
	//	//CSSBException oSSBExp(SSB_SYSTEM_UNKNOWN_EXCEPTION,0,_T("Unhandled Exception in Device Manager SetCameraNumber"));
	//	//m_stErrorDetails = oSSBExp.GetErrorInfo();
	//	// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("%s"),oSSBExp.GetCompleteErrorDesc().c_str());
	//	return false;
	//}
	return bResult;
}
/*
* @brief: Before StartSession Application should set the image type to
*				 retrive the image of specifed type.
*/
bool CDeviceConnector::SetImageType(bool isRefImage)
{
	// g_cmDc.DoDCPrintf(TRACE_OUTPUT_LEVEL1,_T("[DM] Info: Setting Image Type"));
	long lResult = OPOS_E_FAILURE;
	bool bResult = true;
	CString str_Command;
	BSTR bstrCommand = str_Command.AllocSysString();
	long initCommand = isRefImage ? IMG_TYPE_REFERENCE:IMG_TYPE_ITEM;
	try
	{
		if(IsClaimed())
		{
			// DirectIO call to set the image type
			lResult = m_devImagerScanner.DirectIO(NCRDIO_SCANWATCH_SET_IMAGE_TYPE, &initCommand, &bstrCommand);
			if(lResult != OPOS_SUCCESS)
				throw CDeviceException(SSB_DM_ERROR_SETIMAGETYPE,lResult, "DirectIO command failed");
			// g_cmDc.DoDCPrintf(TRACE_OUTPUT_LEVEL1,_T("[DM] Info: Set Image Type Successful"));
		}
		else
			bResult = false;
	}
    CATCH_DEVICE_EXCEPTION

	//catch(CDeviceException ex)
	//{
	//	//if(ex.GetErrorInfo().nSubSystemError == OPOS_E_EXTENDED || ex.GetErrorInfo().nSubSystemError == OPOS_E_FAILURE)
	//	//{
	//	//	long lexcode = m_devImagerScanner.get_ResultCodeExtended();
	//	//	if(lexcode != 0)
	//	//		ex.SetExtErrorCode(lexcode);
	//	//}
	//	//m_stErrorDetails = ex.GetErrorInfo();
	//	// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("%s"),ex.GetCompleteErrorDesc().c_str());
	//	return false;
	//}
	//catch(...)
	//{
	//	//CSSBException oSSBExp(SSB_SYSTEM_UNKNOWN_EXCEPTION,0,_T("Unhandled Exception in Device Manager SetImageType"));
	//	//m_stErrorDetails = oSSBExp.GetErrorInfo();
	//	// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("%s"),oSSBExp.GetCompleteErrorDesc().c_str());
	//	return false;
	//}
	return bResult;
}
/**
* @brief Set Device Online Status
*/
bool CDeviceConnector::SetDeviceOnlineStatus(bool IsOnline)
{
	bool bresult = false;
	if(IsOnline)
	{
		//bresult = RefreshDeviceCaps();
		//if(!bresult)
		//{
		//	 g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Unable to refresh the Device"));
		//	return false;
		//}
		//CDeviceException objDeviceEx(SSB_SUCCESS, 0,_T("Device Online"));
		//m_stErrorDetails = objDeviceEx.GetErrorInfo();
	}
	else
	{
		//CDeviceException objDeviceEx(SSB_DM_ERROR_DEVICE_OFFLINE, 0,_T("Device Offline"));
		//m_stErrorDetails = objDeviceEx.GetErrorInfo();
		// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("%s"),objDeviceEx.GetCompleteErrorDesc().c_str());
	}
	return true; 
}
/**
* @brief Refreshes the device capabilities
*/
bool CDeviceConnector::RefreshDeviceCaps()
{
	// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Refreshing Device Capbilites"));
	try
	{
		if(!m_oDevCaps.CheckUVCDeviceEnabled())
		{
			throw CDeviceException(SSB_DM_ERROR_INITDEVICE,0,"SSB_DM_EEROR_UVC_NOT_ENABLED_STR");
		}
		// Get Camera Info.
		bool isCamInfoOk = GetCameraInfo(m_ucCameraInfoData);
		if (!isCamInfoOk)
		{
			throw CDeviceException(SSB_DM_ERROR_GETCAMERAINFO, 0 , "[DC]Error!!! Failed to get Camera Info from Device");
		}
		bool  breturn  = m_oDevCaps.SetDevCapsfromCamInfo(m_ucCameraInfoData);
		if(breturn)
		{
			if(!m_oDevCaps.IsModelSupported())
			{
				throw CDeviceException(SSB_DM_ERROR_MODEL_NOTSUPPORTED, 0 , "");
			}
			if(!m_oDevCaps.IsScaleOnline())
			{
				throw CDeviceException(SSB_DM_ERROR_SCALE_OFFLINE, 0, "");
			}
			if(!m_oDevCaps.IsFirmwareSupported())
			{
				throw CDeviceException(SSB_DM_ERROR_FW_NOTSUPPORTED, 0, "");
			}
			if(!m_oDevCaps.IsScanwatchEnabled())
			{
                ASSERT(0);
                //throw CSSBException(SSB_DM_ERROR_FW_NOTSUPPORTED, 0, _T(""));
                //throw CSSBException(SSB_DM_ERROR_FW_NOTSUPPORTED, 0, _T(""));
			}
		}
		LPCTSTR ltSuccess = _T("Successfully refreshed Image Scanner");
		int iRet = ReportEvent(m_hEventLog, EVENTLOG_SUCCESS, 0, SSB_SUCCESS, NULL, 1, 0,&ltSuccess, NULL);
		if(iRet == 0)
		{
			int iError = GetLastError();
			//CString messageBuffer = _T("");
			//FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			//	NULL, iError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
			CString csError = _T("");
			csError.Format(_T("Report Event failed with Error code %i"),iError);
			// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("[DM] Info: %s"),csError);
		}
	}
    CATCH_DEVICE_EXCEPTION

	//catch(CDeviceException ex)
	//{
	//	//if(ex.GetErrorInfo().nSubSystemError == OPOS_E_EXTENDED || ex.GetErrorInfo().nSubSystemError == OPOS_E_FAILURE)
	//	//{
	//	//	long lexcode = m_devImagerScanner.get_ResultCodeExtended();
	//	//	if(lexcode != 0)
	//	//		ex.SetExtErrorCode(lexcode);
	//	//}
	//	//m_stErrorDetails = ex.GetErrorInfo();
	//	// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("%s"),ex.GetCompleteErrorDesc().c_str());
	//	//LPCTSTR ltSuccess = ex.GetCompleteErrorDesc().c_str();
	//	//int iRet = ReportEvent(m_hEventLog, EVENTLOG_ERROR_TYPE, 0, SSB_SUCCESS, NULL, 1, 0,&ltSuccess, NULL);
	//	return false;
	//}
	//catch(...)
	//{
	//	//CSSBException oSSBExp(SSB_SYSTEM_UNKNOWN_EXCEPTION,0,_T("Unhandled Exception in Device Manager InitScanWatchProcessing"));
	//	//m_stErrorDetails = oSSBExp.GetErrorInfo();
	//	//// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("%s"),oSSBExp.GetCompleteErrorDesc().c_str());
	//	//LPCTSTR ltSuccess = oSSBExp.GetCompleteErrorDesc().c_str();
	//	//int iRet = ReportEvent(m_hEventLog, EVENTLOG_ERROR_TYPE, 0, SSB_SUCCESS, NULL, 1, 0,&ltSuccess, NULL);
	//	return false;
	//}
	return true;

}