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

const TCHAR szXML_CONFIG_FILENAME[]                 = _T("PipeServerTesterConfig.xml");
const TCHAR szXML_CONFIG_ROOT_NODE[]                = _T("pipeserver-tester-config");
const TCHAR szXML_MESSAGE_SUBSCRIPTION_LIST[]       = _T("message-subscription-list");
const TCHAR szDEFAULT_MESSAGE_SUBSCRIPTION_LIST[]   = _T("CA, CB, C6, C7, C9");

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