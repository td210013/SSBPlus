/**
 * NCRImg.h declares the constants needed to make use of the NCR Image Scanner
 * OPOS extensions.
 */
/******************************************************************************
 *
 * $Workfile:: NCRImg.h                                                      $
 *
 * $Revision:: 1                                                             $
 *
 * $Archive:: /RetailControls/trunk/OPOS/include/NCR/NCRImg.h                $
 *
 * $Date:: 02/03/15 9:30a                                                    $
 *
 ******************************************************************************/

/*  NCRDIO_SCANWATCH is used as common command value for all ScanWatch
 *  related operations like scanwatch process initialization,Getting metadata,
 *  Reset metadata status and getting scanwatch licensed status.
 */
 const LONG NCRDIO_SCANWATCH  = 300;

//** SCANWATCH_INIT(600) - Initializes the scanwatch process.
//    Command :   NCRDIO_SCANWATCH
//    Data    :   In >    SCANWATCH_INIT (600)
//    String  :   (Not used.)
//    ResultCode: OPOS_SUCCESS   = Operation successful.
//                OPOS_E_ILLEGAL = Invalid input value or Direct IO method is called before Open-Claim-Enable.
//                OPOS_E_FAILURE = Operation failed or extended errors like Invalid Control ID or Scanwatch not licensed.
//    Extended  ResultCode :
//                NCR_EIMGSCAN_SCANWTACH_NOT_LICENSED
//                NCR_EIMGSCAN_INVALID_CONTROL_ID
 const LONG SCANWATCH_INIT    = 600;
 
//** SCANWATCH_GET_METADATA(601) - Retrieves scanwatch metadata.
//    Command :   NCRDIO_SCANWATCH
//    Data    :   In >    SCANWATCH_GET_METADATA (601)
//                Out>    This parameter is used to know the size of metadata(in bytes) available in String.
//    String  :   Scanwatch metadata will be returned in string as a byte array.Use below code to convert BSTR to Byte array.
//                BYTE *pStr = new BYTE[lDIOpData];//lDIOpData is data parameter and bstrOut is string parameter.
//                ::WideCharToMultiByte(CP_ACP, 0, bstrOut, lDIOpData, (LPSTR)pStr, lDIOpData, 0, 0);
//    ResultCode: OPOS_SUCCESS   = Operation successful.
//                OPOS_E_ILLEGAL = Invalid input value or Direct IO method is called before Open-Claim-Enable.
//                OPOS_E_FAILURE = Operation failed or extended errors like Invalid Control ID or Metadata not available or
//                                 Device is refreshing metadata or Scanwatch is not licensed.
//    Extended  ResultCode :
//                NCR_EIMGSCAN_SCANWTACH_NOT_LICENSED
//                NCR_EIMGSCAN_INVALID_CONTROL_ID
//                NCR_EIMGSCAN_METADATA_NOT_AVAILABLE
//                NCR_EIMGSCAN_REFRESHING_METADATA
 const LONG SCANWATCH_GET_METADATA   = 601;

//** SCANWATCH_RESET_METADATA_STATUS(602) - Resets scanwatch metadata status.
//    Command :   NCRDIO_SCANWATCH
//    Data    :   In >    SCANWATCH_RESET_METADATA_STATUS (602)
//    String  :   (Not used.)
//    ResultCode: OPOS_SUCCESS   = Operation successful.
//                OPOS_E_ILLEGAL = Invalid input value or Direct IO method is called before Open-Claim-Enable.
//                OPOS_E_FAILURE = Operation failed or extended errors like Invalid Control ID or Scanwatch not licensed.
//    Extended  ResultCode :
//                NCR_EIMGSCAN_SCANWTACH_NOT_LICENSED
//                NCR_EIMGSCAN_INVALID_CONTROL_ID
 const LONG SCANWATCH_RESET_METADATA_STATUS = 602;

