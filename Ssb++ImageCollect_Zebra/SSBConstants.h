#pragma once

//#ifndef _WIN32
//#include "linux/LinDef.h"
//#endif
// #include <vector>

#define SSB_ITEM_UPC_SIZE 30
#define SSB_ITEM_DESC_SIZE 100
#define SSB_ERROR_DESC_SIZE 255 

/* 
*	enum:	SSBSTATUS - Status of the SSB System
*/
typedef enum
{
	UNINITIALIZED,	/**< When the SSB is not Inititilaized */
	ONLINE,			/**< SSB is ONLINE and no error */
	OFFLINE			/**< SSB is initialized and in offline due to error */
} SSBSTATUS;

/* 
*	enum:	SSBERRORLEVEL - SSB Error Level
*/
typedef enum
{
	SSB_INFO, /**< only provides the information no action required  */
	SSB_WARN,	 /**< Provides the error but not affect the flow of the SSB. */
	SSB_ERR		 /**< The Error requires the attention. */
}SSBERRORLEVEL;

/* 
*	@brief	enum:	ENSSBERRORMODULE - SSB Error State
*/
typedef enum
{
	HEALTHY = 0x00,     /**< HEALTHY - SSB Is HEALTHY. */
	DEVICEERROR=0x01,	/**< Image Scanner Error/ Device Connector Error.*/ 
	PLAERROR=0x02,		/**< Picklist Library Error/ PLA Connector Error. */
	PAERROR=0x04,		/**< Produce Assurance Error/ PA Connector Error. */
	SYSTEMERROR=0x08	/**< SSB Generic Error. */
}SSBERRORMODULE;

/* 
*	@brief	enum:	SSBRESSTATUS - Respone for the SSB Request contains this Status
*/
typedef enum
{
  SUCCESS, /**< SSB Request Successfully processed and no Alerts.*/
  FAILED,  /**< SSB Failed to Process the Request.*/
  CENTER,  /**< When the Item is not placed in the center.*/
  ALERT, /**< Suspicious Item is found.*/
  NOALERT, /** Supressing the Alert*/
  DISABLED /**< When the functionality is disabled */
} SSBRESSTATUS;

/* 
*	@brief	enum:	SSBSOURCE - SSB Response contains this Source to define result from specified source
*/
typedef enum
{
	PA,
	PLA
} SSBSOURCE;
/* 
*	@brief	enum:	SSBFLOW - determines the flow of the request
*/
typedef enum
{
	PICKLIST, /**< Picklist flow */
	VALIDATION  /**< Validation flow*/
} SSBFLOW;

/* 
*	@brief	enum:	SSBMODULE - Specifies the subsystems in SSB System
*/
typedef enum
{
	SSBMOD_DEVICE = 0x01,		/**< Image scanner Device */
	SSBMOD_PA = 0x02,			/**< Produce Assurance Libaray */
	SSBMOD_PLA = 0x04			/**< Picklist Assit Libary */ 
} SSBMODULE;
/* 
*	@brief	enum:	SSBPAMODE - Specifies the mode of the Produce Assurance Processing.
*/
typedef enum
{
  PA_MODE_0,
  PA_MODE_1,
  PA_MODE_2,
  PA_MODE_3,
  PA_MODE_4
} SSBPAMODE;

/* 
*	@brief	enum:	SSBDBACTIONS - Actions required to execute DB Request from client.
*/
typedef enum
{
  GET, /**< Gets the item details from PLA DB */
  DEL /**< Deletes the Item from PLA DB */
} SSBDBACTIONS;

/**
* @brief Structure: Contains the Item Details required for Validation Request
*
* @members 
*		nRequestID : Request ID recieved from the Client.
*		nWeight : Weight of the Item on scale.
*		csItemUPC : Item UPC Code
*		cDescription : [Optional] Item Description
*/
typedef struct
{
  int nRequestID;
  int nWeight;
  const char* csItemUPC;
  const char* cDescription;
  //  bool isTrack;
} SSBVALREQUEST,*PSSBVALREQUEST;

/**
* @brief Structure: Contains the Item Details required for Picklist Request
*
* @members 
*		nRequestID : Request ID recieved from the Client.
*		nWeight : Weight of the Item on scale.
*/
typedef struct
{
  int nRequestID;
  int nWeight;
} SSBPLAREQUEST,*PSSBPLAREQUEST;
/**
* @brief Structure: Contains the Item UPC and confidence returned from Picklist. 
*
* @members 
*		nItemUPC : Unique Item Identifier.
*		nConfidence : Confidence returned from PLA Library.
*/
typedef struct
{
  char nItemUPC[30];
  int nConfidence;
} SSBCANDIDATE,*PSSBCANDIDATE;
/**
* @brief Structure: Response returned from PLA Contains the  list of Items with confidence 
*
* @members 
*		pCandidatelist : List of Items
*		number : number of Items.
*		enStatus : Status of the request. - SUCCESS, FAILURE, DISABLED
*/
typedef struct
{
  PSSBCANDIDATE pCandidatelist;
  int number;
  SSBRESSTATUS enStatus;
} SSBPLARESPONSE,*PSSBPLARESPONSE;

