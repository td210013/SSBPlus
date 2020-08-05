
#ifdef _WIN32
#include "stdafx.h"
//#include "uuids.h"
//#include "strmif.h"
#endif
#include "DeviceCapabilities.h"

#define COLOR_HOR 0x01
#define COLOR_VER 0x02
#define COLOR_TD  0x04
#define COLOR_LT  0x08
#define MON_HOR   0x10
#define MON_VER	  0x20
#define MON_TD    0x40
#define MON_LT	  0x80

//extern CmDataCapture g_cmDc;

#define SafeRelease(x){ if(x)  { (x)->Release(); x = NULL;} }
#define SUPPORTED_FW "0518191"
CDeviceCapabilities::CDeviceCapabilities(void)
{
	m_csFirmwareVersion = "N/A";
	m_csModel = "N/A";
	m_csSerialNumber = "N/A";
	m_bScanwatchEnable = false;
	m_bScaleOnline = false;
	m_bScannerModelSupported = false;
	m_bPLASupport = false;
	m_bPASupport = false;
	m_bFirmwareSupported = false;
	m_bUVCEnabled = false;
}

CDeviceCapabilities::~CDeviceCapabilities(void)
{
}

bool CDeviceCapabilities::SetDevCapsfromCamInfo(unsigned char* cameraInfo)
{
	CameraSetup cameraSetup;

	if(cameraInfo != NULL)
	{
		// The first few bytes represent the CameraSetup struct.
		memcpy(&cameraSetup, cameraInfo, sizeof(cameraSetup));
	}
	else
	{
		// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Invalid Camera Info"));
		return false;
	}

	m_csFirmwareVersion.assign((char*)cameraSetup.firmware_version);
	if(m_csFirmwareVersion.length() > 0)
	{
		// g_cmDc.DoDCPrintf(TRACE_ALWAYS, _T("Scanner Firmware Version : %s"), m_csFirmwareVersion.c_str());
		if(m_csFirmwareVersion.compare(0,4,"497-") == 0)
		{
			std::string csFirmware = m_csFirmwareVersion.substr(5,m_csFirmwareVersion.length());
			int nFirmware = atoi(csFirmware.c_str());
			int nsuppFirm = atoi(SUPPORTED_FW);
			if(nFirmware >= nsuppFirm)
			{
				m_bFirmwareSupported = true;
			}
		}
		else if(m_csFirmwareVersion.compare(0,4,"000-") == 0)
		{
			// g_cmDc.DoDCPrintf(TRACE_ALWAYS, _T("Scanner Firmware Version : %s, developer version"), m_csFirmwareVersion.c_str());
			m_bFirmwareSupported = true;
		}
		else
		{
			// g_cmDc.DoDCPrintf(TRACE_ALWAYS, _T("Unable to get the firmware version from camera info"));
			m_bFirmwareSupported = false;
			return false;
		}
	}
	else
	{
		// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Invalid Firmware Version/ unable to determine the FW version"));
		return false;
	}
	
	
	m_csModel.assign((char*)cameraSetup.class_model);
	if(m_csModel.length() > 0)
	{
		// g_cmDc.DoDCPrintf(TRACE_ALWAYS, _T("Scanner Model : %s"), m_csModel.c_str());
		if(m_csModel.compare(0,4,"7879") != 0)
		{
			m_bScannerModelSupported = false;
		}
		else
		{
			m_bScannerModelSupported = true;
		}
	}
	else
	{
		m_bScannerModelSupported = false;
	}
	
	m_bScaleOnline = (cameraSetup.scale_status == 0x00)? true:false;
	m_bScanwatchEnable = (cameraSetup.scanwatch_status == 0x01)? true:false;
	
	m_csSerialNumber.assign((char*) cameraSetup.serial_number);
	if(int(cameraSetup.num_cameras) < 4)
	{
		// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Invalid number of Camera's "));
		return false;
	}
	byte cameraValue = 0x00;
	for(int i=0; i < 4; i++)
	{
		if (strcmp((const char*)cameraSetup.cam_params[i].sensor_type, "Invalid") == 0)
		{
			// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Invalid Camera Driver "));
			return false;
		}
		else
		{
			// g_cmDc.DoDCPrintf(TRACE_ALWAYS, _T("Scanner Camera ID: %d, Sensor Type: %s"), i, (const char*)cameraSetup.cam_params[i].sensor_type);
			bool isColor = false;
			if ((strcmp((const char*)cameraSetup.cam_params[i].sensor_type, "AR0135Color") == 0)||
				(strcmp((const char*)cameraSetup.cam_params[i].sensor_type, "AR0134Color") == 0))
			{
				isColor  = true;
			}
			else if((strcmp((const char*)cameraSetup.cam_params[i].sensor_type, "AR0135Mono") == 0)||
					(strcmp((const char*)cameraSetup.cam_params[i].sensor_type, "AR0134Mono") == 0)||
					(strcmp((const char*)cameraSetup.cam_params[i].sensor_type, "M021") == 0))
			{
				isColor  = false;
			}
			else
			{
				// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("UnKnown Camera Driver "));
				return false;
			}
			switch(i)
			{
				case VERTICAL_CAMERA:
					cameraValue |= isColor?COLOR_VER:MON_VER;
					break;
				case HORIZONTAL_CAMERA:
					cameraValue |= isColor?COLOR_HOR:MON_HOR;
					break;
				case LEADTRAIL_CAMERA:
					cameraValue |= isColor?COLOR_LT:MON_LT;
					break;
				case TOPDOWN_CAMERA:
					cameraValue |= isColor?COLOR_TD:MON_TD;
					break;
				default:
					break;
			}
		}
	}
	if(cameraValue == 0xf0)
	{
		m_bPASupport = true;
		m_bPLASupport = false;
	}
	else if(cameraValue == 0xC3)
	{
		m_bPLASupport = true;
		m_bPASupport = true;
	}
	else
	{
		m_bPLASupport = false;
		m_bPASupport = false;
	}
	// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Scanwatch Enable : %s"), IsScanwatchEnabled()?_T("true"):_T("false"));
	// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Model Supported : %s"), IsModelSupported()?_T("true"):_T("false"));
	// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Firmware Supported : %s"), IsFirmwareSupported()?_T("true"):_T("false"));
	// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Scale Online : %s"), IsScaleOnline()?_T("true"):_T("false"));
	// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("PA Supported : %s"), IsPASupported()?_T("true"):_T("false"));
	// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("PLA Supported : %s"), IsPLASupported()?_T("true"):_T("false"));
	return true;
}
BOOL CDeviceCapabilities::CheckUVCDeviceEnabled()
{
	// to avoid the DirectShow calls the below code is commented, and this will replaced by the USB enumeration in the future release.  
//#ifdef _WIN32
//   ::CoInitialize( NULL );
//   
//   ICreateDevEnum *pCreateDevEnum = NULL;
//   IEnumMoniker  *pEm = NULL;
//   IMoniker *pM = NULL;  
//   IBaseFilter* pSourceFilter = NULL;
//   BOOL bError = FALSE;
//   BOOL bdevicefound = FALSE;
//   HRESULT  hr = E_FAIL;
//
//   // Create a system device enumerator.  
//   hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,IID_ICreateDevEnum, (void **) & pCreateDevEnum);
//   if(FAILED(hr))
//   {
//	   // g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Creating system device enumerator - Failed.hr = 0x%x"), hr);	   
//	   bError = TRUE;
//   }
//
//    // Get video capture devices enumurator. 
//    if(!bError)
//	{
//	   hr = pCreateDevEnum->CreateClassEnumerator( CLSID_VideoInputDeviceCategory, &pEm, 0 );
//	   if( hr != S_OK)
//	   {			
//		   // g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Creating video capture enumerator - Failed.hr = 0x%x"), hr);
//		   bError = TRUE;
//	   }
//	   else
//	   {
//			pEm->Reset( );
//	   }
//	}
//
//    if(!bError)
//	{		
//		// Enumerate through the list and get the video capture device based on VID and PID information.
//		while( pEm->Next( 1, &pM, NULL ) == S_OK )
//		{
//			// Get the property bag interface from the moniker.
//			IPropertyBag * pBag = NULL;
//			hr = pM->BindToStorage( 0, 0, IID_IPropertyBag, (void**) &pBag );
//			if(FAILED(hr))
//			{
//				SafeRelease(pM);
//				continue;
//			}
//
//			//Read the DevicePath from the property bag.
//       		VARIANT varName;
//			VariantInit(&varName);
//			hr = pBag->Read( L"DevicePath", &varName, NULL );
//			if( FAILED(hr) )
//			{
//				SafeRelease(pBag);
//				SafeRelease(pM);           
//				continue;
//			}			
//			
//			CString strDevicepath(_com_util::ConvertBSTRToString(varName.bstrVal));
//							
//			// If this is one of our devices, go and bind to it to get the
//			// base filter interface.       
//			if(IsMatchingDeviceFound(strDevicepath))
//			{            
//				hr = pM->BindToObject( 0, 0, IID_IBaseFilter, (void**)&pSourceFilter);				
//				if(hr == S_OK)
//				{
//					bdevicefound = TRUE;
//				}
//				else
//				{
//					// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Video capture device found,but getting source filter failed."));				
//				}
//			}  
//			// com objects cleanup.
//			SafeRelease(pM);
//			SafeRelease(pBag);
//			VariantClear(&varName);
//			if(bdevicefound)
//				break;
//		}
//	}
//	//Cleanup
//	SafeRelease(pEm);
//	SafeRelease(pCreateDevEnum);
//	SafeRelease(pSourceFilter);	
	m_bUVCEnabled = true;
//	// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Image Scanner Devicefound = %s"), bdevicefound?_T("true"):_T("false"));         	
//	::CoUninitialize();
//    return bdevicefound;
//#else
	return true;
//#endif
}
//BOOL CDeviceCapabilities::IsMatchingDeviceFound(CString& strDevicePath)
//{
//	
//	//sample device path format - \\?\usb#vid_046d&pid_081b&mi_00#7&2971a4b4&1&0000#{65e8773d-8f56-11d0-a3b9-00a0c9223196}\{bbefb6c7-2fc4-4139-bb8b-a58bba724083}
//	BOOL bRet = FALSE;
//	CString strCnfgVid,strCnfgPid;
//	strCnfgVid.Format(_T("%04hx"),m_nVid);
//	strCnfgPid.Format(_T("%04hx"),m_nPid);	
//
//	// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Devicepath = %s"),strDevicePath.GetString()); 
//	// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Configured VID_PID = %s _ %s"),strCnfgVid.GetString(),strCnfgPid.GetString());
//
//	//Get Vid form device path 
//	UINT32 indexStart = strDevicePath.Find(_T('_'));
//	indexStart+=1;//Starting from '_'
//	UINT32 indexCount = 4;//vid 4 chars.
//	CString strDevVid,strDevPid;
//	strDevVid = strDevicePath.Mid(indexStart,indexCount);
//
//	//Get Pid form device path
//	indexStart = strDevicePath.Find(_T('_'),indexStart+indexCount);
//	indexStart+=1;
//	indexCount = 4;//pid 4 chars.
//	strDevPid = strDevicePath.Mid(indexStart,indexCount);			
//
//	// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("Device VID_PID = %s _ %s"),strDevVid.GetString(),strDevPid.GetString());
//
//	//Compare configured VID and PID with device path VID and PID      
//	if(strDevVid.CompareNoCase(strCnfgVid)==0 && strDevPid.CompareNoCase(strCnfgPid)==0)
//	{
//		bRet = TRUE;
//	}
//
//	// g_cmDc.DoDCPrintf(TRACE_ALWAYS,_T("bRet = %b"),bRet);      	
//
//	return bRet;
//}