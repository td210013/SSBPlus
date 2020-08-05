/**
 * NCRPtr.h declares the constants needed to make use of the NCR POS Printer
 * OPOS extensions.
 */
/******************************************************************************
 *
 * $Workfile:: NCRPtr.h                                                       $
 *
 * $Revision:: 18                                                             $
 *
 * $Archive:: /RetailControls/Rel.3_11/OPOS/include/NCR/NCRPtr.h              $
 *
 * $Date:: 6/26/12 1:24a                                                      $
 *
 ******************************************************************************
 *
 * Copyright (c) NCR Corporation 2004.  All rights reserved.
 *
 ******************************************************************************
 *
 * $History:: NCRPtr.h                                                        $
 * 
 * *****************  Version 18  *****************
 * User: Barlom       Date: 6/26/12    Time: 1:24a
 * Updated in $/RetailControls/Rel.3_11/OPOS/include/NCR
 * Checked in additional code for QR barcode which merges changes from Ginah
 * Colon, Sagar Vidya, and Sai Ponnuru.
 * TAR # 474308
 * 
 * *****************  Version 17  *****************
 * User: Barlom       Date: 6/20/12    Time: 9:56p
 * Updated in $/RetailControls/Rel.3_11/OPOS/include/NCR
 * Initial support for printing QR barcodes. Status from printer not yet
 * implemented. 
 * TAR # 474308
 * 
 * *****************  Version 15  *****************
 * User: Standw       Date: 4/01/08    Time: 2:23p
 * Updated in $/RetailControls/Rel.3_9/OPOS/include/NCR
 * TAR 372269: Add support for two sided thermal paper detection
 * 
 * *****************  Version 13  *****************
 * User: Standw       Date: 3/09/07    Time: 2:20p
 * Updated in $/RetailControls/Rel.3_8/OPOS/include/NCR
 * Updated to support two sided thermal printing.
 * 
 * *****************  Version 10  *****************
 * User: Standw       Date: 2/07/06    Time: 4:35p
 * Updated in $/RetailControls/Rel.3_6/OPOS/include/NCR
 * Added the NCRDIO_PTR_SLIP_CHANGE_SIDE DirectIO, primarily for use with the
 * Epson H6000 family of printers.  For convenience, the Epson constant
 * PTR_DI_SLIP_CHANGE_SIDE is also defined, using the same value as
 * NCRDIO_PTR_SLIP_CHANGE_SIDE.
 * 
 * *****************  Version 7  *****************
 * User: Standw       Date: 3/16/05    Time: 4:19p
 * Updated in $/RetailControls/Rel.3_4/OPOS/include/NCR
 * Clarified comment block for NCRDIO_PTR_SET_MINIMUM_REC_LEN DirectIO.
 * 
 * *****************  Version 6  *****************
 * User: Standw       Date: 3/16/05    Time: 3:24p
 * Updated in $/RetailControls/Rel.3_4/OPOS/include/NCR
 * Removed PTRSTAT_REC_DROPPED status; this status applies only to the K585,
 * which is not supported.
 * 
 * *****************  Version 5  *****************
 * User: Barlom       Date: 2/25/05    Time: 2:17p
 * Updated in $/RetailControls/Rel.3_4/OPOS/include/NCR
 * Added Check Scanner specific status and parameters
 * 
 * *****************  Version 4  *****************
 * User: Barlom       Date: 2/25/05    Time: 1:53p
 * Updated in $/RetailControls/Rel.3_4/OPOS/include/NCR
 * Merged in Direct I/O for Check Scanner
 * 
 * *****************  Version 3  *****************
 * User: Standw       Date: 2/25/05    Time: 1:20p
 * Updated in $/RetailControls/Rel.3_4/OPOS/include/NCR
 * Added kiosk specific DirectIOs and constants.
 * 
 * *****************  Version 1  *****************
 * User: Spohnb       Date: 1/23/04    Time: 11:33a
 * Created in $/RetailControls/Rel.3_3/OPOS/include/NCR
 * Initial version.
 *
 *****************************************************************************/

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// NCR POS Printer value-add extensions via DirectIO.
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
//--- POSPrinter DirectIO ---
//---------------------------------------------------------------------------

