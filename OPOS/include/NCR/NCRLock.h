/*************************************************************************
 *
 * NCRLOCK.H
 *
 * Keylock header file for OPOS Applications.
 *
 * Author:  
 *
 *
 * $Workfile:: NCRLock.h                                                 $
 *
 * $Revision:: 9                                                         $
 *
 * $Archive:: /RetailControls/Rel.3_10/OPOS/include/NCR/NCRLock.h        $
 *
 * $Date:: 12/02/09 3:50p                                                 $
 *
 *************************************************************************
 * 
 * Copyright (c) NCR Corporation 1998.  All rights reserved.
 *
 *************************************************************************
 *
 * $History:: NCRLock.h                                                  $
 * 
 * *****************  Version 9  *****************
 * User: Mr160002     Date: 12/02/09   Time: 3:50p
 * Updated in $/RetailControls/Rel.3_10/OPOS/include/NCR
 * Fix const LONG NCRDIO_KEYBOARD_TYPE.
 * 
 * *****************  Version 1  *****************
 * User: Cg151000     Date: 12/17/03   Time: 10:34a
 * Created in $/RetailControls/Rel.3_3/OPOS/include/NCR
 * 
 * *****************  Version 16  *****************
 * User: Nj151001     Date: 9/21/00    Time: 12:32p
 * Updated in $/OPOS/Rel.2_0/Include/NCR
 * Added message to indicate .bas dependency
 * 
 * *****************  Version 14  *****************
 * User: Duffem       Date: 3/01/00    Time: 10:22a
 * Updated in $/OPOS/Rel.1_8/Include/NCR
 * 
 * *****************  Version 12  *****************
 * User: Dw160003     Date: 11/09/99   Time: 10:28a
 * Updated in $/OPOS/Rel.1_7/Include/NCR
 * added VSS headers
 * 
 * *****************  Version 1  *****************
 * User:        Date: 12/08/95    Time: 10:01a
 * Created in $/OPOS/Rel.1_1/Include/NCR
 * 
 *
 ************************************************************************/


//!!!!!!!!!!!!!!!!!!!!!!!!!!! ndj 9/19/00 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// NOTE: Any modifications made to this include file most also be made in
//       ncrlock.bas !!  Do not check in your modified version of this file
//	 unless you are checking in ncrlock.bas as well.

#if !defined(NCRLOCK_H)
#define      NCRLOCK_H


/////////////////////////////////////////////////////////////////////
// NCR-specific Wedge Keylock "KeyPosition" Property Constants,
// "WaitForKeylockChange" Method: "KeyPosition" Parameter, and
// "StatusUpdateEvent" Event: "Data" Parameter
/////////////////////////////////////////////////////////////////////

const LONG LOCK_KP_EXCP         = 4;
const LONG LOCK_KP_FIELD		= 5;
const LONG LOCK_KP_SUPERVISOR1	= 6;
const LONG LOCK_KP_SUPERVISOR2	= 7;

// added for SCOT Supervisor Switch - H/W can be in unknown state.
// Control has registry entry to communicate or not communicate the
// unknown position upwards.
const LONG LOCK_KP_UNKNOWN      = 101;

/////////////////////////////////////////////////////////////////////
// DirectIO method - Keyboard type is returned in *pData may have one of the
// following values:
//
// 0 - Touch or Non wedge keyboard
// 2 - Dynakey
// 5 - BigTicket keyboard
// 7 - POS keyboard
// 8 - Micromotion keyboard
// 
// Calling DirectIO(NCRDIO_KEYBOARD_TYPE, LONG* pData, BSTR* pString);
//
//
/////////////////////////////////////////////////////////////////////

const LONG NCRDIO_KEYBOARD_TYPE      = 0x01; // command to send

 


#endif                  // !defined(NCRLOCK_H)
