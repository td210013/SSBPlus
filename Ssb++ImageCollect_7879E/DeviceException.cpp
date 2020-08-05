#include "stdafx.h"
#include "DeviceException.h"
#include "NCRImg.h"
#include "SSBErrors.h"
//CDeviceException::CDeviceException()
//{
//}
// ********************************************************************************
///
/// Function :  CDeviceException (DWORD dwErrCode,CString sErrText)
///
/// Purpose : Constructor
///
/// Parameters: dwErrCode, LibErrorCode,  sErrText - Additional information
///
/// Returns:
///		\return nothing
///
// ********************************************************************************
CDeviceException::CDeviceException(DWORD dwErrCode, DWORD dwLibErrCode, std::string sErrText) : // : CSSBException(dwErrCode,dwLibErrCode,sErrText)
    m_dwErrorCode(dwErrCode),
    m_dwLibErrorCode(dwLibErrCode),
    m_sErrorText(sErrText)
{
}

CDeviceException::~CDeviceException(void)
{
}

//*********************************************************************************
///
///Function: LoadErrorInfo
///
///Purpose: Loads Error Information from SSB Sub-systems into variables
///
///Parameters: None
///
/// Returns:
///		\return nothing
///
//*********************************************************************************
/*
void CDeviceException::LoadErrorInfo()
{
	if(m_stErrorInfo.nErrorCode == 0)
	{
		m_stErrorInfo.enErrorSatus		= ONLINE;
		m_stErrorInfo.enErroLevel	    = SSB_INFO;
		m_stErrorInfo.nSubSystemError = 0;
		return;
	}
	switch(m_stErrorInfo.nErrorCode)
	{
	case SSB_DM_ERROR_UNABLE_TO_CREATE:
		m_stErrorInfo.enErrorSatus		= OFFLINE;
		m_stErrorInfo.enErroLevel	    = SSB_ERR;
		break;
	case SSB_DM_ERROR_INITDEVICE:
		m_stErrorInfo.enErrorSatus		= OFFLINE;
		m_stErrorInfo.enErroLevel	    = SSB_ERR;
		break;
	case SSB_DM_ERROR_DATA_EVENT:
		m_stErrorInfo.enErrorSatus		= ONLINE;
		m_stErrorInfo.enErroLevel	    = SSB_ERR;
		break;
	case SSB_DM_ERROR_DEVICE_NOT_CLAIMED:
		m_stErrorInfo.enErrorSatus		= OFFLINE;
		m_stErrorInfo.enErroLevel	    = SSB_ERR;
		break;
	case SSB_DM_ERROR_STARTSESSION:
		m_stErrorInfo.enErrorSatus		= OFFLINE;
		m_stErrorInfo.enErroLevel	    = SSB_ERR;
		break;
	case SSB_DM_ERROR_STOPSESSION:
		m_stErrorInfo.enErrorSatus		= OFFLINE;
		m_stErrorInfo.enErroLevel	    = SSB_ERR;
		break;
	case SSB_DM_ERROR_RELEASEDEVICE:
		m_stErrorInfo.enErrorSatus		= OFFLINE;
		m_stErrorInfo.enErroLevel	    = SSB_ERR;
		break;
	case SSB_DM_ERROR_DESTROYWINDOW:
		m_stErrorInfo.enErrorSatus		= OFFLINE;
		m_stErrorInfo.enErroLevel	    = SSB_ERR;
		break;
	case SSB_DM_ERROR_CLOSED:
		m_stErrorInfo.enErrorSatus		= OFFLINE;
		m_stErrorInfo.enErroLevel	    = SSB_ERR;
		break;
	case SSB_DM_ERROR_METADATAERROR:
		m_stErrorInfo.enErrorSatus		= ONLINE;
		m_stErrorInfo.enErroLevel	    = SSB_ERR;
		break;
	case SSB_DM_ERROR_SCANWATCHINIT:
		m_stErrorInfo.enErrorSatus		= OFFLINE;
		m_stErrorInfo.enErroLevel	    = SSB_ERR;
		break;
	case SSB_DM_ERROR_GETTINGIMAGE:
		m_stErrorInfo.enErrorSatus		= ONLINE;
		m_stErrorInfo.enErroLevel	    = SSB_ERR;
		break;
	case SSB_DM_ERROR_SET_CAMERA_NUMBER:
		m_stErrorInfo.enErrorSatus		= OFFLINE;
		m_stErrorInfo.enErroLevel	    = SSB_ERR;
		break;
	case SSB_DM_ERROR_SETIMAGETYPE:
		m_stErrorInfo.enErrorSatus		= OFFLINE;
		m_stErrorInfo.enErroLevel	    = SSB_ERR;
		break;
	case SSB_DM_ERROR_SCANWATCHRESET:
		m_stErrorInfo.enErrorSatus		= OFFLINE;
		m_stErrorInfo.enErroLevel	    = SSB_ERR;
		break;
	case SSB_DM_ERROR_GETCAMERAINFO:
		m_stErrorInfo.enErrorSatus		= OFFLINE;
		m_stErrorInfo.enErroLevel	    = SSB_ERR;
		break;
	case SSB_DM_ERROR_UVC_DISABLED:
		m_stErrorInfo.enErrorSatus		= OFFLINE;
		m_stErrorInfo.enErroLevel	    = SSB_ERR;
		break;
	case SSB_DM_ERROR_MODEL_NOTSUPPORTED:
		m_stErrorInfo.enErrorSatus		= OFFLINE;
		m_stErrorInfo.enErroLevel	    = SSB_ERR;
		break;
	case SSB_DM_ERROR_SCALE_OFFLINE:
		m_stErrorInfo.enErrorSatus		= OFFLINE;
		m_stErrorInfo.enErroLevel	    = SSB_ERR;
		break;
	case SSB_DM_ERROR_FW_NOTSUPPORTED:
		m_stErrorInfo.enErrorSatus		= OFFLINE;
		m_stErrorInfo.enErroLevel	    = SSB_ERR;
		break;
	case SSB_DM_ERROR_SCANWATCH_DISABLED:
		m_stErrorInfo.enErrorSatus		= OFFLINE;
		m_stErrorInfo.enErroLevel	    = SSB_ERR;
		break;
	case SSB_DM_ERROR_INVAL_CAMERA_INFO:
		m_stErrorInfo.enErrorSatus		= OFFLINE;
		m_stErrorInfo.enErroLevel	    = SSB_ERR;
		break;	
	case SSB_DM_ERROR_REFRESH_DEVICE:
		m_stErrorInfo.enErrorSatus		= OFFLINE;
		m_stErrorInfo.enErroLevel	    = SSB_ERR;
		break;
	case SSB_DM_ERROR_DEVICE_OFFLINE:
		m_stErrorInfo.enErrorSatus		= OFFLINE;
		m_stErrorInfo.enErroLevel	    = SSB_ERR;
		break;
	default:
		CSSBException::LoadErrorInfo();
	}

	if(m_stErrorInfo.nSubSystemError != 0)
	{
		switch(m_stErrorInfo.nSubSystemError)
		{
		case OPOS_E_BUSY:
			m_stErrorInfo.enErrorSatus		= ONLINE;
			m_stErrorInfo.enErroLevel	    = SSB_ERR;
			break;
		case OPOS_E_TIMEOUT:
			m_stErrorInfo.enErrorSatus		= ONLINE;
			m_stErrorInfo.enErroLevel	    = SSB_ERR;
			break;
		case OPOS_E_FAILURE:
			if(m_stErrorInfo.nErrorCode != SSB_DM_ERROR_METADATAERROR)
			{
				m_stErrorInfo.enErrorSatus		= OFFLINE;
				m_stErrorInfo.enErroLevel	    = SSB_ERR;
			}
			else
			{
				m_stErrorInfo.enErrorSatus		= ONLINE;
				m_stErrorInfo.enErroLevel	    = SSB_ERR;
			}
			break;
		case OPOS_E_EXISTS:
			m_stErrorInfo.enErrorSatus		= OFFLINE;
			m_stErrorInfo.enErroLevel	    = SSB_ERR;
			break;
		case OPOS_E_NOEXIST:
			m_stErrorInfo.enErrorSatus		= OFFLINE;
			m_stErrorInfo.enErroLevel	    = SSB_ERR;
			break;
		case OPOS_E_OFFLINE:
			m_stErrorInfo.enErrorSatus		= OFFLINE;
			m_stErrorInfo.enErroLevel	    = SSB_ERR;
			break;
		case OPOS_E_NOHARDWARE:
			m_stErrorInfo.enErrorSatus		= OFFLINE;
			m_stErrorInfo.enErroLevel	    = SSB_ERR;
			break;
		case OPOS_E_ILLEGAL:
			m_stErrorInfo.enErrorSatus		= OFFLINE;
			m_stErrorInfo.enErroLevel	    = SSB_ERR;
			break;
		case OPOS_E_DISABLED:
			m_stErrorInfo.enErrorSatus		= OFFLINE;
			m_stErrorInfo.enErroLevel	    = SSB_ERR;
			break;
		case OPOS_E_NOSERVICE:
			m_stErrorInfo.enErrorSatus		= OFFLINE;
			m_stErrorInfo.enErroLevel	    = SSB_ERR;
			break;
		case OPOS_E_NOTCLAIMED:
			m_stErrorInfo.enErrorSatus		= OFFLINE;
			m_stErrorInfo.enErroLevel	    = SSB_ERR;
			break;
		case OPOS_E_CLAIMED:
			m_stErrorInfo.enErrorSatus		= OFFLINE;
			m_stErrorInfo.enErroLevel	    = SSB_ERR;
			break;
		case OPOS_E_CLOSED:
			m_stErrorInfo.enErrorSatus		= OFFLINE;
			m_stErrorInfo.enErroLevel	    = SSB_ERR;
			break;
		case OPOS_E_EXTENDED:

		default:
			CSSBException::LoadErrorInfo();
		}
	}
}
*/