//** SetBitmapType **  Set the bitmap type for bitmap printing.
//                     (See details below in "Bitmap Printing Types".)
//    Data:       In > New bitmap type value.
//                Out> Previous bitmap type value.
//    String:     (Not used.)
//    ResultCode: OPOS_SUCCESS   = Set successfully.
//                OPOS_E_ILLEGAL = Bad bitmap type.
const LONG NCRDIO_PTR_SET_BITMAP_TYPE = 101;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//** SetBarcodeWidth **  Set the minimum width of barcode bars.
//    Data:       In > New minimum bar width in dots.
//                Out> Previous width.
//    String:     (Not used.)
//    ResultCode: OPOS_SUCCESS   = Set successfully.
//                OPOS_E_ILLEGAL = Bad width.
const LONG NCRDIO_PTR_SET_BARCODE_WIDTH = 102;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//** SoundAudibleTone **  Generate an audible tone from the printer.
//    Data:       In > Repeat count (range 1 - 20).
//    String:     (Not used.)
//    ResultCode: OPOS_SUCCESS   = Successful.
//                OPOS_E_ILLEGAL = Printer cannot generate sounds.
//                -other-        = Error occurred, such as printer not claimed
//                                   or enabled, or printer off.
const LONG NCRDIO_PTR_SOUND_AUDIBLE_TONE = 103;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//** SetMinLineCharsMode **  Set the mode for the printer station so that it
//                           supports at least this number of characters.
//                           When XxxLineChars is set, the mode will be set
//                           to the minimum of the XxxLineChars value and this
//                           value.
//    Data:       In > High Word (b.31-16) = Station (PTR_S_...).
//                     Low  Word (b.15-00) = Minimum line character value.
//                       Set to zero for default handling.
//                Out> Previous value (if printer claimed & enabled and station
//                       is valid).
//    String:     (Not used.)
//    ResultCode: OPOS_SUCCESS   = Successful.
//                OPOS_E_ILLEGAL = Printer station cannot support this value.
//                -other-        = Error occurred, such as printer not claimed
//                                   or enabled, or bad station.
const LONG NCRDIO_PTR_SET_MIN_LINE_CHARS_MODE = 104;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//** GetPrinterStatus **  Get the printer status.
//    Data:       Out> Current printer status.
//    String:     (Not used.)
//    ResultCode: OPOS_SUCCESS   = Successful.
//                -other-        = Printer not claimed or enabled.
const LONG NCRDIO_PTR_GET_PRINTER_STATUS = 105;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//** RawOutput **  Pass application data directly to the printer with
//                       no parsing or modification.
//    Data:       In > Station for output.
//    String:     In > Data to send to printer.
//    ResultCode: OPOS_SUCCESS   = Set successfully.
//                Other values   = See ResultCode property.

const LONG NCRDIO_PTR_RAW_OUTPUT = 106;

//** SlipFlip **  Initiate a Slip Flipping Sequence on the Printer.
//                    This tell the printer to active the flip mechanism.   
//    Data:       In > Not Used
//    String:     In > Not Used
//    ResultCode: OPOS_SUCCESS   = Slip Flip Sequence was successful.
//                Other values   = See ResultCode property.
const LONG NCRDIO_PTR_SLIP_FLIP = 107;				//Value Reserved for OPOS 2.x compatibility

//#if defined( JAPAN )	// 12/98 - This directive can be deleted.
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//** SetUserDefinedKanjiChar **  Set the User-Defined Kanji character.
//    Data:       In > High Word (b.31-16) = Station (PTR_S_...).
//                     Low  Word (b.15-00) = character codes.
//                         FFxx -> character codes for the defined characters
//                                   for the first byte.
//                         xxFF -> character codes for the defined characters
//                                   for the second byte.
//    String:     In > Data to send to printer.
//    ResultCode: OPOS_SUCCESS   = Successful.
//                OPOS_E_ILLEGAL = Printer does not support.
//                -other-        = Error occurred, such as printer not claimed
//                                   or enabled, or printer off.
const LONG NCRDIO_PTR_SET_UD_KANJI_CHAR = 108;
//#endif				// 12/98 - This directive can be deleted.

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//** SensorStatus **  Check one of the proprietary sensors for its current value
//
//    Data:       In > Parameter to select sensor to report.
//                Out> Value indicating the current state of the sensor.
//    String:     Not used
//    ResultCode: OPOS_SUCCESS   = Returned current value for sensor in Data.
//                OPOS_E_ILLEGAL = Specified Sensor not support by printer.
const LONG NCRDIO_PTR_SENSOR_STATUS = 109;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//** AutoEjectTimeout **  Set the Auto Eject Time-Out value in the 7401-K580 or K590.
//                   This time-out is the delay between the time a receipt is
//					 presented, and it is auto ejected onto the floor.   
//    Data:       In > Time-Out period
//				  OPOS_FOREVER - Infinite no time-out
//				  One of 20, 40, 60, or 80 seconds in decimal.
//    String:     Not Used
//    ResultCode: OPOS_SUCCESS   = Time-out value set to specified value.
//                OPOS_E_ILLEGAL = An invalid time-out period value was passed.
const LONG NCRDIO_PTR_AUTOEJECT_TIMEOUT = 110;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//** EjectReceipt **  Initiate a Receipt Eject Sequence from a 7401-K580 or K590
//                    printer.  Ejects Receipt from the presenter mechanism.   
//    Data:       Not Used
//    String:     Not Used
//    ResultCode: OPOS_SUCCESS   = Eject successfully transmitted to printer.
//                Other values   = See ResultCode property.
const LONG NCRDIO_PTR_EJECT_RECEIPT = 111;

