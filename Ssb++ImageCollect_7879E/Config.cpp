//******************************************************************************
// Copyright ?1988-2017
// NCR Self Serve Checkout (SSCO)
// All Rights Reserved
//******************************************************************************
// @doc
//
// @module     Config.cpp | Module description
//
// @comm
//
// @devnote
//
// author      tdonnan
//
// @date       10/25/2012
//
// @end
//******************************************************************************
// $HeadURL$
//
// $Rev::                            $:  Revision of last commit
// $Author::                         $:  Author of last commit
// $Date::                           $:  Date of last commit
//
//******************************************************************************

#include "stdafx.h"
#include "Config.h"
#include "Logging.h"
#include <sstream>
#include <iostream>

#ifdef _DEBUG
   #undef THIS_FILE
   static char THIS_FILE[] = __FILE__;
   #define new DEBUG_NEW
#endif

static CConfig * S_pConfig = NULL;

BOOL InitializeConfig()
{
    BOOL bSuccess = FALSE;

    if (S_pConfig == NULL)
    {
        S_pConfig = new CConfig;

        if (S_pConfig != NULL)
        {
            CXmlSerializer oXs;

            eXsRetValue eXsRetVal = oXs.Open(szXML_CONFIG_FILENAME);

            if (eXsRetVal == eXsSuccess)
            {
                eXsRetVal = oXs.Deserialize(szXML_CONFIG_ROOT_NODE, *S_pConfig);

                if (eXsRetVal == eXsSuccess)
                {
                    bSuccess = TRUE;
                    Trace(TRACE_INFO, _T("Successfully loaded configuration file: %s"), szXML_CONFIG_FILENAME );
                }
                else
                {
                    // failed to deserialize xml file: szXML_CONFIG_FILENAME
                    Trace(TRACE_ERROR, _T("Failed to deserialize xml file:: %s"), szXML_CONFIG_FILENAME);
                }
            }
            else
            {
                // failed to opent xml file: szXML_CONFIG_FILENAME
                Trace(TRACE_ERROR, _T("Failed to open xml file:: %s"), szXML_CONFIG_FILENAME);
            }
        }
        else
        {
            // failed to instantiate configration object. Memory allocation failure.
            Trace(TRACE_ERROR, _T("failed to instantiate configration object. Memory allocation failure."));
        }
    }
    return bSuccess;
}

CConfig * GetConfig()
{
    return S_pConfig;
}

void DestroyConfig()
{
    if (S_pConfig)
    {
        delete S_pConfig;
        S_pConfig = NULL;
    }
}

//**********************************************************************
//
// @func    default constructor
//
// @rdesc   None
//
// @comm    
//
//**********************************************************************

CConfig::CConfig() : 
    csScannerProfile(szDEFAULT_SCANNER_PROFILE)
{
   Clear();
}

//**********************************************************************
//
// @func    Copy constructor
//
// @rdesc   
//
// @comm    
//
//**********************************************************************

CConfig::CConfig( const CConfig& a_oRhs )
{
   Assign(a_oRhs);
}

//**********************************************************************
//
// @func    Destructor
//
// @rdesc   
//
// @comm    
//
//**********************************************************************

CConfig::~CConfig()
{
   Clear();
}

//**********************************************************************
//
// @func    Assignment operator
//
// @rdesc   
//
// @comm    
//
//**********************************************************************

CConfig& CConfig::operator= (const CConfig& a_oRhs)
{
   return Assign(a_oRhs);
}

//**********************************************************************
//
// @func    this function performs the foo test :)
//
// @rdesc   
//
// @comm    
//
//**********************************************************************

//int CConfig::Foo(
//   int iinputValue1,
//   int iinputValue2)
//{
//   return iinputValue1 == iinputValue2;
//}

//**********************************************************************
//
// @func    Assignment helper function
//
// @rdesc   
//
// @comm    
//
//**********************************************************************

CConfig& CConfig::Assign( const CConfig& a_oRhs )
{
   //m_lastError = a_oRhs.m_lastError;
   return *this;
}

//**********************************************************************
//
// @func    
//
// @rdesc   
//
// @comm    
//
//**********************************************************************
// @cmember,mfunc Reset class data to known values
void CConfig::Clear(void)
{
   //m_lastError = eSuccess;
}


eXsRetValue CConfig::Serialize(CXmlSerializer xs)
{
    // Not implemented
    ASSERT(0);
    //xs.Serialize(  szXML_TAG_PRINTER_CHARACTER_WIDTH,  nPrinterCharacterWidth  );

    return eXsSuccess;
}

eXsRetValue CConfig::Deserialize(CXmlSerializer xs)
{
    xs.Deserialize( szXML_TAG_SCANNER_PROFILE, csScannerProfile );

    std::string ssEnableCameras;

    xs.Deserialize( szXML_TAG_ENABLE_CAMERAS, ssEnableCameras );

    xs.Deserialize(szXML_TAG_COMPANY_NUMBER, csCompanyNumber);
    xs.Deserialize(szXML_TAG_STORE_NUMBER, csStoreNumber);
    xs.Deserialize(szXML_TAG_LANE_NUMBER, csLaneNumber);

    std::istringstream f(ssEnableCameras);

    std::string s;
    while (std::getline(f, s, ',')) {
        m_vCameras.push_back(atoi(s.c_str()));
    }

    return eXsSuccess;
}

bool CConfig::IsEqual(const CConfig& a_oRhs)
{
    bool bEqual = true;

    //bEqual &= (nPrinterCharacterWidth == a_oRhs.nPrinterCharacterWidth );
    //bEqual &= (oFontNormal.IsEqual(     a_oRhs.oFontNormal));
    //bEqual &= (oFontBold.IsEqual(       a_oRhs.oFontBold));
    //bEqual &= (oFontItalic.IsEqual(     a_oRhs.oFontItalic));
    //bEqual &= (oFontBoldItalic.IsEqual( a_oRhs.oFontBoldItalic));

    return bEqual;
}


//**********************************************************************
//
// @func    
//
// @rdesc   
//
// @comm    
//
//**********************************************************************
// @cmember,mfunc Get the last occurring error
//long CConfig::GetLastError(void)
//{
//   return m_lastError;
//}



//**********************************************************************
//
// @func    
//
// @rdesc   
//
// @comm    
//
//**********************************************************************
// @cmember,mfunc Set the last occurring error
//int CConfig::SetLastError( long p_lastError )
//{
//   m_lastError = p_lastError;
//   return ERROR_SUCCESS == m_lastError;
//}

