/**
 * NCRScan.h declares the constants needed to make use of the NCR Scanner
 * OPOS extensions.
 */
/******************************************************************************
 *
 * $Workfile:: NCRScan.h                                                      $
 *
 * $Revision:: 4                                                              $
 *
 * $Archive:: /RetailControls/Rel.3_4/OPOS/include/NCR/NCRScan.h              $
 *
 * $Date:: 8/09/05 9:30a                                                      $
 *
 ******************************************************************************
 *
 * Copyright (c) NCR Corporation 2004.  All rights reserved.
 *
 ******************************************************************************
 *
 * $History:: NCRScan.h                                                       $
 * 
 * *****************  Version 4  *****************
 * User: Mr160002     Date: 8/09/05    Time: 9:30a
 * Updated in $/RetailControls/Rel.3_4/OPOS/include/NCR
 * Added USB DirectIO
 * 
 * *****************  Version 2  *****************
 * User: Mr160002     Date: 5/26/04    Time: 7:40p
 * Updated in $/RetailControls/Rel.3_3/OPOS/include/NCR
 * Added NCR Specific RFID scan data types
 * 
 * *****************  Version 1  *****************
 * User: Mr160002     Date: 5/13/04    Time: 4:31p
 * Created in $/RetailControls/Rel.3_3/OPOS/include/NCR
 *
 *
 *****************************************************************************/

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// NCR Scanner value-add extensions via DirectIO.
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// "DirectIO" Method Constants for Scanner (NCR 787x, 788x, and 7892)
// Not supported by 783x scanners.
/////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

//** Scanner Tone(501) **  Scanner Tone control.
//    Data:       In > Control Type.
//SCAN_TONE_BEEP (1001)
//SCAN_TONE_ENABLE (1002)
//SCAN_TONE_DISABLE (1003)
//                Out> None.
//    String:     (Not used.)
//    ResultCode: OPOS_SUCCESS   = Set successfully.
//                OPOS_E_ILLEGAL = Invalid Control type.
const LONG NCRDIO_SCAN_TONE             = 501;
const LONG SCAN_TONE_BEEP               = 1001;
const LONG SCAN_TONE_ENABLE             = 1002;
const LONG SCAN_TONE_DISABLE            = 1003;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//** Scanner Reset(502) **  Scanner reset.
//    Data:       In > (Not Used).
//                Out> (Not Used).
//    String:     (Not used.)
//    ResultCode: OPOS_SUCCESS   = Set successfully.
//                OPOS_E_ILLEGAL = Invalid command.
const LONG NCRDIO_SCAN_RESET            = 502;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//** Scanner Status Request(503) **  Returns Scanner Status.
//    Data:       In > (Not Used).
//                Out> (Not Used).
//    String:     Status string (direct from scanner). You must know the scanner interface to decode the string.
//    ResultCode: OPOS_SUCCESS   = Set successfully.
//                OPOS_E_ILLEGAL = Invalid command.
const LONG NCRDIO_SCAN_STATUS           = 503;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//** Scanner Read ROM Request(504) **  Returns ROM data.
//    Data:       In > Address to read from.
//                Out> (Not Used).
//    String:     Returns 30 bytes of ROM data
//    ResultCode: OPOS_SUCCESS   = Set successfully.
//                OPOS_E_ILLEGAL = Invalid command.
const LONG NCRDIO_SCAN_READROM          = 504;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//** Scanner Read ROM Version Request(505) **  Returns ROM version.
//    Data:       In > (Not Used).
//                Out> (Not Used).
//    String:     Returns the ROM version ID
//    ResultCode: OPOS_SUCCESS   = Set successfully.
//                OPOS_E_ILLEGAL = Invalid command.
const LONG NCRDIO_SCAN_ROM_VERSION      = 505;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//** Scanner PaceSetter Control(506) **  Returns ROM version.
//    Data:       In > Requested sub-command
//PACESETTER_READ_GOOD  (1101)
//PACESETTER_READ_NO_READS  (1102)
//PACESETTER_READ_OVERPRINT  (1103)
//PACESETTER_READ_UNDERPRINT  (1104)
//PACESETTER_READ_MISS_MARGIN  (1105)
//PACESETTER_RESET  (1106) PACESETTER_ENABLE_MODE_3  (1107) PACESETTER_DISABLE_MODE_3  (1108)
//                Out> The return value is the status of the Direct I/O sub-command.
//If the return status is OPOS_SUCCESS and a READ sub-command (1101-1105) was issued,
//the tally value is placed in pData.
//    String:     (Not Used).
//    ResultCode: OPOS_SUCCESS   = Set successfully.
//                OPOS_E_ILLEGAL = Invalid command.
const LONG NCRDIO_SCAN_PACESETTER       = 506;
const LONG PACESETTER_READ_GOOD         = 1101;
const LONG PACESETTER_READ_NO_READS     = 1102;
const LONG PACESETTER_READ_OVERPRINT    = 1103;
const LONG PACESETTER_READ_UNDERPRINT   = 1104;
const LONG PACESETTER_READ_MISS_MARGIN  = 1105;
const LONG PACESETTER_RESET             = 1106;
const LONG PACESETTER_ENABLE_MODE_3     = 1107;
const LONG PACESETTER_DISABLE_MODE_3    = 1108;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .


