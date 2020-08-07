//****************************************************************************
// Copyright ?1989-2012
// NCR Advanced Restaurant
// All Rights Reserved
//****************************************************************************
//
// @doc
//
// @module  XmlSerializer.h | Specification file for CXmlSerializer
//
// @comm    Specification file for CXmlSerializer
//
// @devnote
//
// Motivation and Goals for the creation of this tool. A tool that would...
//
//      1) allow us to go between objects, DOM and xml with mininal effort
//      2) make it easy to retrofit any current existing classes 
//      3) to build a common tool that could replace our many different
//         methods for xml serializing/deserializing objects.
//      4) Hide DOM and xml creation details from the objects needing serialization.
//         Client object does not need to know anything about building DOM, XML etc.
//     
// @author   TDonnan
//
// @date     2012/07/01
//
// @end
//
//*****************************************************************************
//
// $Header: p:/archives/POS/common/INC/XmlSerializer.h 1.9 2014/05/27 17:03:19Z td210013 Exp $
//
// $Log $
//
//****************************************************************************
#ifndef XML_SERIALIZER_H
#define XML_SERIALIZER_H

// #include "DisableStlWarnings.h"
// #include "StdMacro.h"
// #include "CString.h"
// #include "ComprisEvtLog.h"
// #include "ParseXmlError.h"
#include "string"
// #include "atlstr.h"

#ifndef NO_MSXML_IMPORT
    #pragma warning(push)
    #pragma warning( disable:  4192)
        #import <msxml6.dll>
    #pragma warning(pop)
#endif

//****************************************************************************
//
// Definitions and Constants
//
//****************************************************************************

// CEL Component identifier
// const TCHAR szXsEventLogComponent[] = _T("CXmlSerializer");

// Macro that calls a COM method returning HRESULT value.
#define CHK_HR(stmt)        do { hr=(stmt); if (FAILED(hr)) goto CleanUp; } while(0)

// Macro to verify memory allcation.
#define CHK_ALLOC(p)        do { if (NULL == (p)) { hr = E_OUTOFMEMORY; goto CleanUp; } } while(0)

// @enum 
typedef enum
{
   eXsSuccess = 0,
   eXsFailure,
   eXsXmldocNull = 20,
   eXsXmldocCreateFailure = 21,
   eXsXmldocSaveFailure = 22,
   eXsXmldocLoadFailure = 23,
} eXsRetValue;



//****************************************************************************
// class forward declaration
//****************************************************************************
class CXmlSerializer;


//****************************************************************************
//
// @class IXmlSerialize | IXmlSerialize definition
//
//****************************************************************************
class IXmlSerialize
{
   public:
      virtual eXsRetValue Serialize( CXmlSerializer xs ) = 0;
      virtual eXsRetValue Deserialize( CXmlSerializer xs ) = 0;
};


//****************************************************************************
// @class CXmlSerializer | CXmlSerializer definition
//****************************************************************************
class CXmlSerializer // : public CComprisEvtLog
{
   //*************************************************************************
   // @access public life span 
   //*************************************************************************
   public:
      // @cmember,mfunc default constructor
      CXmlSerializer(void);

      // @cmember,mfunc destructor
      virtual ~CXmlSerializer();

      // @cmember,mfunc copy constructor
      CXmlSerializer (const CXmlSerializer & a_oRhs);

      // @cmember,mfunc assignment operator
      CXmlSerializer & operator=( const CXmlSerializer & a_oRhs );
      
   //*************************************************************************
   // @access public functions
   //*************************************************************************
   public:
   
      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc DOM object scratch creation
      eXsRetValue Create( void );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc Create internal DOM object
      eXsRetValue Open( void );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc Create internal DOM object
      eXsRetValue Open( LPCTSTR a_szFilename );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc These will be deprecated - Do not use.
      eXsRetValue AttachDom(MSXML2::IXMLDOMElementPtr   a_ptrNode);

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc attach DOM 
      eXsRetValue AttachDom( MSXML2::IXMLDOMDocumentPtr   a_ptrDOM );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc Load document object from given filename
      eXsRetValue Load( LPCTSTR a_szFilename );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc Load document object from previously opened stream
      eXsRetValue Load( IStream* a_istream );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc Load document object from previously opened stream
      eXsRetValue LoadXml( const TCHAR *a_szBuffer );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc Load document object from previously opened stream
      eXsRetValue LoadXml( const BSTR a_szBuffer );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc 
      eXsRetValue Save( LPCTSTR a_szFilename );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc 
      eXsRetValue Save( IStream* a_istream );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc Close all open resources
      eXsRetValue Close( void );