// Add 7196-4205/3205 - Start

//** 7196SetStamp  Set stamp data for 7196-4205/3205.
//    Data:       (Not used.)
//    String:     In > Path of a bitmap file which represents stamp
//    ResultCode: OPOS_SUCCESS   = Set successfully.
//                OPOS_E_ILLEGAL = Bad path or not a 416 x 160 mono color BMP file.
//                OPOS_E_FAILURE = not a 416 x 160 mono color BMP file
const LONG NCRDIO_PTR_7196_SET_STAMP = 112;			//Value Reserved for OPOS 2.x compatibility

// Add 7196-4205/3205 - End

//** RawOutIn **  Pass application data directly to the printer with
//                       no parsing or modification.
//    Data:       In/Out > In:Station for output, Out:Lentgh of the Response in bytes
//    String:     In/out > Data to send to printer, Out:Data to received from printer
//    ResultCode: OPOS_SUCCESS   = Set successfully.
//                Other values   = See ResultCode property.
const LONG NCRDIO_PTR_RAW_OUTIN = 114;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//** GetKioskStatus **  Get the Kiosk specific status bits.
//    Data:       Out> Current kiosk status bits.
//    String:     Not used
//    ResultCode: OPOS_SUCCESS   = Successful.
//                -other-        = Printer not claimed or enabled.
const LONG NCRDIO_PTR_GET_KIOSK_STATUS = 116;


//EuroSymbol
const LONG  NCRDIO_PTR_SET_USERD_CHAR  = 118;		//Value Reserved for OPOS 2.x compatibility


// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//** 2STPaperType **  Return current 2ST paper type if known
//
//    Data:       Out> Current paper type
//						      PTR_2ST_PAPER_UNKNOWN -- Paper Type is not yet determined
//						      PTR_2ST_PAPER_SINGLE -- Single Sided paper roll installed
//						      PTR_2ST_DOUBLE -- Double Sided paper roll installed
//    String:     In > (Not used.)
//    ResultCode: OPOS_SUCCESS   = Returned current paper type or unknown
//                OPOS_E_ILLEGAL = The printer does not support 2ST Paper Type or
//									the functionality is not enabled.
const LONG NCRDIO_PTR_2ST_PAPER_TYPE = 122;
//
//  Parameter constants are:
//
//  -- values indicating paper type --
//
const LONG PTR_2ST_PAPER_UNKNOWN      = 0x00000000;
const LONG PTR_2ST_PAPER_SINGLE       = 0x00000001;
const LONG PTR_2ST_PAPER_DOUBLE       = 0x00000002;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//** SetPaperLow **   Send Paper Low Sensor Configuration Command to K590 printer.   
//    Data:       In > Paper Low Sensor Usage
//                     0 - Paper Low Detection Set for 50ft
//					   1 - Paper Low Detection Set for 10ft
//					   2 - Paper Low Disabled
//    String:     Not Used
//    ResultCode: OPOS_SUCCESS   = Paper Low Option Successfully sent to printer.
//                OPOS_E_ILLEGAL = Command not valid for current printer, or
//                                 parameter value out of range.
//                Other values   = See ResultCode property.
const LONG  NCRDIO_PTR_SET_PAPER_LOW   = 126;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//** SetBlackMark **   Send Black Mark Configuration Command to K590 or F306 printer.
//    Data:       In > Black Mark Option Usage
//                     0 - Document Mode (Ignore Black Mark)
//					   1 - Normal Mode (Cut at first black mark beyond printing)
//					   2 - Advance Mode (Cut at 2nd black mark beyond printing)
//    String:     Not Used
//    ResultCode: OPOS_SUCCESS   = Black Mark Option Successfully sent to printer.
//                OPOS_E_ILLEGAL = Command not valid for current printer, or
//                                 parameter value out of range.
//                Other values   = See ResultCode property.
const LONG  NCRDIO_PTR_SET_BLACK_MARK  = 127;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//** SetMinimumReceiptLength **   Send command to set minimum receipt length to K590, F306.
//    Data:       In > Minimum Receipt Length Option
//                     K590 Values              F306 Values
//                     1: 90mm                  0: None
//					   2: 111mm                 1: 6 inches
//					   3: 150mm
//					   4: 225mm
//					   5: 300mm
//					   6: 375mm
//					   7: 450mm
//					   8: 525mm
//					   9: 600mm
//    String:     Not Used
//    ResultCode: OPOS_SUCCESS   = New Minimum Receipt Length sent to the printer
//									if the m_bUseSoftOptions flag is TRUE.
//                OPOS_E_ILLEGAL = Command not valid for current printer, or
//                                 parameter value out of range.
//                Other values   = See ResultCode property.
const LONG	NCRDIO_PTR_SET_MINIMUM_REC_LEN = 130;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//** SetReceiptRetainLength **   Send command to set receipt retain length to K590 printer.   
//    Data:       In > Receipt Retain Length Option
//                     1 - 25mm
//					   2 - 50mm
//					   3 - 75mm
//    String:     Not Used
//    ResultCode: OPOS_SUCCESS   = New Receipt Retain Length sent to the printer
//									if the m_bUseSoftOptions flag is TRUE.
//                OPOS_E_ILLEGAL = Command not valid for current printer, or
//                                 parameter value out of range.
//                Other values   = See ResultCode property.
const LONG	NCRDIO_PTR_SET_REC_RETAIN_LEN = 131;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//** SlipChangeSide **  Changes the print side for future slip print requests.
//                      The UnifiedPOS 1.5 specification added the ChangePrintSide
//                      method, which is the preferred method for changing the print
//                      side for the slip.
//                      An additional constant, PTR_DI_SLIP_CHANGE_SIDE, is provided
//                      for applications using the Epson DirectIO call.
//    Data:       Not Used
//    String:     Not Used
//    ResultCode: OPOS_SUCCESS   = Successful.
//                Other values   = See ResultCode property.
const LONG	NCRDIO_PTR_SLIP_CHANGE_SIDE = 121;
const LONG	PTR_DI_SLIP_CHANGE_SIDE = NCRDIO_PTR_SLIP_CHANGE_SIDE;