//** IS_SCANWATCH_LICENSED(603) - Gets scanwatch license status.
//    Command :   NCRDIO_SCANWATCH
//    Data    :   In >    IS_SCANWATCH_LICENSED (603)
//    String  :   (Not used.)
//    ResultCode: OPOS_SUCCESS   = Scanwatch is licensed.
//                OPOS_E_ILLEGAL = Invalid input value or Direct IO method is called before Open-Claim-Enable.
//                OPOS_E_FAILURE = Operation failed or Scanwatch not licensed.
//    Extended  ResultCode :
//                NCR_EIMGSCAN_SCANWTACH_NOT_LICENSED
 const LONG IS_SCANWATCH_LICENSED = 603;//RPSUPOS-1576
 
//** SCANWATCH_GET_CAMERAINFO(604) - Camera set-up information.
//    Command :   NCRDIO_SCANWATCH
//    Data    :   In >    SCANWATCH_GET_CAMERAINFO(604)
//                Out>    This parameter is used to know the size of camera info(in bytes) available in String.
//    String  :   Camera info will be returned in string as a byte array.Use below code to convert BSTR to Byte array.
//                BYTE *pStr = new BYTE[lDIOpData];//lDIOpData is data parameter and bstrOut is string parameter.
//                ::WideCharToMultiByte(CP_ACP, 0, bstrOut, lDIOpData, (LPSTR)pStr, lDIOpData, 0, 0);
//    ResultCode: OPOS_SUCCESS   = Operation successful.
//                OPOS_E_ILLEGAL = Invalid input value or Direct IO method is called before Open-Claim-Enable.
//                OPOS_E_FAILURE = Operation failed or extended error(s) like Invalid Control ID.
//    Extended  ResultCode :
//                NCR_EIMGSCAN_INVALID_CONTROL_ID
 const LONG SCANWATCH_GET_CAMERAINFO = 604;//RPSUPOS-1729
 // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//** NCRDIO_SCANWATCH_SET_CAMERA_NUM(301) - sets the active camera number.
//    Command :   NCRDIO_SCANWATCH_SET_CAMERA_NUM
//    Data:       In > During ScanWatch mode,this value will determine which camera number corresponds to the ScanWatch image that will be downloaded.
//                Allowed Data Values :
//                CAMERA_NUM_CAM0 - Camera 0
//                CAMERA_NUM_CAM1 - Camera 1
//                CAMERA_NUM_CAM2 - Camera 2
//                CAMERA_NUM_CAM3 - Camera 3
//                Out> (Not Used).
//    String:     (Not used.)
//    ResultCode: OPOS_SUCCESS   = Operation successful.
//                OPOS_E_ILLEGAL = Invalid input value or Direct IO method is called before Open-Claim-Enable.
//                OPOS_E_FAILURE = Operation failed or extended errors like Invalid Control ID or Scanwatch is not licensed.
//    Extended  ResultCode :
//                NCR_EIMGSCAN_INVALID_CONTROL_ID
const LONG NCRDIO_SCANWATCH_SET_CAMERA_NUM  = 301;
// Data value constants used with NCRDIO_SCANWATCH_SET_CAMERA_NUM command
/** Camera zero*/
const LONG CAMERA_NUM_0  = 0;

/** Camera one*/
const LONG CAMERA_NUM_1  = 1;

/** Camera two*/
const LONG CAMERA_NUM_2  = 2;