//** Scanner Direct Access(507) **  Sends data directly to the scanner.
//    Data:       In > True (1) if data is to be returned from the scanner, and False (0) if no data is to be returned.
//                Out> (Not Used).
//
//    String:     In > String to send to the scanner, including the suffix and BCC character (if needed).
//				  Out> The string data returned (direct from scanner).
//					   You must know the scanner interface to decode the string.from the scanner.
//    ResultCode: OPOS_SUCCESS   = Set successfully.
//                OPOS_E_ILLEGAL = Invalid command.
const LONG NCRDIO_SCAN_DIRECT           = 507;
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//** Scanner Not-On-File(508) **  Sends NOF to the scanner.
//    Data:       In > (Not Used).
//                Out> (Not Used).
//    String:     (Not Used).
//    ResultCode: OPOS_SUCCESS   = Set successfully.
//                OPOS_E_ILLEGAL = Invalid command.
const LONG NCRDIO_SCAN_NOT_ON_FILE      = 508;
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//** Scanner Suspend(509) **  Sends Suspend to the scanner.
//    Data:       In > (Not Used).
//                Out> (Not Used).
//    String:     (Not Used).
//    ResultCode: OPOS_SUCCESS   = Set successfully.
//                OPOS_E_ILLEGAL = Invalid command.
const LONG NCRDIO_SCAN_SUSPEND          = 509;
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//** Scanner Start(510) **  Sends Start to the scanner.
//    Data:       In > (Not Used).
//                Out> (Not Used).
//    String:     (Not Used).
//    ResultCode: OPOS_SUCCESS   = Set successfully.
//                OPOS_E_ILLEGAL = Invalid command.
const LONG NCRDIO_SCAN_START            = 510;
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
// USB DIRECTIO
const LONG NCRDIO_USB_DIRECTIO     = 511;

//** Customer Scanner Enable(514) **  Sends enable command to the customer scanner.
//    Data:       In > The port where customer scanner is connected.
//                Out> (Not Used).
//    String:     (Not Used).
//    ResultCode: OPOS_SUCCESS   = On Success.
//                OPOS_E_ILLEGAL = Invalid command.
const UINT32 NCRDIO_CS_ENABLE           = 514;

//** Customer Scanner Disable(515) **  Sends disable command to the customer scanner.
//    Data:       In > The port where customer scanner is connected.
//                Out> (Not Used).
//    String:     (Not Used).
//    ResultCode: OPOS_SUCCESS   = On Success.
//                OPOS_E_ILLEGAL = Invalid command.
const UINT32 NCRDIO_CS_DISABLE          = 515;

//** Customer Scanner Status(516) **  Gets customer scanner status.
//    Data:       In > The port where customer scanner is connected.
//                Out> (Not Used).
//    String:     Status of the customer scanner.
//    ResultCode: OPOS_SUCCESS   = On Success.
//                OPOS_E_ILLEGAL = Invalid command.
const UINT32 NCRDIO_CS_STATUS           = 516;

  const LONG USB_DEVICE_FUID             = 1111;  //Full Unit ID
  const LONG USB_DEVICE_FIRMWARE         = 1112;
  const LONG USB_DEVICE_SERIALNUMBER     = 1113;
  const LONG USB_MISC_STATUS             = 1114;
  const LONG USB_PROXIMITY_STATUS        = 1115;
  const LONG USB_DEVICE_TIME_DATE        = 1116;
  const LONG USB_DEVICE_BEEP             = 1117;
  const LONG USB_DEVICE_NOF              = 1118;
  const LONG USB_RESET_TALLIES           = 1119;
  const LONG USB_DEVICE_CLEAR_NOF        = 1120;
  const LONG USB_PACESETTER_III          = 1121;
  const LONG USB_PACESETTER_III_LABELS   = 1122;
  const LONG USB_CONFIG_RSS14            = 1123;
  const LONG USB_GET_RSS14_CONFIG        = 1124;
  const LONG USB_DEVICE_CONFIG_REPORT    = 1125;
     

// END USB DIRECTIO
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// "ResultCodeExtended" Property Constants for Scanner
/////////////////////////////////////////////////////////////////////
// Reserved for SCALE:  From 0 + OPOSERREXT to 99 + OPOSERREXT.
// Reserved for SCANNER:  From 100 + OPOSERREXT to 199 + OPOSERREXT.
const LONG NCR_ESCAN_DATALOSS           = 100 + OPOSERREXT; //  Not used
const LONG NCR_ESCAN_BADTAGSUFFIX       = 101 + OPOSERREXT; //  Bad suffix byte on Tag
const LONG NCR_ESCAN_BADTAGPREFIX       = 102 + OPOSERREXT; //  Bad prefix byte on Tag
const LONG NCR_ESCAN_BADTAG             = 103 + OPOSERREXT; //  Missing "30 38" for scanner/scale tag
const LONG NCR_ESCAN_SERIAL             = 104 + OPOSERREXT; //  Serial line error
const LONG NCR_ESCAN_WOBULATOR_FAILURE  = 105 + OPOSERREXT;
const LONG NCR_ESCAN_LASER_FAILURE			= 106 + OPOSERREXT;
const LONG NCR_ESCAN_MOTOR_FAILURE			= 107 + OPOSERREXT;
const LONG NCR_ESCAN_MOTIONTHREAD_FAILURE = 108 + OPOSERREXT; // Motion Thread failed to start
const LONG NCR_ESCAN_BADSUFFIX       = 109 + OPOSERREXT; //  Bad suffix byte
const LONG NCR_ESCAN_BADPREFIX       = 110 + OPOSERREXT; //  Bad prefix byte


// Scan Data Type RFID
// Special cases
const LONG SCAN_SDT_RFID_UNKNOWN       = SCAN_SDT_OTHER + 100;  // Start of RFID specific barcodes
const LONG SCAN_SDT_RFID_OTHER       = SCAN_SDT_OTHER + 101;  // Start of RFID specific barcodes
const LONG SCAN_SDT_RFID       = SCAN_SDT_OTHER + 102;  // Start of RFID specific barcodes

// end