   //*************************************************************************
   // @access public serialize/deserialize functions
   //*************************************************************************
   public:

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc serialize text in current node
      eXsRetValue SetText( const _bstr_t & a_bstr );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc serialize element tag and value
      eXsRetValue Serialize( LPCTSTR a_szTagName, LPCTSTR a_szValue );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc serialize element tag and value
      eXsRetValue Serialize( LPCTSTR a_szTagName, int a_nValue );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc serialize element tag and value
      eXsRetValue Serialize( LPCTSTR a_szTagName, bool a_bValue );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc serialize element tag and value
      eXsRetValue Serialize( LPCTSTR a_szTagName, unsigned long a_nValue );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc serialize tag and object
      eXsRetValue Serialize( LPCTSTR a_szTagName, IXmlSerialize &obj );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc deserialize text from current node
      eXsRetValue GetText( _bstr_t & a_bstr );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc deserialize element tag and value
      eXsRetValue Deserialize( LPCTSTR a_szTagName, char & a_char );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc deserialize element tag and value
      eXsRetValue Deserialize( LPCTSTR a_szTagName, short & a_nValue );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc deserialize element tag and value
      eXsRetValue Deserialize( LPCTSTR a_szTagName, unsigned short & a_nValue );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc deserialize element tag and value
      eXsRetValue Deserialize( LPCTSTR a_szTagName, int & a_nInt );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc deserialize element tag and value
      eXsRetValue Deserialize( LPCTSTR a_szTagName, bool &a_bValue );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc deserialize element tag and value
      eXsRetValue Deserialize( LPCTSTR a_szTagName, unsigned long & a_ulNum );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc deserialize element tag and value
      eXsRetValue Deserialize( LPCTSTR a_szTagName, long & a_nNum );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc deserialize element tag and value
      eXsRetValue Deserialize( LPCTSTR a_szTagName, TCHAR * a_szValue, long a_lSize );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc deserialize element tag and value
      eXsRetValue Deserialize( LPCTSTR a_szTagName, std::string  &a_szValue );

      // @cmember,mfunc deserialize element tag and value
      eXsRetValue Deserialize( LPCTSTR a_szTagName, CString &a_szValue );

      // @cmember,mfunc deserialize element tag and value
      eXsRetValue Deserialize( LPCTSTR a_szTagName, _bstr_t & a_bstrtValue );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc deserialize collection
      long DeserializeCollection( LPCTSTR a_szCollectionTagName );

