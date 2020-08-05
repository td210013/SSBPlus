/**
 * NCRMicr.h declares the constants needed to make use of the NCR MICR OPOS
 * extensions.
 */
/******************************************************************************
 *
 * $Workfile:: NCRMicr.h                                                      $
 *
 * $Revision:: 1                                                              $
 *
 * $Archive:: /RetailControls/Rel.3_3/OPOS/include/NCR/NCRMicr.h              $
 *
 * $Date:: 1/23/04 11:33a                                                     $
 *
 ******************************************************************************
 *
 * Copyright (c) NCR Corporation 2004.  All rights reserved.
 *
 ******************************************************************************
 *
 * $History:: NCRMicr.h                                                       $
 * 
 * *****************  Version 1  *****************
 * User: Spohnb       Date: 1/23/04    Time: 11:33a
 * Created in $/RetailControls/Rel.3_3/OPOS/include/NCR
 * Initial version.
 *
 *****************************************************************************/

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// NCR MICR value-add extensions via DirectIO.
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

//---------------------------------------------------------------------------
//--- MICR DirectIO ---
//---------------------------------------------------------------------------

//** ClearExceptions **  Clear all exception strings.
//    Data:       (Not used.)
//    String:     (Not used.)
//    ResultCode: OPOS_SUCCESS.
const LONG NCRDIO_MICR_CLEAR_EXCEPTIONS = 201;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//** AddException **  Add an exception string to the end of the list of strings.
//                    (See details below in "MICR Data Handling".)
//    Data:       Out> If improper format, set to the approximate character number
//                  where bad format was found.
//    String:     In > Exception string.
//    Remarks:    When Micr data is parsed, the exception strings will be examined
//                  starting with the first one added. If the raw data does not
//                  match the string, then the next exception string is examined.
//                  If no exception strings match, then the default strings are used.
//    ResultCode: OPOS_SUCCESS   = String is ok; exception added.
//                OPOS_E_ILLEGAL = String has improper format.
const LONG NCRDIO_MICR_ADD_EXCEPTION = 202;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