//** SetColorPaper **   Send command to set the type of Color Thermal paper in the printer.   
//    Data:       In > Color Paper Type
//                     1 - Monochrome
//					   2 - Two Color
//    String:     In > Not Used
//    ResultCode: OPOS_SUCCESS   = New Color Paper type sent to the printer
//                OPOS_E_ILLEGAL = Command not valid for current printer, or
//                                 parameter value out of range.
//                Other values   = See ResultCode property.
const LONG	NCRDIO_PTR_SET_COLOR_PAPER = 132;			//Value Reserved for OPOS 2.x compatibility


// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//** SetRegParam **  Set or change a parameter that may be initialized by
//                   the system registry.
//    Data:       In > ParameterConstant + New value.
//                Out> Previous value.
//    String:     (Not used.)
//    ResultCode: OPOS_SUCCESS   = Set successfully.
//                OPOS_E_ILLEGAL = Bad parameter constant or value.
const LONG NCRDIO_PTR_SET_REG_PARAM = 151;			//Value Reserved for OPOS 2.x compatibility


//TAR 474308 QR Barcodes
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
//** QRModel **   Override the registry setting for QRModel with parameter   
//    Data:       In > Model Number
//                     PTR_PARAM_QR_MODEL_1 
//					   PTR_PARAM_QR_MODEL_2
//
//    String:     In > Not Used
//    ResultCode: OPOS_SUCCESS   = Successfully set the active QRModel value.
//                OPOS_E_ILLEGAL = Command not valid for current printer, or
//                                 parameter value out of range.
//                Other values   = See ResultCode property.

const LONG  NCRDIO_PTR_QR_MODEL   = 140;

const LONG	PTR_PARAM_QR_MODEL_1  = 1;
const LONG	PTR_PARAM_QR_MODEL_2  = 2;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
//** QRErrLevel **   Override the registry setting for QR barcode error correction level with parameter   
//    Data:       In > Model Number
//                     PTR_PARAM_QR_ERR_LVL_L	1 
//					           PTR_PARAM_QR_ERR_LVL_M	2
//					           PTR_PARAM_QR_ERR_LVL_Q	3
//					           PTR_PARAM_QR_ERR_LVL_H	4
//
//    String:     In > Not Used
//    ResultCode: OPOS_SUCCESS   = Successfully set the active QR barcode error correction level.
//                OPOS_E_ILLEGAL = Command not valid for current printer, or
//                                 parameter value out of range.
//                Other values   = See ResultCode property.

const LONG  NCRDIO_PTR_QR_ERROR_LEVEL  = 141;

