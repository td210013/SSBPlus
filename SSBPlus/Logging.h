////////////////////////////////////////////////////////
// Logging.h: interface for the CLogging class.
//
////////////////////////////////////////////////////////

#if !defined(AFX_LOGGING_H__6F36FAC3_E2CC_11D6_A616_00A0C9EC8483__INCLUDED_)
#define AFX_LOGGING_H__6F36FAC3_E2CC_11D6_A616_00A0C9EC8483__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// #include "RPSW/CmClass.h"	 //include for data capture, commented out 
#include "CmDataCapture.h" // es185043 - I used CmClass.h file in C:\scot\ADK\include dir
#include <ATltrace.h>
#include <atlstr.h>

const TCHAR DRMESSAGEHANDLER_DCAP_KEY[] = _T("DataCapture\\");
const TCHAR REGDRMESSAGEHANDLER[] =_T("SOFTWARE\\NCR\\SsbImageCollectDlg\\");


const UINT TRACE_API                = 0x00000001; // API method entry and exit
const UINT TRACE_ERROR              = 0x00000002; // Error event - major unrecoverable problem
const UINT TRACE_WARNING            = 0x00000004; // Warning event - minor recoverable problem
const UINT TRACE_INFO	            = 0x00000008; // Informational event - trace some normal activity
const UINT TRACE_EXTENSIVE          = 0x00000010; // Trace event - extensive tracing
const UINT TRACE_PERFORMANCE        = 0x00000020; // Performance tracing
const UINT TRACE_SYNCHRONIZATION    = 0x00000040; // Synchronization tracing
const UINT TRACE_EVENT              = 0x00000080; // Event tracing
const UINT TRACE_AUTOMATION         = 0x00000100; // Automation tracing
const UINT TRACE_ALL		        = 0xFFFFFFFF; // Trace all

const TCHAR cMASKED[]               = _T("[masked]");
const TCHAR cASTERISK[]             = _T("****");
const TCHAR cEQUAL                  = _T('=');
const TCHAR cSEMICOLON              = _T(';');

#if 1
#define Trace \
   if( CLogging::GetLogging() ) \
   if( CLogging::GetLogging()->GetDataCapture() ) \
   CLogging::GetLogging()->GetDataCapture()->DCPrintf
#else
   // my temporary logging replacement
   
   static void MyPrintf(int /*x*/, LPCTSTR /*szFormat*/, ...)
   {
      // CString csMsg;
      // CString csFormat;

      // va_list args;
      // va_start(args, szFormat);

      // csFormat.Format(_T("%s\n"), szFormat);
      // csMsg.Format( csFormat, args );
      // va_end(args);
      // TRACE(szFormat);
      // ATLTRACE(szFormat);
   }

#define Trace MyPrintf
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogging class
//

class CLogging  
{
public:
	~CLogging();
	
	inline CString GetDataCapRegPath() const { return m_strDataCapRegPath; }
	inline CmDataCapture* GetDataCapture() { return &m_DataCap; }
	static CString GetFileVersion();
    static CLogging* GetLogging();	
	static BOOL Initialize();	
	static void UnInitialize();
	static CString MaskedParam(const CString& csParam);

private:
	CLogging();    
	
	CmDataCapture m_DataCap;
	CString m_strDataCapRegPath;
};

#endif // !defined(AFX_LOGGING_H__6F36FAC3_E2CC_11D6_A616_00A0C9EC8483__INCLUDED_)
