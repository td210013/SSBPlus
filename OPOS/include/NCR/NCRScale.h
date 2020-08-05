/**
 * NCRScan.h declares the constants needed to make use of the NCR Scale
 * OPOS extensions.
 */
/******************************************************************************
 *
 * $Workfile:: NCRScale.h                                                     $
 *
 * $Revision:: 1                                                              $
 *
 * $Archive:: /RetailControls/Rel.3_3/OPOS/include/NCR/NCRScale.h             $
 *
 * $Date:: 5/13/04 4:31p                                                      $
 *
 ******************************************************************************
 *
 * Copyright (c) NCR Corporation 2004.  All rights reserved.
 *
 ******************************************************************************
 *
 * $History:: NCRScale.h                                                      $
 * 
 * *****************  Version 1  *****************
 * User: Mr160002     Date: 5/13/04    Time: 4:31p
 * Created in $/RetailControls/Rel.3_3/OPOS/include/NCR
 *
 *
 *****************************************************************************/

/////////////////////////////////////////////////////////////////////
// "DirectIO" Method Constants for Scale (NCR 787x and 788x)
/////////////////////////////////////////////////////////////////////

// No Longer supported ** const long NCRDIO_SCAL_WEIGHT_DELAY     = 606;

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// NCR Scale value-add extensions via DirectIO.
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// "DirectIO" Method Constants for Scale (NCR 787x, 788x, and 7892)
// Not supported by 783x Scales.
/////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------

//** Scale Status Request(601) **  Returns Scale Status.
//    Data:       In > (Not Used).
//                Out> (Not Used).
//    String:     Status string (direct from Scale). You must know the Scale interface to decode the string.
//    ResultCode: OPOS_SUCCESS   = Set successfully.
//                OPOS_E_ILLEGAL = Invalid command.
const long NCRDIO_SCAL_STATUS           = 601;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//** Scale Read ROM Request(602) **  Returns ROM data.
//    Data:       In > Address to read from.
//                Out> (Not Used).
//    String:     Returns 30 bytes of ROM data
//    ResultCode: OPOS_SUCCESS   = Set successfully.
//                OPOS_E_ILLEGAL = Invalid command.
const long NCRDIO_SCAL_READROM          = 602;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//** Scale Read ROM Version Request(603) **  Returns ROM version.
//    Data:       In > (Not Used).
//                Out> (Not Used).
//    String:     Returns the ROM version ID
//    ResultCode: OPOS_SUCCESS   = Set successfully.
//                OPOS_E_ILLEGAL = Invalid command.
const long NCRDIO_SCAL_ROM_VERSION      = 603;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//** Scale Live Weight(604) **  Returns weight for display or scale status.
//    Data:       In > (Not Used).
//                Out> The return value is the status of the Scale Monitor command or the weight.
//					   You must know the Scale interface to decode the string.from the Scale.
//    String:     (Not Used).
//    ResultCode: OPOS_SUCCESS   = Set successfully.
//                OPOS_E_ILLEGAL = Invalid command.
const long NCRDIO_SCAL_LIVE_WEIGHT      = 604;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .


//** Scale Direct Access(605) **  Sends data directly to the Scale.
//    Data:       In > True (1) if data is to be returned from the Scale, and False (0) if no data is to be returned.
//                Out> (Not Used).
//
//    String:     In > String to send to the Scale, including the suffix and BCC character (if needed).
//				  Out> The string data returned (direct from Scale).
//					   You must know the Scale interface to decode the string.from the Scale.
//    ResultCode: OPOS_SUCCESS   = Set successfully.
//                OPOS_E_ILLEGAL = Invalid command.
const long NCRDIO_SCAL_DIRECT           = 605;
// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
// "ResultCodeExtended" Property Constants for Scale
/////////////////////////////////////////////////////////////////////
// Reserved for SCALE:  From 0 + OPOSERREXT to 99 + OPOSERREXT.
// Reserved for Scale:  From 100 + OPOSERREXT to 199 + OPOSERREXT.

const long NCR_ESCAL_UNSTABLE           = 2 + OPOSERREXT; // ReadWeight NO stable weight available
const long NCR_ESCAL_UNDERZERO          = 3 + OPOSERREXT; // ReadWeight Weight under zero
const long NCR_ESCAL_ZEROWEIGHT         = 4 + OPOSERREXT; // ReadWeight Stable Weight of zero
const long NCR_ESCAL_WEIGHTUNCHANGED    = 5 + OPOSERREXT; // ReadWeight status Weight unchanged
const long NCR_ESCAL_NOTREADY           = 6 + OPOSERREXT; // ReadWeight status Not Ready TAR 72258


// end