const LONG	PTR_PARAM_QR_ERR_LVL_L  = 1;
const LONG	PTR_PARAM_QR_ERR_LVL_M  = 2;
const LONG	PTR_PARAM_QR_ERR_LVL_Q	= 3;
const LONG	PTR_PARAM_QR_ERR_LVL_H	= 4;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
//** QRSize **   Override the registry setting for QRSize with parameter   
//    Data:       In > Model Number
//                     PTR_PARAM_QR_SIZE_DEFAULT  0 ( Indicates to attempt to calculate size form height parameter ) 
//					   1 - 16 = Specifies a specific size to use
//
//    String:     In > Not Used
//    ResultCode: OPOS_SUCCESS   = Successfully set the active QRModel value.
//                OPOS_E_ILLEGAL = Command not valid for current printer, or
//                                 parameter value out of range.
//                Other values   = See ResultCode property.

const LONG  NCRDIO_PTR_QR_SIZE   = 142;

const LONG	PTR_PARAM_QR_SIZE_DEFAULT  = 0;
const LONG	PTR_PARAM_QR_SIZE_MIN = 1;
const LONG	PTR_PARAM_QR_SIZE_MAX = 16;

// END TAR 474308 QR Barcodes

// RPSUPOS-967 Start
// DirectIO to Enable/Disable the Thai font diacritical character space compensation
//
// ** Enable/Disable Service Object inserting spaces to compensate for diacritical characters no longer taking up space
//
//    Data:       In > Enable/Disable
//                     PTR_PARAM_SP_COMP_DISABLE - 0
//					   PTR_PARAM_SP_COMP_ENABLE - 1
//
//    String:     In > Not Used
//    ResultCode: OPOS_SUCCESS   = Successfully Enable/Disable the Thai font diacritical character space compensation value.
//                OPOS_E_ILLEGAL = Command not valid for current printer, or
//                                 parameter value out of range.
//                Other values   = See ResultCode property.
const LONG NCRDIO_PTR_THAI_SP_COMP = 145;

const LONG PTR_PARAM_SP_COMP_DISABLE = 0;
const LONG PTR_PARAM_SP_COMP_ENABLE = 1;
// RPSUPOS-967 End

//SSCO 6.0 Updates - Start

// DirectIO to Enable the Partial Cut for SSCO 6.0 Printers
//
// ** Enable/Disable Service Object inserting spaces to compensate for diacritical characters no longer taking up space
//
//    Data:       In > Not Used
//
//    String:     In > Not Used
//    ResultCode: OPOS_SUCCESS   = Successfully Enabled the Partial Cut Mechanism.
//                OPOS_E_ILLEGAL = Command not valid for current printer, or
//                                 parameter value out of range.
//                Other values   = See ResultCode property.
const LONG NCRDIO_PTR_ENABLE_PARTIALCUT = 146;

// DirectIO to sends a Bezel LED control Request to SSCO 6.0 Printers
//
//
//    Data:       In > Single byte value from 1-32
//
//    String:     In > Not Used
//    ResultCode: OPOS_SUCCESS   = Successfully Enabled the Partial Cut Mechanism.
//                OPOS_E_ILLEGAL = Command not valid for current printer, or
//                                 parameter value out of range.
//                Other values   = See ResultCode property.
const LONG NCRDIO_PTR_BEZEL_CONTROL_REQUEST = 147;

// DirectIO to configures the Bezel LED Mode for SSCO 6.0 Printers
//
//
//    Data:       In > Single byte value either 0 or 1
//
//    String:     In > Not Used
//    ResultCode: OPOS_SUCCESS   = Successfully Enabled the Partial Cut Mechanism.
//                OPOS_E_ILLEGAL = Command not valid for current printer, or
//                                 parameter value out of range.
//                Other values   = See ResultCode property.
const LONG NCRDIO_PTR_SET_BEZEL_LED_MODE = 148;
//End SSCO 6.0 Updates

//RPSUPOS-1602, 7199 Printer Development
// DirectIO to Print the Bitmap in GrayScale
//
//
//    Data:       In > Station
//                     PTR_S_JOURNAL - 0	(Not Supported)
//					   PTR_S_RECEIPT - 2
//					   PTR_S_SLIP	 - 4
//
//    String:     In > Path of the Colored Bitmap to Print in GrayScale
//    ResultCode: OPOS_SUCCESS   = Successfully set the active QRModel value.
//                OPOS_E_ILLEGAL = Command not valid for current printer, or
//                                 parameter value out of range.
//                Other values   = See ResultCode property.
const LONG NCRDIO_PTR_GS_BITMAP_PRINT = 149;


// ** Enable Watermark Printing for 7199 Printer
//
//    Data:       In > Alignment
//                     Left Alignment									- 0
//					   Center Alignment									- 1
//					   Right Alignment									- 2
//					   Roll1(Left -> Center -> Right -> Left -> ...)	- 3
//					   Roll2(Right -> Center -> Left -> Right -> ...)	- 4
//					   Twist(Left -> Center -> Right -> Center -> Left -> ...)	- 5
//
//    String:     In > Path of the Colored Bitmap to Print in GrayScale
//    ResultCode: OPOS_SUCCESS   = Successfully set the active QRModel value.
//                OPOS_E_ILLEGAL = Command not valid for current printer, or
//                                 parameter value out of range.
//                Other values   = See ResultCode property.
const LONG NCRDIO_PTR_ENABLE_WATERMARK_PRINTING	= 150;