// ********************************************************************************
///
/// Function :  SetExtErrorCode (long lexRetcode)
///
/// Purpose : Get the information about extented error
///
/// Parameters:  lexRetcode - extended error code
///
/// Returns:
///		\return nothing
///
// ********************************************************************************
/*
void CDeviceException::SetExtErrorCode(long lexRetcode)
{

 //  m_stErrorInfo.nSubSystemError = lexRetcode;
 //  switch(lexRetcode)
 //  {
 //  case NCR_EIMGSCAN_INVALID_CONTROL_ID:
	//   	m_stErrorInfo.enErrorSatus		= OFFLINE;
	//	m_stErrorInfo.enErroLevel	    = SSB_ERR;
	//	break;
	//case NCR_EIMGSCAN_METADATA_NOT_AVAILABLE:
	//   	m_stErrorInfo.enErrorSatus		= ONLINE;
	//	m_stErrorInfo.enErroLevel	    = SSB_ERR;
	//	break;
	//case NCR_EIMGSCAN_REFRESHING_METADATA:
	//   	m_stErrorInfo.enErrorSatus		= ONLINE;
	//	m_stErrorInfo.enErroLevel	    = SSB_ERR;
	//	break;
	//default:
	//	m_stErrorInfo.enErrorSatus		= ONLINE;
	//	m_stErrorInfo.enErroLevel		= SSB_ERR;
 //  }
}
*/
/*
std::string  CDeviceException::GetErrorDesc(DWORD dwErrCode)
{
  // std::string csErrorText = "";
  switch(dwErrCode)
  {
  case SSB_DM_ERROR_UNABLE_TO_CREATE:
    csErrorText.assign(SSB_DM_ERROR_UNABLE_TO_CREATE_STR);
    break;
  case SSB_DM_ERROR_INITDEVICE:
    csErrorText.assign(SSB_DM_ERROR_INITDEVICE_STR);
    break;
  case SSB_DM_ERROR_DATA_EVENT:
    csErrorText.assign(SSB_DM_ERROR_DATA_EVENT_STR);
    break;
  case SSB_DM_ERROR_DEVICE_NOT_CLAIMED:
    csErrorText.assign(SSB_DM_ERROR_UNABLE_TO_CLAIM_STR);
    break;
  case SSB_DM_ERROR_STARTSESSION:
    csErrorText.assign(SSB_DM_ERROR_STARTSESSION_STR);
    break;
  case SSB_DM_ERROR_STOPSESSION:
    csErrorText.assign(SSB_DM_ERROR_STOPSESSION_STR);
    break;
  case SSB_DM_ERROR_RELEASEDEVICE:
    csErrorText.assign(SSB_DM_ERROR_RELEASEDEVICE_STR);
    break;
  case SSB_DM_ERROR_DESTROYWINDOW:
    csErrorText.assign(SSB_DM_ERROR_DESTROYWINDOW_STR);
    break;
  case SSB_DM_ERROR_CLOSED:
    csErrorText.assign(SSB_DM_ERROR_CLOSED_STR);
    break;
  case SSB_DM_ERROR_METADATAERROR:
    csErrorText.assign(SSB_DM_ERROR_METADATAERROR_STR);
    break;
  case SSB_DM_ERROR_SCANWATCHINIT:
    csErrorText.assign(SSB_DM_ERROR_SCANWATCHINIT_STR);
	break;
  case SSB_DM_ERROR_GETTINGIMAGE:
	  csErrorText.assign(SSB_DM_ERROR_GETTINGIMAGE_STR);
	  break;
  case SSB_DM_ERROR_SET_CAMERA_NUMBER:
	  csErrorText.assign(SSB_DM_ERROR_SET_CAMERA_NUMBER_STR);
	  break;
  case SSB_DM_ERROR_SETIMAGETYPE:
	  csErrorText.assign(SSB_DM_ERROR_SETIMAGETYPE_STR);
    break;
  case SSB_DM_ERROR_SCANWATCHRESET:
    csErrorText.assign(SSB_DM_ERROR_SCANWATCHRESET_STR);
    break;
  case SSB_DM_ERROR_GETCAMERAINFO:
    csErrorText.assign(SSB_DM_ERROR_GETCAMERAINFO_STR);
    break;
  case SSB_DM_ERROR_UVC_DISABLED:
    csErrorText.assign(SSB_DM_EEROR_UVC_NOT_ENABLED_STR);
    break;
  case SSB_DM_ERROR_MODEL_NOTSUPPORTED:
    csErrorText.assign(SSB_DM_ERROR_MODEL_NOTSUPPORTED_STR);
    break;
  case SSB_DM_ERROR_SCALE_OFFLINE:
    csErrorText.assign(SSB_DM_ERROR_SCALE_OFFLINE_STR);
    break;
  case SSB_DM_ERROR_FW_NOTSUPPORTED:
    csErrorText.assign(SSB_DM_ERROR_FW_NOTSUPPORTED_STR);
    break;
  case SSB_DM_ERROR_SCANWATCH_DISABLED:
	csErrorText.assign(SSB_DM_ERROR_SCANWATCH_DISABLED_STR);
    break;
  case SSB_DM_ERROR_INVAL_CAMERA_INFO:
	csErrorText.assign(SSB_DM_ERROR_INVAL_CAMERA_INFO_STR);
    break;
  case SSB_DM_ERROR_REFRESH_DEVICE:
	csErrorText.assign(SSB_DM_ERROR_REFRESH_DEVICE_STR);
    break;
  case SSB_DM_ERROR_DEVICE_OFFLINE:
	  csErrorText.assign(SSB_DM_ERROR_DEVICE_OFFLINE_STR);
    break;
  default:
	  csErrorText = CSSBException::GetErrorDesc(dwErrCode);
  }
  return csErrorText;
  return m_sErrorText;
}
*/