/** Camera three*/
const LONG CAMERA_NUM_3  = 3;
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//** NCRDIO_SCANWATCH_SET_IMAGE_TYPE(302) - sets image type to be retrieved from scanwatch device.
//    Data:       In > During ScanWatch mode,this value will determine,which image type will be downloaded by the host.
//                Data values :
//                IMG_TYPE_ITEM - Retrieve item image from scanwatch device.
//                IMG_TYPE_REFERENCE - Retrieve reference image from scanwatch device
//                Out> (Not Used).
//    String:     (Not used.)
//    ResultCode: OPOS_SUCCESS   = Set successfully.
//                OPOS_E_ILLEGAL = Invalid input value or Direct IO method is called before Open-Claim-Enable.
//                OPOS_E_FAILURE = Operation failed or extended errors like Invalid Control ID or Scanwatch is not licensed.
//    Extended  ResultCode :
//                NCR_EIMGSCAN_SCANWTACH_NOT_LICENSED
//                NCR_EIMGSCAN_INVALID_CONTROL_ID
const LONG NCRDIO_SCANWATCH_SET_IMAGE_TYPE  = 302;
// Data value constants used with NCRDIO_SCANWATCH_SET_IMAGE_TYPE command
/** Item image*/
const LONG IMG_TYPE_ITEM  = 0;

/** Reference image*/
const LONG IMG_TYPE_REFERENCE  = 1;
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//RPSUPOS-1665 start
// Image Scanner SO has to provide a Direct IO call to know NCR specific Image type(YUY2) and UPOS standard image type on Image event.
//**  NCRDIO_GET_IMAGE_TYPE (303) - Gets NCR specific/UPOS standard image type.
//    Data:       In > (Not Used)
//                Out> Value of Image type.
//                     IMG_TYP_BMP (OPOS Constant)
//                     IMG_TYP_JPEG (OPOS Constant)
//                     IMG_TYP_GIF (OPOS Constant)
//                     IMG_TYP_PNG (OPOS Constant)
//                     IMG_TYP_TIFF(OPOS Constant)
//                     NCR_IMG_TYP_YUY2 - YUY2 data is retrieved.
//                     NCR_IMG_TYP_UNKNOWN - Other than YUY2 data.
//    String:     (Not used.)
//    ResultCode: OPOS_SUCCESS   = Image Type retrieved successfully.
//                OPOS_E_FAILURE = Operation Failed(Image Event not yet received).
const LONG NCRDIO_GET_IMAGE_TYPE  = 303;
/////////////////////////////////////////////////////////////////////
// Data value constants retrieved with NCRDIO_GET_IMAGE_TYPE command
/////////////////////////////////////////////////////////////////////
 /** Image type is YUY2 */
const LONG NCR_IMG_TYP_YUY2                 = 21;

/** Image type is unknown */
const LONG NCR_IMG_TYP_UNKNOWN              = 0;
// RPSUPOS-1665 End
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//** NCRDIO_BCSD_INIT(304) - Initializes barcode substitution detection process.
//    Command :   NCRDIO_BCSD_INIT
//    Data:       In > (Not Used).
//                Out> (Not Used).
//    String:     (Not used.)
//    ResultCode: OPOS_SUCCESS   = Operation successful.
//                OPOS_E_ILLEGAL = Direct IO method is called before Open-Claim-Enable.
//                OPOS_E_FAILURE = Operation failed or extended error(s) like Invalid Control ID.
//    Extended  ResultCode :
//                NCR_EIMGSCAN_INVALID_CONTROL_ID
const LONG NCRDIO_BCSD_INIT  = 304;
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//** NCRDIO_BCSD_GET_METADATA(305) - Retrieves barcode substitution metadata information.
//    Command :   NCRDIO_BCSD_GET_METADATA
//    Data    :   In > (Not Used).
//                Out> This parameter can be used to know the size of metadata(in bytes) available in String.
//    String  :   BCSD metadata will be returned in string as a byte array.Use below code to convert BSTR to Byte array.
//                BYTE *pStr = new BYTE[lDIOpData];//lDIOpData is data parameter and bstrOut is string parameter.
//                ::WideCharToMultiByte(CP_ACP, 0, bstrOut, lDIOpData, (LPSTR)pStr, lDIOpData, 0, 0);
//    ResultCode: OPOS_SUCCESS   = Operation successful.
//                OPOS_E_ILLEGAL = Invalid input value or Direct IO method is called before Open-Claim-Enable.
//                OPOS_E_FAILURE = Operation failed or extended error(s) like Invalid Control ID.
//    Extended  ResultCode :
//                NCR_EIMGSCAN_INVALID_CONTROL_ID
 const LONG NCRDIO_BCSD_GET_METADATA = 305;
 // . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//** NCRDIO_BCSD_RESET(306) - Resets barcode substitution detection process.