// ** Disable Watermark Printing for 7199 Printer
//
//    Data:       In > Station
//                     PTR_S_JOURNAL - 0	(Not Supported)
//					   PTR_S_RECEIPT - 2
//					   PTR_S_SLIP	 - 4
//
//    String:     In > Not Used
//    ResultCode: OPOS_SUCCESS   = Successfully set the active QRModel value.
//                OPOS_E_ILLEGAL = Command not valid for current printer, or
//                                 parameter value out of range.
//                Other values   = See ResultCode property.
const LONG NCRDIO_PTR_DISABLE_WATERMARK_PRINTING	= 151;

//End RPSUPOS-1602, 7199 Printer Development

//---------------------------------------------------------------------------
//--- Check Scanner DirectIO ---
//---------------------------------------------------------------------------

//** SetThresholdLevel **  Set the contrast level that pics up image
//
//    Data:       In > Single byte value from -128..0..127
//    String:     (Not used.)
//    ResultCode: OPOS_SUCCESS   = Set successfully.
//                OPOS_E_ILLEGAL = Invalid Parameter
//                Other values   = See ResultCode property.
const LONG NCRDIO_CHKSCAN_SET_THRESHOLD_LEVEL = 301;

//** ReadThresholdLevel **  Reads from the printer contrast level
//
//    Data:       Out > Single byte value from -128..0..127
//    String:     (Not used.)
//    ResultCode: OPOS_SUCCESS   = Set successfully.
//                Other values   = See ResultCode property.
const LONG NCRDIO_CHKSCAN_READ_THRESHOLD_LEVEL = 302;


//---------------------------------------------------------------------------
//--- Common Device DirectIO ---
//---------------------------------------------------------------------------

//** GetCCLData **  Obtain device information such as Class-Model & Serial #
//
//    Data:       Out > Number of items contained in the string
//    String:     Out > String of items separated by LF
//    ResultCode: OPOS_SUCCESS   = Set successfully.
//                Other values   = See ResultCode property.
const LONG NCRDIO_DEVICE_GET_CCL_DATA = 401;

//---------------------------------------------------------------------------
//--- POSPrinter DirectIO Events ---
//---------------------------------------------------------------------------

// NCR_PTR_DIOE_SENSOR_UPDATE: Proprietary Sensor Change Event
//      Data:       The new sensor state.  This will be one of the sensor state
//                  values listed below.
//      String:     Not used
//
const LONG NCR_PTR_DIOE_SENSOR_UPDATE				= 0x00000001;

// NCR_PTR_DIOE_2ST_PAPER_SENSOR: 2ST Paper Type Event
//      Data:       The new paper type.  This will be one of these paper type
//                  values:
//                      PTR_2ST_PAPER_UNKNOWN
//                      PTR_2ST_PAPER_SINGLE
//                      PTR_2ST_PAPER_DOUBLE
//      String:     Not used
//
const LONG NCR_PTR_DIOE_2ST_PAPER_SENSOR			= 0x00000010;


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// PrintBitmap extension for parameter "Width"
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
//   - If the OPOS constant PTR_BM_ASIS is specified, then the bitmap is printed
//       using the number of printer _dots_ given in the bitmap.  The bitmap may
//       still be transformed for the lower resolutions (as in "single density").
//   - If the NCR constant NCR_PTR_BM_ASIS is specified, then the bitmap is printed
//       using the number of printer _bits_ given in the bitmap.  The bitmap bits
//       are sent as printer bit image bits one-for-one, and the size of the result
//       will vary from one resolution to another.

const LONG NCR_PTR_BM_ASIS = -21;


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Bitmap Printing Types.
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
//   Specifies the quality and method of bitmap printing.
//
//   - If a specific type is specified, then it will be used.
//      - If PTR_BM_ASIS or NCR_PTR_BM_ASIS: If not too big, then print.
//          Else an error is returned.
//      - Else: If the specified width is less or equal to the maximum
//          allowed by this type with the current metrics (including
//          linechars), then transform the bitamp to the specified width
//          and print.
//          Else an error is returned.
//
//   - Default will change based on the LetterQuality flag for the station.
//      - If True: Uses the high quality download type if available, else HQ direct.
//        If False: Uses the low quality download type if available, else LQ direct.
//      - If PTR_BM_ASIS or NCR_PTR_BM_ASIS: If not too big, then print.
//          Else try the "next type". If no next, an error is returned.
//            Ordering of next:
//              LQ download -> LQ direct -> HQ download -> HQ direct.
//      - Else: If the specified width is less or equal to the maximum
//          allowed by this type with the current metrics (including
//          linechars), then transform the bitamp to the specified width
//          and print.
//          Else try the "next type". If no next, an error is returned.
//            Ordering of next:
//              LQ download -> LQ direct
//              HQ download -> HQ direct.

