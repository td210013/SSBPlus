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
 * $Archive:: /RetailControls/Rel.3_4/OPOS/include/NCR/NCRImg.h              $
 *
 * $Date:: 02/03/15 9:30a                                                    $
 *
 ******************************************************************************/

#ifndef NCRIMG_H
#define NCRIMG_H

#ifndef _WIN32
#include "JposConstants.h"
#else
#include "Opos.h"
#endif

//** Scanwatch(300) ** Scanwatch related features.
//    Data:       In > Control Type.
//SCANWATCH_INIT (600)
//SCANWATCH_GET_METADATA (601)
//SCANWATCH_RESET_METADATA_STATUS (602)
//IS_SCANWATCH_LICENSED (603)
//                Out> for 'SCANWATCH_GET_METADATA' control type,this param is used to know the size of metadata(in bytes).
//    String:     For 'SCANWATCH_GET_METADATA' control type,this param is used to retrieve metadata.
//    ResultCode: OPOS_SUCCESS   = Set successfully.
//                OPOS_E_ILLEGAL = Invalid Control type.
//				  OPOS_E_FAILURE = Operation Failed
//				  OPOS_E_EXTENDED = Invalid Control ID or Metadata not available or Device is refreshing metadata
//	  Extended	ResultCode :
//				  NCR_EIMGSCAN_SCANWTACH_NOT_LICENSED
//				  NCR_EIMGSCAN_INVALID_CONTROL_ID
//				  NCR_EIMGSCAN_METADATA_NOT_AVAILABLE (Only for SCANWATCH_GET_METADATA)
//				  NCR_EIMGSCAN_REFRESHING_METADATA (Only for SCANWATCH_GET_METADATA)

const long NCRDIO_SCANWATCH  = 300;
const long SCANWATCH_INIT    = 600;
const long SCANWATCH_GET_METADATA   = 601;
const long SCANWATCH_RESET_METADATA_STATUS = 602;
const long IS_SCANWATCH_LICENSED = 603;//RPSUPOS-1576
const long SCANWATCH_GET_CAMERAINFO = 604;
const long SCANWATCH_CAMERAINFO_SIZE = 512;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//** Scanwatch set camera number (301)** sets the active camera number.
//    Data:       In > During ScanWatch mode,this value will determine which camera number corresponds to the ScanWatch image that will be downloaded.
//				  Data values :
//				  CAMERA_NUM_CAM0 - Camera 0
//				  CAMERA_NUM_CAM1 - Camera 1
//				  CAMERA_NUM_CAM2 - Camera 2
//				  CAMERA_NUM_CAM3 - Camera 3
//                Out> (Not Used).
//    String:     (Not used.)
//    ResultCode: OPOS_SUCCESS   = Set successfully.
//                OPOS_E_ILLEGAL = Invalid command.
//				  OPOS_E_FAILURE = Operation Failed
//				  OPOS_E_EXTENDED = Invalid Control ID or Scanwatch not licensed
//	  Extended	ResultCode :
//				  NCR_EIMGSCAN_SCANWTACH_NOT_LICENSED
//				  NCR_EIMGSCAN_INVALID_CONTROL_ID
const long NCRDIO_SCANWATCH_SET_CAMERA_NUM  = 301;
const long CAMERA_NUM_0  = 0;
const long CAMERA_NUM_1  = 1;
const long CAMERA_NUM_2  = 2;
const long CAMERA_NUM_3  = 3;
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//** Scanwatch set image type (302)** sets image type to be retrieved from scanwatch device.
//    Data:       In > During ScanWatch mode,this value will determine,which image type will be downloaded by the host.
//				  Data values :
//				  IMG_TYPE_ITEM - Retrieve item image from scanwatch device.
//				  IMG_TYPE_REFERENCE - Retrieve reference image from scanwatch device//
//                Out> (Not Used).
//    String:     (Not used.)
//    ResultCode: OPOS_SUCCESS   = Set successfully.
//                OPOS_E_ILLEGAL = Invalid command.
//				  OPOS_E_FAILURE = Operation Failed
//				  OPOS_E_EXTENDED = Invalid Control ID or Scanwatch not licensed
//	  Extended	ResultCode :
//				  NCR_EIMGSCAN_SCANWTACH_NOT_LICENSED
//				  NCR_EIMGSCAN_INVALID_CONTROL_ID
const long NCRDIO_SCANWATCH_SET_IMAGE_TYPE  = 302;
const long IMG_TYPE_ITEM  = 0;
const long IMG_TYPE_REFERENCE  = 1;

//RPSUPOS-1665 start
// Image Scanner SO has to provide a Direct IO call to know NCR specific Image type(YUY2) on Image event.
//**  Get image type (303)** Gets NCR specific image type.
//    Data:       In > (Not Used)
//                Out> Value of NCR specific Image type.
//					   NCR_IMG_TYP_YUY2 - YUY2 data is retrieved.
//					   NCR_IMG_TYP_UNKNOWN - Other than YUY2 data.
//    String:     (Not used.)
//    ResultCode: OPOS_SUCCESS   = Image Type retrieved successfully.
//				  OPOS_E_FAILURE = Operation Failed(Image Event not yet received).
const long NCRDIO_GET_IMAGE_TYPE  = 303;
//RPSUPOS-1665 End

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// "ResultCodeExtended" Property Constants for ImageScanner
/////////////////////////////////////////////////////////////////////

#ifdef _WIN32
const long  NCR_EIMGSCAN_INVALID_CONTROL_ID = 1 + OPOSERREXT; //Invalid Control ID
const long  NCR_EIMGSCAN_METADATA_NOT_AVAILABLE = 2 + OPOSERREXT;//Metadata not available
const long  NCR_EIMGSCAN_REFRESHING_METADATA = 3 + OPOSERREXT;//Device is refreshing metadata
const long  NCR_EIMGSCAN_SCANWTACH_NOT_LICENSED = 4 + OPOSERREXT;//Scanwatch is not licensed //RPSUPOS-1576
#else
const long  NCR_EIMGSCAN_INVALID_CONTROL_ID = 1 + JPOSERREXT; //Invalid Control ID
const long  NCR_EIMGSCAN_METADATA_NOT_AVAILABLE = 2 + JPOSERREXT;//Metadata not available
const long  NCR_EIMGSCAN_REFRESHING_METADATA = 3 + JPOSERREXT;//Device is refreshing metadata
const long  NCR_EIMGSCAN_SCANWTACH_NOT_LICENSED = 4 + JPOSERREXT;//Scanwatch is not licensed //RPSUPOS-1576
#endif

// RPSUPOS-1665 Start
/////////////////////////////////////////////////////////////////////
// NCR Extended "ImageType" Property Constant(s)
/////////////////////////////////////////////////////////////////////
const long NCR_IMG_TYP_YUY2                 = 21;
const long NCR_IMG_TYP_UNKNOWN              = 0;
// RPSUPOS-1665 End

#endif /* NCRIMG_H */
