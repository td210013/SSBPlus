//******************************************************************************
// Copyright ?1988-2017
// NCR Self Serve Checkout (SSCO)
// All Rights Reserved
//******************************************************************************
// @doc
//
// @module     Config.h | Module description
//
// @comm
//
// @devnote
//
// author      tdonnan
//
// @date       01/01/2017
//
// @end
//
//******************************************************************************
// $HeadURL$
//
// $Rev::                            $:  Revision of last commit
// $Author::                         $:  Author of last commit
// $Date::                           $:  Date of last commit
//
//******************************************************************************
#ifndef Config_H
#define Config_H

#include "XmlSerializer.h"
#include "vector"


typedef enum {
    YUY2_960_600_45 = 0,
    YUY2_1280_800_45 = 1,
    YUY2_480_300_45 = 2,
    YUY2_320_200_45 = 3,
} eFrameType;

typedef enum {
    STANDARD = 0,
    ALWAYS_ON = 1,
    ALWAYS_OFF = 2,
} eIlluminationMode;


const TCHAR szXML_CONFIG_FILENAME[]       = _T("SsbImageCollectConfigZebra.xml");
const TCHAR szXML_CONFIG_ROOT_NODE[]      = _T("ssb-image-collection-config");
const TCHAR szXML_TAG_SCANNER_PROFILE[]   = _T("scanner-profile");  
const TCHAR szXML_TAG_COMPANY_NUMBER[]    = _T("company-number");
const TCHAR szXML_TAG_STORE_NUMBER[]      = _T("store-number");
const TCHAR szXML_TAG_LANE_NUMBER[]       = _T("lane-number");
const TCHAR szXML_TAG_ENABLE_CAMERAS[]    = _T("enable-cameras");
const TCHAR szXML_TAG_FRAME_TYPE[]        = _T("frame-type");
const TCHAR szXML_TAG_ILLUMINATION_MODE[] = _T("illumination-mode");

const TCHAR szDEFAULT_SCANNER_PROFILE[]   = _T("Zebra_MP7000");

//******************************************************************************
//
// @class Config | Layout for a C++ class
//
// @base  BaseClassName | IXmlSerialize - serialize / serialize support interface
//
//******************************************************************************
class CConfig : public IXmlSerialize
{
   //***************************************************************************
   // @access data members
   //***************************************************************************
   public:

       CString csScannerProfile; 
       CString csCompanyNumber;
       CString csStoreNumber;
       CString csLaneNumber;

       // Zebra device settings 
       eFrameType FrameType;
       eIlluminationMode IlluminationMode;

       // std::vector<int> m_vCameras;

   //***************************************************************************
   // @access Life cycle
   //***************************************************************************
   public:

      // @cmember,mfunc Default constructor
       CConfig();

      // @cmember,mfunc Copy constructor
       CConfig(const CConfig& a_oRhs);

      // @cmember,mfunc Destructor
      virtual ~CConfig();

   //***************************************************************************
   // @access interface
   //***************************************************************************
   public:

      // @cmember,mfunc serialize object
      eXsRetValue Serialize(CXmlSerializer xs);

      // @cmember,mfunc deserialize object
      eXsRetValue Deserialize(CXmlSerializer xs);

      // @cmember,mfunc Assignment helper function
      CConfig& Assign(const CConfig& a_oRhs);

      // @cmember,mfunc Assignment operator
      CConfig& operator= (const CConfig& a_oRhs);

      // @cmember,mfunc Reset class data to known values
      void Clear(void);

      bool IsEqual(const CConfig& a_oRhs);

   //***************************************************************************
   // @access interface
   //***************************************************************************
   public:
       void Log();

}; // Config::

BOOL InitializeConfig();

CConfig * GetConfig();

void DestroyConfig();


#endif