const char NCRDIO_BITMAPTYPE_DEFAULT   = 0;

// Direct printing types: Does not download into printer first.
//   Usually causes overall higher throughput, but perceived throughput
//   may be lower, plus print quality may suffer due to banding at the
//   end of each line.
const BYTE NCRDIO_BITMAPTYPE_HIGHQ     = 0x11; // HQ.
const BYTE NCRDIO_BITMAPTYPE_LOWQ      = 0x12; // LQ.

// Download printing types.
const BYTE NCRDIO_BITMAPTYPE_HIGHQ_DL  = 0x21; // HQ.
const BYTE NCRDIO_BITMAPTYPE_LOWQ_DL   = 0x22; // LQ.


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Printer Status values:
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// NCR-specific status bit flags returned from the DirectIO Command Get
// Printer Status.
//
// The following device status constants define those bits:
//

const int PTRSTAT_FLAG          = 0x80000000; // (Always on.)

const int PTRSTAT_ALL           = 0x3FFFFFFF; // All statuses.
const int PTRSTAT_STATE         = 0x00000003; // All state flags.
const int PTRSTAT_PRINTER       = 0x0FFFFFF0; // All printer statuses.
const int PTRSTAT_PRINTER_FAULT = 0x0FF00000; // All printer faults/failures.
const int PTRSTAT_DRAWER        = 0x30000000; // All drawer statuses.

// Printer states.
const int PTRSTAT_OFF_STATE     = 0x00000001; // Printer is off.
const int PTRSTAT_ERROR_STATE   = 0x00000002; // Printer is in error: Not accepting requests.

// OPOS printer sensor values.
const int PTRSTAT_COVER_OPEN    = 0x00000010; // Cover or door open.
const int PTRSTAT_JRN_EMPTY     = 0x00000040; // Journal out.
const int PTRSTAT_JRN_NEAREND   = 0x00000080; // Journal low.
const int PTRSTAT_REC_EMPTY     = 0x00000100; // Receipt out.
const int PTRSTAT_REC_NEAREND   = 0x00000200; // Receipt low.
const int PTRSTAT_SLP_EMPTY     = 0x00000400; // Slip leading edge sensor: no paper.
const int PTRSTAT_SLP_NEAREND   = 0x00000800; // Slip trailing edge sensor: no paper.

// Printer slip sensor values: May not match OPOS values due to differences in
//   what the printer reports.
const int PTRSTAT_SLP_TOF       = 0x00001000; // Slip top of form sensor on.
const int PTRSTAT_SLP_BOF       = 0x00002000; // Slip bottom of form sensor on.

// Printer states only available when real-time commands are supported.
const int PTRSTAT_SLP_SELECTED  = 0x00010000; // Slip selected.
const int PTRSTAT_SLP_WAITING   = 0x00020000; // Printer waiting for insertion.

// Printer failure conditions. Support varies by printer.
const int PTRSTAT_AUTORECOVER_FAULT           // Auto-recoverable fault, such as
                                = 0x00100000; //   temperature fault.
const int PTRSTAT_MOTOR_FAULT   = 0x00200000; // Motor fault.
const int PTRSTAT_KNIFE_FAULT   = 0x00400000; // Knife fault.
const int PTRSTAT_FAULT         = 0x00800000; // General fault.
const int PTRSTAT_FAILURE       = 0x01000000; // Unrecoverable error.

// Drawer states.
const int PTRSTAT_DWR1_OPEN     = 0x10000000; // Drawer 1 open.
const int PTRSTAT_DWR2_OPEN     = 0x20000000; // Drawer 2 open.


const int PTRSTAT_DWR_SHIFT     = 27;         // Bit shift count to get drawer statuses to
                                              //   low bits: Bit 1 for drawer 1; 2 for 2.

