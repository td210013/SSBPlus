/////////////////////////////////////////////////////////////////////
//
// OposLock.h
//
//   Keylock header file for OPOS Applications.
//
// Modification history
// ------------------------------------------------------------------
// 95-12-08 OPOS Release 1.0                                     CRM
//
/////////////////////////////////////////////////////////////////////

#if !defined(OPOSLOCK_H)
#define      OPOSLOCK_H


#include "Opos.h"


/////////////////////////////////////////////////////////////////////
// "KeyPosition" Property Constants
// "WaitForKeylockChange" Method: "KeyPosition" Parameter
// "StatusUpdateEvent" Event: "Data" Parameter
/////////////////////////////////////////////////////////////////////

const LONG LOCK_KP_ANY          = 0;    // WaitForKeylockChange Only
const LONG LOCK_KP_LOCK         = 1;
const LONG LOCK_KP_NORM         = 2;
const LONG LOCK_KP_SUPR         = 3;
/* Jay R -- Added 02-17-2011 UPOS 1.13 Compliance -- Start */
const LONG LOCK_KP_ELECTRONIC   = 0;
const LONG LOCK_KT_STANDARD     = 1;
const LONG LOCK_KT_ELECTRONIC   = 2;
/* Jay R -- Added 02-17-2011 UPOS 1.13 Compliance -- End */

#endif                  // !defined(OPOSLOCK_H)
