////////////////////////////////////////////////////////////
// Logging.cpp: implementation of the CLogging class.
//
////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Logging.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogging class
//

// Static members //
static CLogging *m_theLogging = NULL;

//************************************************************************ 
//
// Function:		CLogging
// Purpose:			Constructor
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************ 
CLogging::CLogging()
{
	static const TCHAR PSX_DCAP_INITMSG[] = _T("-- PipeServer Monitor Start Tracing -- ");
	m_strDataCapRegPath = REGDRMESSAGEHANDLER;
	m_strDataCapRegPath += DRMESSAGEHANDLER_DCAP_KEY;	
	m_DataCap.ReadRegistry( m_strDataCapRegPath, _T("") );

   // Removed the GetFileVersion call as it causes and access violation when 
   // this COM object is registered in subdirectory that is not the home or 
   // or current work directory of the application.
   //
   // m_DataCap.Initialize( PSX_DCAP_INITMSG + GetFileVersion() );  // causes dll incompatibility between release and debug mode.
   m_DataCap.Initialize(PSX_DCAP_INITMSG);
}

//************************************************************************ 
//
// Function:		~CLogging
// Purpose:			Destructor
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************ 
CLogging::~CLogging()
{
}


//************************************************************************ 
//
// Function:		GetFileVersion
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************ 
// #include <Psapi.h> 
CString CLogging::GetFileVersion()
{
    // AFX_MANAGE_STATE( AfxGetStaticModuleState() );
   CString strVersion(_T("No version info available"));

   TCHAR szFile[MAX_PATH + 1] = _T("");

   // 
   // The call below throws and access violation when the COM object
   // is registered in a different sub directory than the application.
   // The access violation goes away when the call below is removed.
   // Removing the module filename from the logs for the time being.
   //
   GetModuleFileName(AfxGetApp()->m_hInstance, szFile, MAX_PATH);
   
   // GetModuleFileNameEx(AfxGetApp()->m_hInstance, NULL, szFile, MAX_PATH);

    DWORD dwArg = 0;
    DWORD dwInfoSize = GetFileVersionInfoSize( szFile, &dwArg );
    if( dwInfoSize > 0 )
    {
        BYTE *pBuff = new BYTE[dwInfoSize];
        if( GetFileVersionInfo( szFile, 0, dwInfoSize, pBuff ) )
        {
            VS_FIXEDFILEINFO *vInfo = NULL;
            UINT uInfoSize = 0;
            if( VerQueryValue( pBuff, _T("\\"), (LPVOID*)&vInfo, &uInfoSize ) )
            {
                if( uInfoSize > 0 )
                {
                    strVersion.Format( _T("File version: %d.%d.%d.%d"), HIWORD( vInfo->dwFileVersionMS ), LOWORD( vInfo->dwFileVersionMS ),
                       HIWORD( vInfo->dwFileVersionLS ), LOWORD( vInfo->dwFileVersionLS ) );                    
                }
                else
                {
                    strVersion = _T("Version information not available");
                }               
            }
            else
            {
                strVersion = _T("Version information not available");
            }           
        }
        else
        {
            strVersion = _T("Error retrieving version info");            
        }
        
        delete[] pBuff;
    }
    else
    {
        strVersion = _T("No version info available");
    }
    return strVersion;
}

//************************************************************************ 
//
// Function:		GetLogging
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************ 
CLogging* CLogging::GetLogging()
{ 
	return m_theLogging;
}

//************************************************************************ 
//
// Function:		Initialize
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************ 
BOOL CLogging::Initialize()
{
	if( !m_theLogging )
    {
        m_theLogging = new CLogging;
    }
	return ( m_theLogging != NULL );
}

//************************************************************************ 
//
// Function:		UnInitialize
// Purpose:			
// Parameters:
// Returns:	
// Calls:
// Side Effects:
// Notes:
//
//************************************************************************ 
void CLogging::UnInitialize()
{
	CLogging *myCopyTheLogging = m_theLogging;
	m_theLogging = NULL;
	// Ensure that any attempted tracing after this point is ignored
    if( myCopyTheLogging )
	{
		myCopyTheLogging->m_DataCap.DoDCPrintf( TRACE_EXTENSIVE, _T("CLogging::UnInitialize() - Doing data capture flush and cleanup") );
		myCopyTheLogging->m_DataCap.Flush();
		myCopyTheLogging->m_DataCap.Cleanup();
		delete myCopyTheLogging;
	}
}

CString CLogging::MaskedParam(const CString& csParam)
{
    CString csVal(csParam);
    CString csTemp(csVal);
    CString csReturn;	
    int nLength = csVal.GetLength();	
    bool bFlag = true; 

    do
    {			
        int nStart = csVal.Find(cMASKED);
        if(nStart > -1)   
        {
	        int nIndexEq = csVal.Find(cEQUAL, nStart);
	        int nIndexCol = csVal.Find(cSEMICOLON, nIndexEq);
	        if(nIndexEq > -1)
	        {
	            csTemp = csVal.Left(nIndexEq + 1);
	            csTemp += cASTERISK;
	        }

	        if((nIndexCol > -1) && (nIndexCol < nLength))
	        {
	            csVal = csVal.Mid(nIndexCol, nLength - nIndexCol);
	        }
	        else
	        {
	            bFlag = false;
	        }
         }
         else
         {
	        return csTemp;
         }
    	csReturn += csTemp;
    }while(bFlag);

    return csReturn;

}