/**
* @brief Structure: Response returned from validation Contains whether the item is suspected or not 
*
* @members 
*		nItemUPC : Unique Item Identifier.
*		isSuspected : true - if item is suspected, false - if the item is not suspected.
*		cImagePath : Suspected Image Path
*		enStatus : Status of the request. (values depends on the flow and function)
*		noAlert : When no alert need to be sent to client(Silent-Mode).
*/
typedef struct
{
	bool isSuspected;
	SSBSOURCE enSource;
	SSBRESSTATUS enStatus;
	char cItemUPC[MAX_PATH];
	char cImagePath[MAX_PATH];
} SSBVALRESPONSE,*PSSBVALRESPONSE;
/**
* @brief Structure: Contains the error Information of the System and Subsystem 
*
* @members 
*		nErrorCode : SSB Functionality Error Code.
*		nSubSystemError : Sub-System Error code.
*		enErroLevel : Error Level.
*		enErrorModule : Specifies the Sub-system
*		enSSBStatus : Status of the SSB
*/
typedef struct
{
	int nErrorCode;
	int nSubSystemError;
	SSBERRORLEVEL enErroLevel;
	SSBERRORMODULE   enErrorModule;
	SSBSTATUS	enErrorSatus;
	char csAdditionInfo[MAX_PATH];
}SSBERROR,*PSSBERROR; 

/**
* @brief Structure: Contains the error Information of the System and Subsystem 
*
* @members 
*		stError : Error information
*		csErrorTxt : Error Description for the SSB Error.
*		csSubSytemErrorTxt : Error Description for the Sub-System Error.
*
*/
typedef struct
{
	SSBERROR stError;
	char csErrorTxt[MAX_PATH]; 
	char csSubSytemErrorTxt[MAX_PATH];
} SSBERRORINFO,*PSSBERRORINFO;

/**
* @brief Structure: Contains the PLA configuration from client
*
* @members 
*		noofitems : Number of Items returned from PLA Library.
*		bEnable : Picklist functionalty enable/disable.
*		bCenterDetection : Center Dectection functionality enable/disable.
*		bValEnable : PLA Validation functionality enable/disable.
*		bScaleMetric : Scale in metric or not.
*		ltimeout : Maximum timeout.
*
*/
typedef struct
{
  int noofitems;
  bool bEnable;
  bool bCenterDetection;
  bool bValEnable;
  bool bScaleMetric;
  long ltimeout;
} SSBPLACONFIG,*PSSBPLACONFIG;

/**
* @brief Structure: Contains the PLA configuration from client
*
* @members 
*		bEnable : Produce Assurance functionalty enable/disable.
*		bCenterDetection : Center Dectection functionality enable/disable.
*		nMode : Produce Assurance mode - Aggressive/Conservative.
*		bScaleMetric : Scale in metric or not.
*		ltimeout : Maximum timeout.
*
*/
typedef struct
{
  bool bEnable;
  bool bCenterDetection;
  bool bScaleMetric;
  int nMode;
  long ltimeout;
} SSBPACONFIG,*PSSBPACONFIG;

/**
* @brief Structure: Contains the Item details from DB 
*
* @members 
*		cItemUPC : Unique Identifier of an Item.
*		cDescription : Item Description.
*
*/
typedef struct
{
  char  cItemUPC[100];
  char  cDescription[100];
} SSBDBITEM,*PSSBDBITEM;

/**
* @brief Structure: Contains the list of items for DB Operations  
*
* @members 
*		pItems : List of Items
*		nItemCount : number of items in the list.
*		action : action required - GET/DELETE.
*
*/
typedef struct
{
  PSSBDBITEM pItems;
  int	nItemCount;
  SSBDBACTIONS  enAction;
} SSBDBITEMS,*PSSBDBITEMS;

typedef struct
{
	unsigned char m_nWeightUnit;
	char m_csWeight[3];
	char m_csWeightSub[4];
} SSBMETADATA,*PSSBMETADATA;

#ifdef _WIN32
typedef void (_stdcall *SetStatusString)(const char*);
#else
typedef void (*SetStatusString)(const char*);
#endif