// Self-Service Printer specific device status values
const int PTRSTAT_PPS_EMPTY     = 0x00000001; // Receipt Presenter Paper Sensor
const int PTRSTAT_PO_ROTATED    = 0x00000002; // Receipt Presenter Mechanism Orientation
const int PTRSTAT_CPC_EMPTY     = 0x00000004; // Kiosk Paper Chute
const int PTRSTAT_PTR_RESET     = 0x00000008; // Reset Button Pressed
const int PTRSTAT_PRESENTER_ERR = 0x00000010; // Receipt Presenter Error
const int PTRSTAT_PAPER_JAM     = 0x00000020; // Paper failed to feed properly Jam assumed
const int PTRSTAT_CDS_OPEN		= 0x00000040; // Kiosk Cabinet Door Open
const int PTRSTAT_BMD_FAILURE   = 0x00000080; // Black mark detect failure
const int PTRSTAT_HEAD_FAILURE  = 0x00000100; // Thermal Print Head Mechanism Failure
const int PTRSTAT_KNIFE_CUT     = 0x80000000; // A Knife Cut has been issued.


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// NCR Specific Extended Result Code Printer Status Values:
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
const LONG NCR_EPTR = 10000;

// Self Service Printer Errors:

//		Receipt Presenter not ready to start next transaction.
const LONG NCR_EPTR_RPM_NOT_READY		= 154 + NCR_EPTR;
//		Cabinet door on Kiosk open unable to print.
const LONG NCR_EPTR_CDS_DOOR_OPEN		= 155 + NCR_EPTR;
//		Presenter Mechanism Error
const LONG NCR_EPTR_RPM_MECH_ERROR		= 156 + NCR_EPTR;
//		Presenter Paper Jam
const LONG NCR_EPTR_RPM_PAPER_JAM		= 157 + NCR_EPTR;
//		Presenter Black Mark Failure
const LONG NCR_EPTR_RPM_BLACK_MARK		= 158 + NCR_EPTR;
//		Print Head Failure
const LONG NCR_EPTR_PRINT_HEAD_FAILURE  = 159 + NCR_EPTR;


// - 2 Sided Thermal Errors

//      Operation only allowed in Transaction Mode if in a 2ST Mode
const LONG NCR_EPTR_2ST_MODE            = 170 + NCR_EPTR;

//  TAR 474308: support for QRBarcode Validation

// If "ResultCodeExtended" bitwise ANDed (&) with PTRSTAT_FLAG is zero
//   (that is, the bit PTRSTAT_FLAG is off), then "ResultCodeExtended" contains
//   an NCR-specific error status to clarify the error.

// - QR Barcode Validation Error Values

const LONG NCR_EPTR_QR_ERRLVL_NOTVALID		  = 180 + NCR_EPTR;
const LONG NCR_EPTR_QR_INVALID_BARCODE_DATA	= 181 + NCR_EPTR;
const LONG NCR_EPTR_QR_TOO_MANY_CHARACTERS	= 182 + NCR_EPTR;


//  TAR 474308: End support for QRBarcode Validation


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// Proprietary Sensor Values:
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// Identifiers and status values status for NCR proprietary sensors,
// used with NCRDIO_PTR_SENSOR_STATUS Direct IO.
//

//  Sensor identifiers
// These are the possible parameter constants used with NCRDIO_PTR_SENSOR_STATUS
const LONG PTR_Cabinet_Door_Switch      = 0x00100000;
const LONG PTR_Presenter_Paper_Sensor   = 0x00200000;
const LONG PTR_Cabinet_Paper_Chute      = 0x00400000;
const LONG PTR_Presenter_Orientation    = 0x00800000;

// Sensor state values
// These are the values returned by NCRDIO_PTR_SENSOR_STATUS and
// NCR_PTR_DIOE_SENSOR_UPDATE.
const LONG PTR_CDS_Door_Open        = 0x00100001; // Cabinet door switch open
const LONG PTR_CDS_Door_Closed		= 0x00100002; // Cabinet door switch closed
const LONG PTR_PPS_Paper_Present    = 0x00200001; // Presenter paper present
const LONG PTR_PPS_No_Paper         = 0x00200002; // Presenter paper out
const LONG PTR_CPC_Paper_Present    = 0x00400001; // Cabinet paper chute: Paper present
const LONG PTR_CPC_No_Paper         = 0x00400002; // Cabinet paper chute: No paper
const LONG PTR_PO_Rotated_90        = 0x00800001; // Presenter rotated
const LONG PTR_PO_Normal            = 0x00800002; // Presenter not rotated (normal)

//---------------------------------------------------------------------------
//--- Check Scanner Statuses ---
//---------------------------------------------------------------------------

// - Scan Check data errors:
//	Line error during read: Parity, overrun, framing.
const LONG NCR_ECHK_LINE_ERROR			 = 301 + NCR_EPTR;
// - Check Scanner Cover Open
const LONG NCR_ECHK_COVER_OPEN			 = 302 + NCR_EPTR;


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// DocumentHeight extension for Check Scanner Height to perform AutoSize on
//   the Height of a document.
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
const LONG NCR_CHK_DOC_HEIGHT_AUTOSIZE = -1; // Added for RFC TAR # 281958