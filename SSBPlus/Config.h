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

const TCHAR szXML_CONFIG_FILENAME[]             = _T("SsbPlus.xml");
const TCHAR szXML_CONFIG_ROOT_NODE[]            = _T("ssb-plus-config");
const TCHAR szXML_TAG_SCANNER_PROFILE[]         = _T("scanner-profile");  
const TCHAR szXML_TAG_COMPANY_NUMBER[]          = _T("company-number");
const TCHAR szXML_TAG_STORE_NUMBER[]            = _T("store-number");
const TCHAR szXML_TAG_LANE_NUMBER[]             = _T("lane-number");
const TCHAR szXML_TAG_ENABLE_CAMERAS[]          = _T("enable-cameras");
const TCHAR szXML_MESSAGE_SUBSCRIPTION_LIST[]   = _T("message-subscription-list");


const TCHAR szDEFAULT_SCANNER_PROFILE[]   = _T("NCR_78xx_HID_0215_4-07879_ImageScanner");
const TCHAR szDEFAULT_MESSAGE_SUBSCRIPTION_LIST[] = _T("CA, CB, C6, C7, C9");

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

       CString csScannerProfile; // registry hive for scanner profile
       CString csCompanyNumber;
       CString csStoreNumber;
       CString csLaneNumber;
       std::vector<int> m_vCameras;
       CString csMessageSubscriptionList;


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

}; // Config::

BOOL InitializeConfig();

CConfig * GetConfig();

void DestroyConfig();


#endif