// ********************************************************************************
///
/// Function :  GetSubSysErrorDesc(long lRetCode)
///
/// Purpose : Get the error string about the error
///
/// Parameters:  lRetCode - error code
///
/// Returns:
///		\return error string
///
// ********************************************************************************
std::string CDeviceException::GetSubSysErrorDesc(DWORD lRetCode)
{

  CString csErrorText = _T("");
  if(lRetCode == OPOS_E_BUSY)
  {
      // csErrorText.LoadString(IDS_OPOS_E_BUSY);
      csErrorText = "OPOS_E_BUSY";
  }
  else if(lRetCode == OPOS_E_TIMEOUT)
  {
    // csErrorText.LoadString(IDS_OPOS_E_TIMEOUT);
    csErrorText = "OPOS_E_TIMEOUT";
  }
  else if(lRetCode == OPOS_E_FAILURE)
  {
    //csErrorText.LoadString(IDS_OPOS_E_FAILURE);
    csErrorText = "OPOS_E_FAILURE";
  }
  else if(lRetCode == OPOS_E_EXISTS)
  {
    //csErrorText.LoadString(IDS_OPOS_E_EXISTS);
    csErrorText = "OPOS_E_EXISTS";
  }
  else if(lRetCode == OPOS_E_NOEXIST)
  {
    //csErrorText.LoadString(IDS_OPOS_E_NOEXIST);
    csErrorText = "OPOS_E_NOEXIST";
  }
  else if(lRetCode == OPOS_E_OFFLINE)
  {
    //csErrorText.LoadString(IDS_OPOS_E_OFFLINE);
    csErrorText = "OPOS_E_OFFLINE";
  }
  else if(lRetCode == OPOS_E_NOHARDWARE)
  {
    //csErrorText.LoadString(IDS_OPOS_E_NOHARDWARE);
    csErrorText = "OPOS_E_NOHARDWARE";
  }
  else if(lRetCode == OPOS_E_ILLEGAL)
  {
    //csErrorText.LoadString(IDS_OPOS_E_ILLEGAL);
    csErrorText = "OPOS_E_ILLEGAL";
  }
  else if(lRetCode == OPOS_E_DISABLED)
  {
    //csErrorText.LoadString(IDS_OPOS_E_DISABLED);
    csErrorText = "OPOS_E_DISABLED";
  }
  else if(lRetCode == OPOS_E_NOSERVICE)
  {
    //csErrorText.LoadString(IDS_OPOS_E_NOSERVICE);
    csErrorText = "OPOS_E_NOSERVICE";
  }
  else if(lRetCode == OPOS_E_NOTCLAIMED)
  {
    //csErrorText.LoadString(IDS_OPOS_E_NOTCLAIMED);
    csErrorText = "OPOS_E_NOTCLAIMED";
  }
  else if(lRetCode == OPOS_E_CLAIMED)
  {
    //csErrorText.LoadString(IDS_OPOS_E_CLAIMED);
    csErrorText = "OPOS_E_CLAIMED";
  }
  else if(lRetCode == OPOS_E_CLOSED)
  {
    //csErrorText.LoadString(IDS_OPOS_E_CLOSED);
    csErrorText = "OPOS_E_CLOSED";
  }
  else if(lRetCode == NCR_EIMGSCAN_INVALID_CONTROL_ID)
  {
    //csErrorText.LoadString(IDS_OPOS_EX_INVALID_CONTROL);
    csErrorText = "NCR_EIMGSCAN_INVALID_CONTROL_ID";
  }
  else if(lRetCode == NCR_EIMGSCAN_METADATA_NOT_AVAILABLE)
  {
    //csErrorText.LoadString(IDS_OPOS_EX_METADATA_NOT_AVAILABLE);
    csErrorText = "NCR_EIMGSCAN_METADATA_NOT_AVAILABLE";
  }
  else if(lRetCode == NCR_EIMGSCAN_REFRESHING_METADATA)
  {
    //csErrorText.LoadString(IDS_OPOS_REFF_META_DATA);
    csErrorText = "OPOS_E_BUSY";
  }
  else
  {
    csErrorText = "NA";
  }

  return (char*)csErrorText.GetString();
}