      ////////////////////////////////////////////////////////////////////////
      // @mfunc <c CXmlSerializer> set current item based on index
      eXsRetValue SetCollectionItem( const long a_lIndex );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc deserialize a collection item/object
      eXsRetValue Deserialize( const long a_lIndex, IXmlSerialize & obj );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc deserialize a collection item/object
      eXsRetValue Deserialize( const long a_lIndex, char & a_char );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc deserialize a collection item/object
      eXsRetValue Deserialize( const long a_lIndex, unsigned short & a_nValue );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc deserialize a collection item/object
      eXsRetValue Deserialize( const long a_lIndex, char * a_szValue, long a_lSize );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc deserialize a collection item/object
      eXsRetValue Deserialize( const long a_lIndex, CString &a_szValue );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc deserialize tag and object
      eXsRetValue Deserialize( LPCTSTR a_szTagName, IXmlSerialize &obj );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc deserialize attribute tag and data
      // where a_nSize is the number of characters that will be placed in value
      // including NULL.
      eXsRetValue DeserializeAttribute( LPCTSTR a_szTagName, LPTSTR a_szValue, int a_nSize );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc deserialize attribute tag and data
      eXsRetValue DeserializeAttribute( LPCTSTR a_szTagName, std::string & a_szValue );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc deserialize attribute tag and data
      eXsRetValue DeserializeAttribute( LPCTSTR a_szTagName, CString & a_szValue );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc deserialize attribute tag and data
      eXsRetValue DeserializeAttribute( LPCTSTR a_szTagName, _bstr_t & a_bstrtValue );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc deserialize attribute tag and data
      eXsRetValue DeserializeAttribute( LPCTSTR a_szTagName, short & a_nValue );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc deserialize attribute tag and data
      eXsRetValue DeserializeAttribute( LPCTSTR a_szTagName, int & a_nValue );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc deserialize attribute tag and data
      eXsRetValue DeserializeAttribute( LPCTSTR a_szTagName, bool &a_bValue );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc deserialize attribute tag and data
      eXsRetValue DeserializeAttribute( LPCTSTR a_szTagName, long & a_lValue );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc deserialize attribute tag and data
      eXsRetValue DeserializeAttribute( LPCTSTR a_szTagName, unsigned long & a_ulValue );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc serialize attribute tag and data
      eXsRetValue SerializeAttribute( LPCTSTR a_szTagName, _variant_t a_varValue );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc serialize attribute tag and data
      //eXsRetValue SerializeAttribute( LPCTSTR a_szTagName, const bool a_bValue );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc Get the representative xml text
      CString GetXml( void );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc Get the representative xml text
      bool GetXml( _bstr_t & a_bstrtXml );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc Get a deep copy of the contained xml doc object
      eXsRetValue CopyDoc( MSXML2::IXMLDOMDocumentPtr& a_xmldoc );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc Get the internal doc pointer
      MSXML2::IXMLDOMDocumentPtr GetDoc();

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc create node and nests the serializer
      bool Serialize( LPCTSTR a_szTagName );

      bool Deserialize( LPCTSTR a_szTagName );

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc 
      eXsRetValue UnnestCurrentNode( void );

      //*************************************************************************
   // @access protected member functions
   //*************************************************************************
   protected:

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc 
      void ResetData();

      ////////////////////////////////////////////////////////////////////////
      // @cmember,mfunc 
      long GetSelectCount( void );


   //*************************************************************************
   // @access protected data
   //*************************************************************************
   protected:

      MSXML2::IXMLDOMDocumentPtr     m_ptrDoc;
      MSXML2::IXMLDOMElementPtr      m_ptrCurrentNode;
      MSXML2::IXMLDOMNodeListPtr     m_ptrNodeList;

}; // CXmlSerializer::

#if 1
//****************************************************************************
// @class CXmlSerializer | CXmlSerializerEx definition
//****************************************************************************
class CXmlSerializerEx : public CXmlSerializer
{
   //*************************************************************************
   // @access public life span 
   //*************************************************************************
   public:
      // @cmember,mfunc default constructor
      CXmlSerializerEx();

      // @cmember,mfunc copy constructor
      CXmlSerializerEx(const CXmlSerializerEx & rhs);

      // @cmember,mfunc assignment
      CXmlSerializerEx(const CXmlSerializer & src);

   //*************************************************************************
   // @access public functions
   //*************************************************************************
   public:

      // @cmember,mfunc intialize object from an XML string
      eXsRetValue InitFromXml( const CString& a_strXML );

      // @cmember,mfunc deserialize element tag and value
      eXsRetValue DeserializeEx( LPCTSTR a_szTagName, CString& a_szValue );

      // @cmember,mfunc deserialize attribute tag and data
      eXsRetValue DeserializeAttributeEx( LPCTSTR a_szTagName, CString  &a_szValue );

      // @cmember,mfunc 
      CString GetXmlEx( void );

}; // CXmlSerializerEx::

#endif

//////////////////////////////////////////////////////////////////////////////
// Module identification
static const char RCSID_XML_SERIALIZER_H[] = "$Header: p:/archives/POS/common/INC/XmlSerializer.h 1.9 2014/05/27 17:03:19Z td210013 Exp $";

#endif // XML_SERIALIZER_H