// String Table
// The following definitions were taken from the string tables in ScanWatchSecurityBroker.rc.
#define OPOS_E_OFFLINE_STR                  _T("The device is off-line.")
#define OPOS_E_NOHARDWARE_STR               _T("The device is not connected to the system or is not powered on.")
#define OPOS_E_ILLEGAL_STR                  _T("Attempt was made to perform an illegal or unsupported operation with the device, or an invalid parameter value was used.")
#define OPOS_E_DISABLED_STR                 _T("Cannot perform operation while device is disabled.")
#define OPOS_E_NOSERVICE_STR                _T("The Control cannot communicate with the Service Object. Most likely, a setup or configuration error must be corrected.")
#define OPOS_E_NOTCLAIMED_STR               _T("Attempt was made to access an exclusive-use device that must be claimed before the method or property set action can be used. If the device is already claimed by another process, then the status OPOS_E_CLAIMED is returned instead.")
#define OPOS_E_CLAIMED_STR                  _T("Attempt was made to access a device that is claimed by another process. The other must release the device before this access may be made.")
#define OPOS_E_CLOSED_STR                   _T("Attempt was made to access a close device.")
#define SSB_DM_SUCCESS_STR                  _T("Device Manager: Success")
#define SSB_DM_ERROR_UNABLE_TO_CREATE_STR   _T("Unable to Create Device")
#define SSB_DM_ERROR_UNABLE_TO_OPEN_STR     _T("Unable to Open Device")
#define SSB_DM_ERROR_INITDEVICE_STR         _T("Device Manager: Device Initialization Failed")
#define SSB_DM_ERROR_CHECKHEALTH_FAILED_STR _T("Device Check Health Failed")
#define SSB_DM_ERROR_DATA_EVENT_STR         _T("Device Manager: Data Event Failed")
#define SSB_DM_ERROR_UNABLE_TO_CLAIM_STR    _T("Unable to Claime Device")
#define SSB_DM_ERROR_UNABLE_TO_ENABLE_STR   _T("Unable to Enable Device")
#define SSB_DM_ERROR_STARTSESSION_STR       _T("Device Manager: Start Session Failed")
#define SSB_DM_ERROR_STOPSESSION_STR        _T("Device Manager: Stop Session Failed")
#define SSB_DM_ERROR_RELEASEDEVICE_STR      _T("Device Manager: Failed to Release Device")
#define SSB_DM_ERROR_DESTROYWINDOW_STR      _T("Failed to Destroy Device Window")
#define SSB_DM_ERROR_CLOSED_STR             _T("Failed to Close the Device")
#define SSB_DM_ERROR_METADATAERROR_STR      _T("Device Manager: Failed to Get Metadata")
#define SSB_DM_ERROR_SCANWATCHINIT_STR      _T("Device Manager: Failed to Initialize Scanwatch Process")
#define SSB_DM_ERROR_GETTINGIMAGE_STR       _T("Device Manager: Failed to Get Image from Device")
#define SSB_DM_ERROR_SET_CAMERA_NUMBER_STR	_T("Device Manager: Failed to Set Camera Number")
#define SSB_DM_ERROR_SETIMAGETYPE_STR       _T("Device Manager: Failed to Set Image Type")
#define SSB_DM_ERROR_SCANWATCHRESET_STR     _T("Device Manager: Failed to Reset Scanwatch Process")
#define SSB_DM_ERROR_GETCAMERAINFO_STR      _T("Failed to Get Camera Information")
#define SSB_DM_EEROR_UVC_NOT_ENABLED_STR	_T("Failed to enumerate UVC device")
#define SSB_DM_ERROR_SCALE_OFFLINE_STR		_T("Scale Offline/Disabled")
#define SSB_DM_ERROR_MODEL_NOTSUPPORTED_STR	_T("Device model not supports PA/PLA")
#define SSB_DM_ERROR_FW_NOTSUPPORTED_STR	_T("Device firmware not supported")
#define SSB_DM_ERROR_SCANWATCH_DISABLED_STR	_T("Scanwatch bit not enabled on Scanner")
#define SSB_DM_ERROR_INVAL_CAMERA_INFO_STR  _T("Scanner Configuration not found in Camera Information")
#define SSB_DM_STATUS_SUSPICIOUS_STR        _T("Suspicious Item Found")
#define SSB_DM_ERROR_REFRESH_DEVICE_STR		_T("Failed to refresh the Device")
#define SSB_DM_ERROR_DEVICE_OFFLINE_STR		_T("Scanner disconnected or offline")
#define SSB_ERROR_UNKNOWN_EXCEPTION_STR		_T("SSB Unknown Exception")
#define OPOS_EX_INVALID_CONTROL             _T("Invalid request to Device, Control ID not found. ")
#define OPOS_EX_METADATA_NOT_AVAILABLE      _T("Metadata not available on Device")
#define OPOS_REFF_META_DATA                 _T("Device Processing Metadata (Wait and Try)")
#define OPOS_E_BUSY_STR                     _T("Cannot perform operation while the State is OPOS_S_BUSY: Must wait until output is no longer in progress.")
#define OPOS_E_TIMEOUT_STR                  _T("The Service Object timed out waiting for a response from the device, or the Control timed out waiting for a response from the Service Object.")
#define OPOS_E_FAILURE_STR                  _T("The device cannot perform the requested procedure, even though the device is connected to the system, powered on, and on-line.")
#define OPOS_E_EXISTS_STR                   _T("The file name (or other specified value) already exists.")
#define OPOS_E_NOEXIST_STR                  _T("The file name (or other specified value) does not exist")
#define SSB_PLA_ERROR_INITIALIZE_STR        _T("PLA: Failed to initialize the PickList Assist Library ")
#define SSB_PLA_ERROR_INSTANCE_STR	        _T("PLA: Failed to get instance of CPickListAssistApi")
#define SSB_PLA_ERROR_PROCESSING_STR		_T("PLA: Failed to process PickList Assist request")
#define SSB_PLA_ERROR_INVALID_BUFF_STR		_T("PLA: Failed to read item/reference image buffer")
#define SSB_PLA_ERROR_TRAINING_STR			_T("PLA: Failed to Train item")
#define SSB_PLA_ERROR_UPDATE_REF_STR        _T("PLA: Failed to update Reference Image")
#define SSB_PLA_ERROR_GET_LIST_STR	        _T("PLA: Failed to Get Candidate list from PickList Database")
#define SSB_PLA_ERROR_DELETE_LIST_STR       _T("PLA: Failed to Delete PickList Item from Database")
#define SSB_PLA_ERROR_RESET_ALL_STR         _T("PLA: Failed to Reset items in pickList Database")
#define SSB_PLA_ERROR_GET_ITEM_STR			_T("PLA: Failed to Get Item from PickList Database")
#define SSB_PLA_ERROR_RESET_ITEM_STR		_T("PLA: Failed to Reset Item in PickList Database")
#define SSB_PLA_ERROR_CAMERAINFO_STR		_T("PLA: Failed to get camera setup information")
#define SSB_PLA_ERROR_NOT_SUPPORTED_STR		_T("PLA: PLA Feature not Supported on device")
#define SSB_PA_ERROR_INITIALIZE_STR         _T("PA: Failed to initialize the ProduceAssurance Library ")
#define SSB_PA_ERROR_INSTANCE_STR	        _T("PA: Failed to get instance of CProduceAssuranceApi")
#define SSB_PA_ERROR_LOG_FILE_STR			_T("PA: Failed to get log file name")
#define SSB_PA_ERROR_PROCESSING_STR			_T("PA: Failed to process ProduceAssurance request")
#define SSB_PA_ERROR_INVALID_BUFF_STR		_T("PA: Invalid item/reference image buffer")
#define SSB_PA_ERROR_IMAGE_COUNT_STR		_T("PA: Image count is not as expected")
#define SSB_PA_ERROR_CAMERAINFO_STR         _T("PA: Failed to get camera setup information")
#define SSB_PA_ERROR_CONFIG_STR		        _T("PA: Invalid Configuration")
#define SSB_PA_ERROR_NOT_SUPPORTED_STR		_T("PA: PA Feature not Supported on device")

#define SSB_SYSTEM_INVALID_PARAMS_STR		_T("Invalid Parameter")
#define SSB_SYSTEM_INVALID_INSTANCE_STR		_T("Invalid Instance")
#define SSB_SYSTEM_UNKNOWN_EXCEPTION_STR	_T("Unknown Exception")
#define SSB_SYSTEM_FAIL_COPY_STR			_T("Failed Copying images")
#define SSB_SYSTEM_FAIL_PROC_IMAGE_STR		_T("Failed processing Images")
#define SSB_SYSTEM_FAIL_INIT_STR			_T("Failed to Initialize SSB")
#define SSB_SYSTEM_FAIL_UNINT_STR			_T("Failed to Uninitialize SSB")
#define SSB_SYSTEM_FEATURE_DISABLED_STR		_T("Feature Disabled")
#define SSB_SYSTEM_HIGH_CPU_USAGE_STR		_T("SSB required CPU or RAM not available to claim the device")
#define SSB_SYSTEM_ERROR_OFFLINE_STR		_T("SSB Offline")