//    Command :   NCRDIO_BCSD_RESET
//    Data:       In > (Not Used).
//                Out> (Not Used).
//    String:     (Not used.)
//    ResultCode: OPOS_SUCCESS   = Operation successful.
//                OPOS_E_ILLEGAL = Direct IO method is called before Open-Claim-Enable.
//                OPOS_E_FAILURE = Operation failed or extended error(s) like Invalid Control ID.
//    Extended  ResultCode :
//                NCR_EIMGSCAN_INVALID_CONTROL_ID
const LONG NCRDIO_BCSD_RESET  = 306;
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//**  NCRDIO_SET_FRAME_RESOLUTION (307) - Sets frame resolution.
//    Command :   NCRDIO_SET_FRAME_RESOLUTION
//    Data:       In > Frame resolution value.
//                     RESOLUTION_320x240
//                     RESOLUTION_640x480
//                     RESOLUTION_1280x960
//                Out> (Not Used)
//    String:     (Not used.)
//    ResultCode: OPOS_SUCCESS   = Resolution changed successfully.
//                OPOS_E_FAILURE = Operation failed or extended error(s) like resolution not supported.
//    Extended  ResultCode :
//                NCR_EIMGSCAN_RESOLUTION_NOT_SUPPORTED
const LONG NCRDIO_SET_FRAME_RESOLUTION  = 307;
/////////////////////////////////////////////////////////////////////
// Data value constants retrieved with NCRDIO_SET_FRAME_RESOLUTION command
/////////////////////////////////////////////////////////////////////
 /** 320x240 resolution*/
const LONG RESOLUTION_320x240               = 0;

 /** 640x480 resolution*/
const LONG RESOLUTION_640x480               = 1;

 /** 1280x960 resolution*/
const LONG RESOLUTION_1280x960              = 2;
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//** NCRDIO_SET_BCSD_IMAGE_BUFFER_INDEX(308) - Sets BCSD image buffer index value.
//    Data:       In > This value will be determine,which BCSD buffer image part will be downloaded by the host.
//                Data values : Zero based index values.Allowed index range is 0 to 18
//                Out> (Not Used).
//    String:     (Not used.)
//    ResultCode: OPOS_SUCCESS   = Set successfully.
//                OPOS_E_FAILURE = Operation failed or extended errors like Invalid Control ID.
//    Extended  ResultCode :
//                NCR_EIMGSCAN_INVALID_CONTROL_ID
const LONG NCRDIO_SET_BCSD_IMAGE_BUFFER_INDEX  = 308;

/////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// NCR Specific "ResultCodeExtended" Property Constants for ImageScanner
////////////////////////////////////////////////////////////////////////////

/** Invalid UVC control ID.This error occurs when ImageScanner SO try to access undefined UVC control ID */
const long  NCR_EIMGSCAN_INVALID_CONTROL_ID      = 1 + OPOSERREXT;

/** Metadata not available error */
const long  NCR_EIMGSCAN_METADATA_NOT_AVAILABLE  = 2 + OPOSERREXT;

/** Metadata refreshing error*/
const long  NCR_EIMGSCAN_REFRESHING_METADATA     = 3 + OPOSERREXT;

/** Scanwatch is not licensed error */
const long  NCR_EIMGSCAN_SCANWTACH_NOT_LICENSED  = 4 + OPOSERREXT;//RPSUPOS-1576

const long  NCR_EIMGSCAN_BYTE_ARRAY_TOOSMALL     = 5 + OPOSERREXT;//sync with javaPOS error codes.

/** Resolution not supported  error*/
const long  NCR_EIMGSCAN_RESOLUTION_NOT_SUPPORTED  = 6 + OPOSERREXT;