//** RemoveNonDigits **  Selects optional space and dash removal from account
//                       and/or transit numbers. The default value is ""; this
//                       default may be changed using the system registry.
//    Data:       (Not used.)
//    String:     In > RemoveNonDigits string.
//                  Set to "A" for Account; "T" for Transit; "AT" for both;
//                  "" for neither",
//    ResultCode: OPOS_SUCCESS.
const LONG NCRDIO_MICR_SET_REMOVE_NON_DIGITS = 203;


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
// MICR Data Handling.
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
//
// MICR data consists of digits, spaces, plus four special MICR symbols:
//   Transit, On-Us, Amount, Dash.
//
// In the following discussion, and in the DirectIO command NCRDIO_MICR_ADD_EXCEPTION,
// these characters are used to describe MICR format data:
//     t o a - space   MICR substitution symbols for Transit, On-Us,
//                       Amount, Dash, and Space.
//     X               Digit, space, or dash.
//     E               EPC: Digit.
//     T               Transit number: Digit or dash.
//                       (For US checks, only digits are allowed.)
//     A               Account number: Digit, space, or dash.
//                       (Account number begins and ends with a digit.)
//     S               Serial/sequence number: Digit.
//     $               Amount: Digit.
//     B               Series of blanks (spaces) and/or dashes.
//                       Optional unless a repeat count is given.
//     *               Trailer: Series of any digits, dashes, and spaces followed
//                       by an optional amount field.
//                       (= "X[0+]a$[10]aB" if amount present and "X[0+]" if not).
//
//   An optional repeat count may follow any character except *:
//     [count]         Specifies exactly "count" characters.
//     [min-max]       Specifies between "min" and "max" characters.
//     [min+]          Specifies at least "min" characters.
//
//   The general MICR data format is:
//    oX[1+]o EtT[9]t X[1+]oX[0+] a$[10]a
//
//   The fields are:
//    oX[1+]o     Auxiliary on-us (optional).
//                  Bounded on left and right with On-Us characters.
//                  Often used for serial number, in which case 'X' are digits.
//    E           EPC character (optional).
//                  Immediately precedes leftmost Transit character.
//    tT[9]t      Transit number (required).
//                  Bounded on left and right with Transit characters.
//    X[1+]oX[0+] On-us (optional).
//                  (More than one On-Us symbol could be included, although I
//                  haven't seen this.)
//    a$[10]a     Amount (optional).
//                  Bounded on left and right with Amount characters.
//
//===========================================================================
//
// The transit number and amount can always be found and properly parsed.
// The placement of the serial and account numbers, however, varies between
//   banks.  In addition, the Auxiliary On-Us and the On-Us fields may contain
//   other bank-specific data.
//
// To handle the majority of cases, the following default handling of serial
// and account numbers is performed:
//
// Case (1): Serial number in Auxiliary On-Us field.
//           Account number in On-Us field from first digit to the On-Us symbol,
//             not including spaces and dashes before the On-Us.
//   "BoS[1+]oBEtT[9]tBA[1+]Bo*"
//     -> Serial number is the number S[1+]
//     -> Account number is the number A[1+].
//   Example:
//      "o9876o t123456789t 12-345-6789 o 6666 a0000054321a"
//        Transit: "123456789"      Amount: "0000054321"
//        Account: "12-345-6789"    Serial: "9876"
//        EPC:     ""
//        ** If the RemoveNonDigits string contains an 'A', then
//           Account is "123456789".
//
// Case (2): Account number in On-Us field. This field starts with an optional
//             On-Us symbol, then the account number is taken from first digit to the
//             required trailing On-Us symbol, not including spaces and dashes
//             before the trailing On-Us.
//           Serial number in On-Us field after the On-Us symbol.
//             Must be three or more digits.
//   "BEtT[9]tBo[0-1]A[1+]BoBS[3+]*",
//     -> Account number is the number A[1+], if present.
//     -> Serial number is the number S[3+]
//   Example:
//      "5t123456789t o12 345-67 o 6666"
//        Transit: "123456789"      Amount: ""
//        Account: "12 345-67"      Serial: "6666"
//        EPC:     "5"
//        ** If the RemoveNonDigits string contains an 'A', then
//           Account is "1234567".
//
// Case (3): Serial number in On-Us field from first digit to next non-digit;
//             must be between three and five digits.
//           Account number in On-Us field from first digit after the serial
//             number to the On-Us symbol, not including spaces and dashes
//             before the On-Us.
//   "BEtT[9]tBS[3-5]B[1+]A[1+]Bo*"
//     -> Serial number is the number S[3-5]
//     -> Account number is the number A[1+].
//   Example:
//      "t12345-789t 555 12 345-67 o 66"
//        Transit: "12345-789"      Amount: ""
//        Account: "12 345-67"      Serial: "555"
//        EPC:     ""
//        ** If the RemoveNonDigits string contains a 'T', then
//           Transit is "12345789".
//        ** If the RemoveNonDigits string contains an 'A', then
//           Account is "1234567".
//   Example:
//      "t123456789t 555 12 345-67 o 66 a0000054321a"
//        Transit: "123456789"      Amount: "0000054321"
//        Account: "12 345-67"      Serial: "555"
//        EPC:     ""
//        ** If the RemoveNonDigits string contains an 'A', then
//           Account is "1234567".
//
// Case (4): Account number in On-Us field from first digit to the On-Us symbol,
//             not including spaces and dashes before the On-Us.
//   "BEtT[9]tBA[1+]Bo*"
//     -> No serial number.
//     -> Account number is the number A[1+].
//   Example:
//      "t123456789t 55 12 345-67 o 66"
//        Transit: "123456789"      Amount: ""
//        Account: "55 12 345-67"   Serial: ""
//        EPC:     ""
//        ** If the RemoveNonDigits string contains an 'A', then
//           Account is "551234567".
//
// Case (5): Serial number in On-Us field from first digit to the On-Us symbol,
//             must be three or more digits.
//           Account number in second On-Us field from the first digit after the
//             On-Us symbol that ended the serial number to the On-Us symbol,
//             not including spaces and dashes before the On-Us.
//   "BtT[9]tBS[3+]BoBA[1+]o*",
//     -> Serial number is the number S[3+]
//     -> Account number is the number A[1+].
//   Example:
//      "t12345-789t 555 o 12 345-67 o 66"
//        Transit: "12345-789"      Amount: ""
//        Account: "12 345-67"      Serial: "555"
//        EPC:     ""
//        ** If the RemoveNonDigits string contains a 'T', then
//           Transit is "12345789".
//        ** If the RemoveNonDigits string contains an 'A', then
//           Account is "1234567".
//===========================================================================
//
// If the default handling is not correct for some banks, then exception
// strings can be added for these cases.  The exception strings should be
// formatted as in these examples.
//
// Example exception string "t061000052tBA[10-12]o*":
//     This string requires that the EPC and Serial Number be absent.
//   If Micr Data is: "t061000052t 123 456 789o"
//        Transit: "061000052"      Amount: ""
//        Account: "123 456 789"    Serial: ""
//        EPC:     ""
//        ** If the RemoveNonDigits string contains an 'A', then
//           Account is "123456789".
//        (Without the exception, then default case (3) would have returned "123"
//         as the serial number and "456 789" (** or 456789) as the account number.)
//   If Micr Data is: "t061000052t 123456789012o3333a0000054321a"
//        Transit: "061000052"      Amount: "0000054321"
//        Account: "123456789012"   Serial: ""
//        EPC:     ""
//        (Note that the "3333" between the On-Us and Amount symbols is ignored.
//         Without the exception, then default case (2) would have returned "3333"
//         as the serial number.)
//
// Example exception string "t052000113tBS[3]BA[7-10]o*":
//     This string requires that the EPC be absent.
//   If Micr Data is: "t052000113t 123 456 789o"
//        Transit: "052000113"      Amount: ""
//        Account: "456 789"        Serial: "123"
//        EPC:     ""
//        ** If the RemoveNonDigits string contains an 'A', then
//           Account is "456789".
//
// Example exception string "tTTT00011TtBS[2]A[10]S[3]o*":
//     This string requires that the EPC be absent.
//   If Micr Data is: "t99-000119t 123 456 7890987o a9876543210a"
//        Transit: "99-000119"      Amount: "9876543210"
//        Account: "3 456 7890"     Serial: "12987"
//        EPC:     ""
//        ** If the RemoveNonDigits string contains a 'T', then
//           Transit is "99000119".
//        ** If the RemoveNonDigits string contains an 'A', then
//           Account is "34567890".
