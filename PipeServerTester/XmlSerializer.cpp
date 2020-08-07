//****************************************************************************
// Copyright ?1989-2012
// NCR Advanced Restaurant
// All Rights Reserved
//****************************************************************************
// @doc
//
// @module  XmlSerializer.h | Specification file for CXmlSerializer
//
// @comm    Implementation file for CXmlSerializer
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
// @author  TDonnan
//
// @date    2012/07/01
//
// @end
//*****************************************************************************
// $Header: $
//
// $Log: XmlSerializer.cpp $
//
//****************************************************************************

#include "Stdafx.h"

#pragma warning( push )
#pragma warning(disable:4786)
#pragma warning(disable:4310)

// #include "ComprisEvtLog.h"
// #include "LockedXmlFile.h"
#include "XmlSerializer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif


//****************************************************************************
// Definitions and Constants
//****************************************************************************


////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> default constructor
CXmlSerializer::CXmlSerializer() :  
      m_ptrDoc(NULL),
      m_ptrCurrentNode(NULL),
      m_ptrNodeList(NULL)
{
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> destructor
CXmlSerializer::~CXmlSerializer() 
{ 
   ResetData();
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> copy constructor
CXmlSerializer::CXmlSerializer(
   const CXmlSerializer& a_oRhs) 
{
   m_ptrDoc          = a_oRhs.m_ptrDoc;  
   m_ptrCurrentNode  = a_oRhs.m_ptrCurrentNode;
   m_ptrNodeList     = a_oRhs.m_ptrNodeList;
} 

CXmlSerializer & CXmlSerializer::operator=( const CXmlSerializer & a_oRhs )
{
   if (this != &a_oRhs)
   {
      m_ptrCurrentNode  = a_oRhs.m_ptrCurrentNode;
      m_ptrNodeList     = a_oRhs.m_ptrNodeList;
      m_ptrDoc          = a_oRhs.m_ptrDoc;  
   }
   return *this;
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> DOM object scratch creation
eXsRetValue CXmlSerializer::Create( void )
{
   Close();

   HRESULT hr = E_FAIL;
   MSXML2::IXMLDOMProcessingInstructionPtr ptrPI = NULL;

   // create DOM
   
   #if (_MSC_VER < 1910 )
      if (FAILED(hr = m_ptrDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60))))
   #else
      if (FAILED(hr = m_ptrDoc.CreateInstance(_T("Msxml2.DOMDocument.4.0"))))
   #endif
   // if (FAILED(hr = m_ptrDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60))))
   {
      // // LLOGERR((szXsEventLogComponent, _T("Failed to create DOMDocument, hr: 0x%p"), hr));
      goto CleanUp;
   }

   // create and append processing instructions
   if ( NULL == (ptrPI = m_ptrDoc->createProcessingInstruction("xml", " version='1.0' encoding='UTF-8'")) )
   {
      // LLOGERR((szXsEventLogComponent, _T("Failed to create DOMDocument processing instruction."))); 
      goto CleanUp;
   }

   if ( NULL == m_ptrDoc->appendChild( ptrPI ) )
   {
      // LLOGERR((szXsEventLogComponent, _T("Failed to create DOMDocument processing instruction."))); 
      goto CleanUp;
   }

   m_ptrCurrentNode = m_ptrDoc->GetdocumentElement();

   return eXsSuccess;

   // Cleanup
   CleanUp:

      ResetData();

   return eXsFailure;
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> Create internal DOM object
eXsRetValue CXmlSerializer::Open( void )
{
   HRESULT hr = E_FAIL;

   Close();

   // create DOM
   #if (_MSC_VER < 1910 )
      if (FAILED(hr = m_ptrDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60))))
   #else
      if (FAILED(hr = m_ptrDoc.CreateInstance(_T("Msxml2.DOMDocument.4.0"))))
   #endif
   //if ( FAILED(hr = m_ptrDoc.CreateInstance(_T("Msxml2.DOMDocument.4.0")) ) )
   {
      // LLOGERR((szXsEventLogComponent, _T("Failed to create DOMDocument, hr: 0x%p"), hr));
      goto CleanUp;
   }

   return eXsSuccess;

   // cleanup
   CleanUp:

      ResetData();

   return eXsFailure;
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> Create internal DOM object
eXsRetValue CXmlSerializer::Open( LPCTSTR a_szFilename )
{

   if (eXsSuccess == Open())
   {
      return Load(a_szFilename);
   }

   ResetData();
   return eXsFailure;
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> Create internal DOM object
eXsRetValue CXmlSerializer::AttachDom( 
   MSXML2::IXMLDOMElementPtr    a_ptrNode )
{
   m_ptrDoc          = NULL;
   m_ptrCurrentNode  = NULL;
   

   if ( a_ptrNode == NULL )
   {
      return eXsFailure;
   }

   m_ptrDoc          = a_ptrNode->GetownerDocument();
   m_ptrCurrentNode  = a_ptrNode;

   m_ptrNodeList     = NULL;
   return eXsSuccess;
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> Create internal DOM object
eXsRetValue CXmlSerializer::AttachDom( 
   MSXML2::IXMLDOMDocumentPtr    a_ptrDOM )
{
   m_ptrDoc          = NULL;
   m_ptrCurrentNode  = NULL;
   m_ptrNodeList     = NULL;

   if ( a_ptrDOM == NULL )
   {
      return eXsFailure;
   }

   m_ptrDoc          = a_ptrDOM;
   m_ptrNodeList     = NULL;
   return eXsSuccess;
}


////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> Load document object from given filename
eXsRetValue CXmlSerializer::Load( LPCTSTR a_szFilename )
{
   eXsRetValue eXsRet = eXsFailure;

   // ATLASSERT(m_ptrDoc != NULL);
   ASSERT(m_ptrDoc != NULL);

   // load DOM from file
   if ( m_ptrDoc != NULL && m_ptrDoc->load( a_szFilename ) == VARIANT_TRUE )
   {
      // LLOGDEBUG((szXsEventLogComponent, _T("DOMDocument loaded successfully from file %s"), a_szFilename)); 
      // leaving following line for debug.
      // printf("XML DOM loaded from %s:\n%s\n", a_szFilename, (LPCSTR)m_ptrDoc->xml);
      eXsRet = eXsSuccess;
   }
   else
   {
      // Failed to load DOM from file
      // not catastrophic as this may be the first time we need a file.
      // LLOGWARN((szXsEventLogComponent, _T("Warning DOMDocument not initiaized from %s"), a_szFilename)); 
   }

   return eXsRet;
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> Load document object from previously opened stream
eXsRetValue CXmlSerializer::Load( IStream* a_istream )
{
   eXsRetValue eReturn = eXsFailure;

   // ATLASSERT( m_ptrDoc != NULL );
   ASSERT( m_ptrDoc != NULL );

   // load DOM from IStream
   if ( m_ptrDoc != NULL && m_ptrDoc->load( a_istream ) == VARIANT_TRUE )
   {
      // LLOGDEBUG((szXsEventLogComponent, _T("DOMDocument loaded successfully from IStream*: 0x%p"), (void*)a_istream)); 
      // leaving following line for debug.
      // printf("XML DOM loaded from %s:\n%s\n", a_szFilename, (LPCSTR)m_ptrDoc->xml);
      eReturn = eXsSuccess;
   }
   else
   {
      // Failed to load DOM from file
      // not catastrophic as this may be the first time we need a file.
      // LLOGWARN((szXsEventLogComponent, _T("Warning DOMDocument not initiaized from IStream*: 0x%p"), a_istream)); 
   }

   return eReturn;
}

// @cmember,mfunc intialize object from an XML string
eXsRetValue CXmlSerializer::LoadXml( const TCHAR * a_szBuffer )
{
   HRESULT hr = E_FAIL;

   ResetData();

   // create DOM
   #if (_MSC_VER < 1910 )
      if (FAILED(hr = m_ptrDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60))))
   #else
      if (FAILED(hr = m_ptrDoc.CreateInstance(_T("Msxml2.DOMDocument.4.0"))))
   #endif
   //if ( FAILED(hr = m_ptrDoc.CreateInstance(_T("Msxml2.DOMDocument.4.0")) ) )
   {
      // LLOGERR((szXsEventLogComponent, _T("Failed to create DOMDocument. hr=%0x0"), hr));
      goto CleanUp;
   }

   // load DOM from file
   if ( (m_ptrDoc->loadXML(_bstr_t(a_szBuffer))) == VARIANT_TRUE )
   {
      //// LLOGDEBUG((szXsEventLogComponent, _T("DOMDocument loaded successfully from XML string."))); 
      // leaving following line for debug.
      // printf("XML DOM loaded from %s:\n%s\n", a_szFilename, (LPCSTR)m_ptrDoc->xml);
   }
   else
   {
      // Failed to load DOM from file
      // not catastrophic as this may be the first time we need a file.
      // LLOGWARN((szXsEventLogComponent, _T("Warning DOMDocument not initiaized from XML string."))); 
   }

   return eXsSuccess;

   // cleanup
   CleanUp:

      ResetData();

   return eXsFailure;
}

// @cmember,mfunc intialize object from an XML string
eXsRetValue CXmlSerializer::LoadXml( BSTR bstr)
{
   HRESULT hr = E_FAIL;

   ResetData();

   // create DOM
   #if (_MSC_VER < 1910 )
      if (FAILED(hr = m_ptrDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60))))
   #else
      if (FAILED(hr = m_ptrDoc.CreateInstance(_T("Msxml2.DOMDocument.4.0"))))
   #endif
   //if ( FAILED(hr = m_ptrDoc.CreateInstance(_T("Msxml2.DOMDocument.4.0")) ) )
   {
      // LLOGERR((szXsEventLogComponent, _T("Failed to create DOMDocument. hr=%0x0"), hr));
      goto CleanUp;
   }

   // load DOM from file
   if ( (m_ptrDoc->loadXML(_bstr_t(bstr))) == VARIANT_TRUE )
   {
      //// LLOGDEBUG((szXsEventLogComponent, _T("DOMDocument loaded successfully from XML string."))); 
      // leaving following line for debug.
      // printf("XML DOM loaded from %s:\n%s\n", a_szFilename, (LPCSTR)m_ptrDoc->xml);
   }
   else
   {
      // Failed to load DOM from file
      // not catastrophic as this may be the first time we need a file.
      // LLOGWARN((szXsEventLogComponent, _T("Warning DOMDocument not initiaized from XML string."))); 
   }

   return eXsSuccess;

   // cleanup
   CleanUp:

      ResetData();

   return eXsFailure;
}


////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> 
eXsRetValue CXmlSerializer::Save( LPCTSTR a_szFilename )
{
   if ( NULL == m_ptrDoc )
   {
      // LLOGERR((szXsEventLogComponent, _T("No Current doc to save: %s"), a_szFilename));
      return eXsFailure;
   }

   m_ptrDoc->save(_variant_t(a_szFilename));

   return eXsSuccess;
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> 
eXsRetValue CXmlSerializer::Save( IStream* a_istream )
{
   if ( NULL == m_ptrDoc )
   {
      // LLOGERR((szXsEventLogComponent, _T("No Current doc object to save")));
      return eXsFailure;
   }

   m_ptrDoc->save(a_istream);

   return eXsSuccess;
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> Close all open resources
eXsRetValue CXmlSerializer::Close( void )
{
   ResetData();
   m_ptrDoc = NULL;
   return eXsSuccess;
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> serialize current node text
eXsRetValue CXmlSerializer::SetText( const _bstr_t & a_bstr )
{
   if ( m_ptrCurrentNode == NULL )
   {
      return eXsFailure;
   }

   m_ptrCurrentNode->text =  a_bstr;

   return eXsSuccess;
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> serialize element tag and value
eXsRetValue CXmlSerializer::Serialize( LPCTSTR a_szTagName, LPCTSTR a_szValue )
{
   eXsRetValue eRet = eXsFailure;

   MSXML2::IXMLDOMElementPtr ptrNewNode = m_ptrDoc->createElement( a_szTagName );

   if ( NULL == ptrNewNode )
   {
      // LLOGERR((szXsEventLogComponent, _T("Failed to create element: %s"), a_szTagName));
      eRet = eXsFailure;
      goto CleanUp;
   }

   if ( NULL == m_ptrCurrentNode->appendChild( ptrNewNode ) ) 
   {
      // LLOGERR((szXsEventLogComponent, _T("Failed to append element: %s"), a_szTagName));
      eRet = eXsFailure;
      goto CleanUp;
   }

   ptrNewNode->Puttext( a_szValue );

   return eXsSuccess;

   // cleanup
   CleanUp:

      return eXsFailure;
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> serialize element tag and value
eXsRetValue CXmlSerializer::Serialize( LPCTSTR a_szTagName, int a_nValue )
{
   TCHAR szTemp[50];

   #if _MSC_VER < 1400
      _itot(a_nValue, szTemp, 10 );
   #else
      _itot_s(a_nValue, szTemp, 10 );
   #endif

   return Serialize( a_szTagName, szTemp );
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> serialize element tag and value
eXsRetValue CXmlSerializer::Serialize( LPCTSTR a_szTagName, bool a_bValue )
{
   eXsRetValue eRet = eXsFailure;

   if ( a_bValue )
   {
      eRet = Serialize( a_szTagName, -1);
   }
   else
   {
      eRet = Serialize( a_szTagName, 0);
   }

   return eRet;
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> serialize element tag and value
eXsRetValue CXmlSerializer::Serialize( LPCTSTR a_szTagName, unsigned long a_nValue )
{
   TCHAR szTemp[50];

   #if _MSC_VER < 1400
      _ultot(a_nValue, szTemp, 10);
   #else
     _ultot_s(a_nValue, szTemp, 10);
   #endif

   return Serialize( a_szTagName, szTemp );
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> serialize tag and object
eXsRetValue CXmlSerializer::Serialize( LPCTSTR a_szTagName, IXmlSerialize &obj )
{
   eXsRetValue eRet = eXsFailure;

   // nest into object
   if( Serialize( a_szTagName ) )
   {
      // deserialize object
      eRet = obj.Serialize( *this );
   }

   UnnestCurrentNode();

   return eRet;
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> deserialize current node text
eXsRetValue CXmlSerializer::GetText( _bstr_t & a_bstrtValue )
{
   if ( NULL == m_ptrCurrentNode )
   {
      // LLOGERR((szXsEventLogComponent, _T("No Current Node for deserializing")));
      return eXsFailure;
   }

   a_bstrtValue = m_ptrCurrentNode->Gettext();

   return eXsSuccess;
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> deserialize element tag and value
eXsRetValue CXmlSerializer::Deserialize( LPCTSTR a_szTagName, char & a_char )
{
   eXsRetValue eRet = eXsFailure;

   if ( NULL == m_ptrCurrentNode )
   {
      // LLOGERR((szXsEventLogComponent, _T("No Current Node for deserializing node: %s"), a_szTagName));
      return eRet;
   }

   CString str;

   eRet = Deserialize( a_szTagName, str );

   if ( eRet == eXsSuccess )
   {
      a_char = static_cast<char>(_ttoi( (LPCTSTR)str ));
   }

   return eRet;
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> deserialize element tag and value
eXsRetValue CXmlSerializer::Deserialize( LPCTSTR a_szTagName, short & a_nValue )
{
   eXsRetValue eRet = eXsFailure;

   int nValue = 0;

   eRet = Deserialize( a_szTagName, nValue );

   a_nValue = static_cast<short>(nValue);

   return eRet;
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> deserialize element tag and value
eXsRetValue CXmlSerializer::Deserialize( LPCTSTR a_szTagName, unsigned short & a_nValue )
{
   eXsRetValue eRet = eXsFailure;

   int nValue = 0;

   eRet = Deserialize( a_szTagName, nValue );

   a_nValue = static_cast<unsigned short>(nValue);

   return eRet;
}


////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> deserialize element tag and value
eXsRetValue CXmlSerializer::Deserialize( LPCTSTR a_szTagName, int & a_nInt )
{
   eXsRetValue eRet = eXsFailure;

   if ( NULL == m_ptrCurrentNode )
   {
      // LLOGERR((szXsEventLogComponent, _T("No Current Node for deserializing node: %s"), a_szTagName));
      return eRet;
   }

   CString str;

   eRet = Deserialize( a_szTagName, str );

   if ( eRet == eXsSuccess )
   {
      a_nInt = _ttoi( (LPCTSTR)str );
   }

   return eRet;
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> deserialize element tag and value
eXsRetValue CXmlSerializer::Deserialize( LPCTSTR a_szTagName, bool &a_bValue )
{
   eXsRetValue eRet = eXsFailure;

   int nValue;

   eRet = Deserialize( a_szTagName, nValue );

   a_bValue = (nValue == -1);
 
   return eRet;
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> deserialize element tag and value
eXsRetValue CXmlSerializer::Deserialize( LPCTSTR a_szTagName, unsigned long & a_ulNum )
{
   eXsRetValue eRet = eXsFailure;

   if ( NULL == m_ptrCurrentNode )
   {
      // LLOGERR((szXsEventLogComponent, _T("No Current Node for deserializing node: %s"), a_szTagName ));
      return eRet;
   }

   std::string str;

   eRet = Deserialize( a_szTagName, str );

   if ( eRet == eXsSuccess )
   {
      a_ulNum = strtoul( str.c_str(), NULL,  0 );
   }
   return eXsSuccess;
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> deserialize element tag and value
eXsRetValue CXmlSerializer::Deserialize( LPCTSTR a_szTagName, long & a_nNum )
{
   return Deserialize( a_szTagName, (unsigned long &)a_nNum );
}


////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> deserialize element tag and value
eXsRetValue CXmlSerializer::Deserialize( LPCTSTR a_szTagName, TCHAR * a_szValue, long a_lSize )
{
   eXsRetValue eRet = eXsFailure;

   if ( NULL == m_ptrCurrentNode )
   {
      // LLOGERR((szXsEventLogComponent, _T("No Current Node for deserializing node: %s"), a_szTagName));
      return eRet;
   }

   MSXML2::IXMLDOMElementPtr ptrNode;

   ptrNode = m_ptrCurrentNode->selectSingleNode( _bstr_t(a_szTagName));

   if ( ptrNode )
   {
      _bstr_t bstrtValue;

      bstrtValue  = ptrNode->Gettext();

      _tcsncpy(a_szValue, (LPCTSTR)bstrtValue, a_lSize);

      // memcpy( a_szValue, (LPCTSTR)bstrtValue, a_lSize * sizeof(TCHAR) );
   }
   else
   {
      // LLOGWARN((szXsEventLogComponent, _T("Could not find node : %s"), a_szTagName));
   }
   return eXsSuccess;
}


////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> deserialize element tag and value
eXsRetValue CXmlSerializer::Deserialize( LPCTSTR a_szTagName, std::string& a_szValue )
{
   eXsRetValue eRet = eXsFailure;

   if ( NULL == m_ptrCurrentNode )
   {
      // LLOGERR((szXsEventLogComponent, _T("No Current Node for deserializing node: %s"), a_szTagName));
      return eRet;
   }

   MSXML2::IXMLDOMElementPtr ptrNode;

   ptrNode = m_ptrCurrentNode->selectSingleNode( _bstr_t(a_szTagName));

   if ( ptrNode )
   {
      a_szValue  = ptrNode->Gettext();
   }
   else
   {
      // LLOGWARN((szXsEventLogComponent, _T("Could not find node : %s"), a_szTagName));
   }
   return eXsSuccess;
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> deserialize element tag and value
eXsRetValue CXmlSerializer::Deserialize( LPCTSTR a_szTagName, CString &a_szValue )
{
   if ( NULL == m_ptrCurrentNode )
   {
      // LLOGERR((szXsEventLogComponent, _T("No Current Node for deserializing node: %s"), a_szTagName));
      return eXsFailure;
   }

   MSXML2::IXMLDOMElementPtr ptrNode;

   ptrNode = m_ptrCurrentNode->selectSingleNode( _bstr_t(a_szTagName));

   if ( ptrNode )
   {
      _bstr_t bstrt = ptrNode->Gettext();
      a_szValue = (LPCTSTR)bstrt;
   }
   else
   {
      // LLOGWARN((szXsEventLogComponent, _T("Could not find node : %s"), a_szTagName));
   }

   return eXsSuccess;
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> deserialize element tag and value
eXsRetValue CXmlSerializer::Deserialize( LPCTSTR a_szTagName, _bstr_t & a_bstrtValue )
{
   if ( NULL == m_ptrCurrentNode )
   {
      // LLOGERR((szXsEventLogComponent, _T("No Current Node for deserializing node: %s"), a_szTagName));
      return eXsFailure;
   }

   MSXML2::IXMLDOMElementPtr ptrNode;

   ptrNode = m_ptrCurrentNode->selectSingleNode( _bstr_t(a_szTagName));

   if ( ptrNode )
   {
      a_bstrtValue  = ptrNode->Gettext();
   }
   else
   {
      // LLOGWARN((szXsEventLogComponent, _T("Could not find node : %s"), a_szTagName));
   }

   return eXsSuccess;
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> deserialize collection
long CXmlSerializer::DeserializeCollection( LPCTSTR a_szCollectionTagName )
{
   long lLength = 0;
   m_ptrNodeList = NULL;

   if ( NULL == m_ptrCurrentNode )
   {
      // LLOGERR((szXsEventLogComponent, _T("No Current node(s) for select: %s"), a_szCollectionTagName));
      return 0;
   }

   m_ptrNodeList = m_ptrCurrentNode->getElementsByTagName(_bstr_t(a_szCollectionTagName));

   if ( m_ptrNodeList )
   {
      lLength = m_ptrNodeList->Getlength();
   }

   return lLength;
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> set current item based on index
eXsRetValue CXmlSerializer::SetCollectionItem( const long a_lIndex )
{
   eXsRetValue eRet = eXsFailure;

   if ( NULL == m_ptrNodeList )
   {
      // LLOGERR((szXsEventLogComponent, _T("No Current Nodelist to deserialize")));
      return eXsFailure;
   }

   m_ptrCurrentNode = m_ptrNodeList->Getitem( a_lIndex );

   if ( m_ptrCurrentNode == NULL )
   {
      // LLOGERR((szXsEventLogComponent, _T("Could not get collection item to deserialize: index:%d"), a_lIndex));
      return eXsFailure;
   }
   return eRet; 
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> deserialize a collection item/object
eXsRetValue CXmlSerializer::Deserialize( const long a_lIndex, IXmlSerialize & obj )
{
   eXsRetValue eRet = eXsFailure;

   if ( NULL == m_ptrNodeList )
   {
      // LLOGERR((szXsEventLogComponent, _T("No Current Nodelist to deserialize")));
      return eXsFailure;
   }

   if ( m_ptrCurrentNode = m_ptrNodeList->Getitem( a_lIndex ) )
   {
      eRet = obj.Deserialize( *this );
   }
   else
   {
      // LLOGERR((szXsEventLogComponent, _T("Could not get collection item to deserialize: index:%d"), a_lIndex));
      return eXsFailure;
   }
         
   return eRet; 
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> deserialize a collection chars
eXsRetValue CXmlSerializer::Deserialize( const long a_lIndex, char &a_char )
{
   eXsRetValue eRet = eXsFailure;

   if ( NULL == m_ptrNodeList )
   {
      // LLOGERR((szXsEventLogComponent, _T("No Current Nodelist to deserialize")));
      return eXsFailure;
   }

   if ( m_ptrCurrentNode = m_ptrNodeList->Getitem( a_lIndex ) )
   {
      _bstr_t bstrtValue;

      bstrtValue = m_ptrCurrentNode->Gettext();

      a_char = static_cast<char>(_ttoi( (LPCTSTR)bstrtValue ));
   }
   else
   {
      // LLOGERR((szXsEventLogComponent, _T("Could not get collection item to deserialize: index:%d"), a_lIndex));
      return eXsFailure;
   }
         
   return eRet; 
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> deserialize a collection chars
eXsRetValue CXmlSerializer::Deserialize( const long a_lIndex, unsigned short & a_nValue )
{
   eXsRetValue eRet = eXsFailure;

   if ( NULL == m_ptrNodeList )
   {
      // LLOGERR((szXsEventLogComponent, _T("No Current Nodelist to deserialize")));
      return eXsFailure;
   }

   if ( m_ptrCurrentNode = m_ptrNodeList->Getitem( a_lIndex ) )
   {
      _bstr_t bstrtValue;

      bstrtValue = m_ptrCurrentNode->Gettext();

      a_nValue = static_cast<unsigned short>(_ttoi( bstrtValue ));
   }
   else
   {
      // LLOGERR((szXsEventLogComponent, _T("Could not get collection item to deserialize: index:%d"), a_lIndex));
      return eXsFailure;
   }
         
   return eRet; 
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> deserialize a collection chars
eXsRetValue CXmlSerializer::Deserialize( const long a_lIndex, char *a_char, long a_lSize )
{
   eXsRetValue eRet = eXsFailure;

   if ( NULL == m_ptrNodeList )
   {
      // LLOGERR((szXsEventLogComponent, _T("No Current Nodelist to deserialize")));
      return eXsFailure;
   }

   if ( m_ptrCurrentNode = m_ptrNodeList->Getitem( a_lIndex ) )
   {
      _bstr_t bstrtValue;

      bstrtValue = m_ptrCurrentNode->Gettext();

      strncpy( a_char, (LPCSTR)bstrtValue, a_lSize - 1 );

      a_char[a_lSize - 1] = '\0';
   }
   else
   {
      // LLOGERR((szXsEventLogComponent, _T("Could not get collection item to deserialize: index:%d"), a_lIndex));
      return eXsFailure;
   }
         
   return eRet; 
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> deserialize a collection CStrings
eXsRetValue CXmlSerializer::Deserialize( const long a_lIndex, CString &a_szValue )
{
   eXsRetValue eRet = eXsFailure;

   if ( NULL == m_ptrNodeList )
   {
      // LLOGERR((szXsEventLogComponent, _T("No Current Nodelist to deserialize")));
      return eXsFailure;
   }

   if ( m_ptrCurrentNode = m_ptrNodeList->Getitem( a_lIndex ) )
   {
      _bstr_t bstrtValue;

      bstrtValue = m_ptrCurrentNode->Gettext();

      a_szValue = (LPCSTR)bstrtValue;
   }
   else
   {
      // LLOGERR((szXsEventLogComponent, _T("Could not get collection item to deserialize: index:%d"), a_lIndex));
      return eXsFailure;
   }
         
   return eRet; 
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> deserialize tag and object
eXsRetValue CXmlSerializer::Deserialize( LPCTSTR a_szTagName, IXmlSerialize &obj )
{
   eXsRetValue eRet = eXsFailure;

   if ( NULL != Deserialize( a_szTagName ) )
   {
      eRet = obj.Deserialize(*this );
      UnnestCurrentNode();
   }
   else
   {
      // LLOGDEBUG((szXsEventLogComponent, _T("Node '%s' not found"), a_szTagName));
   }

   return eRet; 
}

////////////////////////////////////////////////////////////////////////
// @cmember,mfunc deserialize attribute tag and data
eXsRetValue CXmlSerializer::DeserializeAttribute( LPCTSTR a_szTagName, LPTSTR a_szValue, int a_nSize )
{
   if ( NULL == m_ptrCurrentNode )
   {
      // LLOGERR((szXsEventLogComponent, _T("No Current Node for deserializing attribute: %s"), a_szTagName));
      return eXsFailure;
   }

   _variant_t var;

   var =  m_ptrCurrentNode->getAttribute( a_szTagName );

   if ( var.vt == VT_BSTR )
   {
      _bstr_t bstrt = var;
      _tcsncpy( a_szValue, (LPCTSTR)bstrt, a_nSize - 1 );
      a_szValue[a_nSize - 1] = '\0';
   }

   return eXsSuccess;
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> deserialize attribute tag and data
eXsRetValue CXmlSerializer::DeserializeAttribute( LPCTSTR a_szTagName, std::string & a_szValue )
{
   if ( NULL == m_ptrCurrentNode )
   {
      // LLOGERR((szXsEventLogComponent, _T("No Current Node for deserializing attribute: %s"), a_szTagName));
      return eXsFailure;
   }

   _variant_t var;

   var =  m_ptrCurrentNode->getAttribute( a_szTagName );

   if ( var.vt == VT_BSTR )
   {
      _bstr_t bstrt = var;
      a_szValue = (LPCSTR) bstrt;
   }
   return eXsSuccess;
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> deserialize attribute tag and data
eXsRetValue CXmlSerializer::DeserializeAttribute( LPCTSTR a_szTagName, CString & a_szValue )
{
   if ( NULL == m_ptrCurrentNode )
   {
      // LLOGERR((szXsEventLogComponent, _T("No Current Node for deserializing attribute: %s"), a_szTagName));
      return eXsFailure;
   }

   _variant_t var;

   var =  m_ptrCurrentNode->getAttribute( a_szTagName );

   if ( var.vt == VT_BSTR )
   {
      _bstr_t bstrt = var;
      a_szValue = (LPCSTR) bstrt;
   }

   return eXsSuccess;
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> deserialize attribute tag and data
eXsRetValue CXmlSerializer::DeserializeAttribute( LPCTSTR a_szTagName, _bstr_t & a_bstrtValue )
{
   if ( NULL == m_ptrCurrentNode )
   {
      // LLOGERR((szXsEventLogComponent, _T("No Current Node for deserializing attribute: %s"), a_szTagName));
      return eXsFailure;
   }

   _variant_t var;

   var =  m_ptrCurrentNode->getAttribute( a_szTagName );

   if ( var.vt == VT_BSTR )
   {
      a_bstrtValue = var;
   }

   return eXsSuccess;
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> deserialize attribute tag and data
eXsRetValue CXmlSerializer::DeserializeAttribute( LPCTSTR a_szTagName, short & a_nValue )
{
   _variant_t var;

   if ( NULL == m_ptrCurrentNode )
   {
      // LLOGERR((szXsEventLogComponent, _T("No Current Node for deserializing attribute: %s"), a_szTagName));
      return eXsFailure;
   }

   var =  m_ptrCurrentNode->getAttribute(a_szTagName);

   if ( var.vt == VT_BSTR )
   {
      _bstr_t bstrt = var;
      a_nValue = static_cast<short>(_ttoi((LPCTSTR) bstrt));
   }

   return eXsSuccess;
}


////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> deserialize attribute tag and data
eXsRetValue CXmlSerializer::DeserializeAttribute( LPCTSTR a_szTagName, int & a_nValue )
{
   _variant_t var;

   if ( NULL == m_ptrCurrentNode )
   {
      // LLOGERR((szXsEventLogComponent, _T("No Current Node for deserializing attribute: %s"), a_szTagName));
      return eXsFailure;
   }

   var =  m_ptrCurrentNode->getAttribute(a_szTagName);

   if ( var.vt == VT_BSTR )
   {
      _bstr_t bstrt = var;
      a_nValue = _ttoi((LPCTSTR) bstrt);
   }

   return eXsSuccess;
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> deserialize attribute tag and data
eXsRetValue CXmlSerializer::DeserializeAttribute( LPCTSTR a_szTagName, bool &a_bValue )
{
   if ( NULL == m_ptrCurrentNode )
   {
      // LLOGERR((szXsEventLogComponent, _T("No Current Node for deserializing attribute: %s"), a_szTagName));
      return eXsFailure;
   }

   _variant_t var;

   var =  m_ptrCurrentNode->getAttribute( a_szTagName );

   if ( var.vt == VT_BSTR )
   {
      _bstr_t bstrt = var;

      if ( _tcscmp( (LPCTSTR) bstrt, _T("0") ) == 0 )
      {
         a_bValue = false;
      }
      else
      {
         a_bValue = true;
      }
   }

   return eXsSuccess;
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> deserialize attribute tag and data
eXsRetValue CXmlSerializer::DeserializeAttribute( LPCTSTR a_szTagName, long & a_lValue )
{
   if ( NULL == m_ptrCurrentNode )
   {
      // LLOGERR((szXsEventLogComponent, _T("No Current Node for deserializing attribute: %s"), a_szTagName));
      return eXsFailure;
   }

   _variant_t var;

   var =  m_ptrCurrentNode->getAttribute(a_szTagName);

   if ( var.vt == VT_BSTR )
   {
      _bstr_t bstrt = var;
      a_lValue = _ttol((LPCTSTR) bstrt);
   }

   return eXsSuccess;
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> deserialize attribute tag and data
eXsRetValue CXmlSerializer::DeserializeAttribute( LPCTSTR a_szTagName, unsigned long & a_ulValue )
{
   if ( NULL == m_ptrCurrentNode )
   {
      // LLOGERR((szXsEventLogComponent, _T("No Current Node for deserializing attribute: %s"), a_szTagName));
      return eXsFailure;
   }

   _variant_t var;

   var =  m_ptrCurrentNode->getAttribute(a_szTagName);

   if ( var.vt == VT_BSTR )
   {
      _bstr_t bstrt = var;
      a_ulValue = _tcstoul((LPCTSTR) bstrt, NULL,  0 );
   }

   return eXsSuccess;
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> serialize attribute tag and data
eXsRetValue CXmlSerializer::SerializeAttribute( LPCTSTR a_szTagName, _variant_t a_varValue )
{
   if ( NULL == m_ptrCurrentNode )
   {
      // LLOGERR((szXsEventLogComponent, _T("No Current Node for setting attribute: %s"), a_szTagName));
      return eXsFailure;
   }

   HRESULT hr = m_ptrCurrentNode->setAttribute(a_szTagName, a_varValue);
   if ( FAILED ( hr ) )
   {
      // LLOGERR((szXsEventLogComponent, _T("Set attribute: %s failed, hr: 0x%p"), a_szTagName, hr));
      return eXsFailure;
   }

   return eXsSuccess;
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> serialize attribute tag and data
//eXsRetValue SerializeAttribute( LPCTSTR a_szTagName, const bool a_bValue )
//{
//   HRESULT hr = E_FAIL;

//   if ( !m_ptrCurrentNode )
//   {
//      // LLOGERR((szXsEventLogComponent, "No Current Node for setting attribute: %s", a_szTagName));
//      return eXsFailure;
//   }

//   if ( a_bValue )
//   {
//      hr = m_ptrCurrentNode->setAttribute(a_szTagName, _T("true"));
//   }
//   else
//   {
//      hr = m_ptrCurrentNode->setAttribute(a_szTagName, _T("false"));
//   }

//   if ( FAILED ( hr ) )
//   {
//      // LLOGERR((szXsEventLogComponent, "Set attribute: %s failed, hr: 0x%p", a_szTagName, hr));
//      return eXsFailure;
//   }
//   return eXsSuccess;
//}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> Get the representative xml text
CString CXmlSerializer::GetXml( void )
{

   if ( NULL == m_ptrDoc )
   {
      // LLOGERR((szXsEventLogComponent, _T("Object not initialized")));
      return CString(_T(""));
   }

   return CString((LPCSTR)(m_ptrDoc->Getxml()));
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> Get the representative xml text
bool CXmlSerializer::GetXml( _bstr_t & a_bstrtXml )
{
   bool bSucceeded = false;

   if ( NULL == m_ptrDoc )
   {
      // LLOGERR((szXsEventLogComponent, _T("Object not initialized")));
      a_bstrtXml = L"";
   }
   else
   {
      a_bstrtXml = m_ptrDoc->Getxml();
   }

   return bSucceeded;
}


////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> Get a deep copy of the contained xml doc object
eXsRetValue CXmlSerializer::CopyDoc( MSXML2::IXMLDOMDocumentPtr& a_xmldoc )
{
   HRESULT hr = S_OK;

   // make sure we got something to work with
   if ( NULL == m_ptrDoc || NULL == a_xmldoc)
   {
      return eXsXmldocNull;
   }

   // make the deep copy
   hr = m_ptrDoc->save(a_xmldoc.GetInterfacePtr());

   // if that failed...
   if (FAILED(hr)) 
   {
      return eXsXmldocSaveFailure;
   }

   return eXsSuccess;
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> returns the internal doc pointer
MSXML2::IXMLDOMDocumentPtr CXmlSerializer::GetDoc()
{
   return m_ptrDoc;
}


////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> 
void CXmlSerializer::ResetData()
{
   m_ptrCurrentNode = NULL;
   m_ptrNodeList = NULL;
   m_ptrDoc = NULL;
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> 
long CXmlSerializer::GetSelectCount( void )
{
   if ( NULL != m_ptrNodeList )
   {
      return m_ptrNodeList->Getlength();
   }
   return 0;
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> 
eXsRetValue CXmlSerializer::UnnestCurrentNode( void )
{
   if ( NULL == m_ptrCurrentNode )
   {
      // LLOGERR((szXsEventLogComponent, _T("Cannot unnest node")));
   }

   m_ptrCurrentNode = m_ptrCurrentNode->GetparentNode();

   if ( NULL != m_ptrCurrentNode )
   {
      return eXsSuccess;
   }

   return eXsFailure;
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> 
bool CXmlSerializer::Serialize( LPCTSTR a_szTagName )
{
   HRESULT hr = E_FAIL;
   MSXML2::IXMLDOMElementPtr ptrNewNode = NULL; 

   CHK_ALLOC( ptrNewNode = m_ptrDoc->createElement(_bstr_t(a_szTagName)) );

   // if we do not have a root node yet
   if ( NULL == m_ptrCurrentNode )
   {
      // append new node to doc
      CHK_ALLOC( m_ptrCurrentNode = m_ptrDoc->appendChild( ptrNewNode ) );
   }
   else
   {
      // append new node to current node
      CHK_ALLOC( m_ptrCurrentNode->appendChild( ptrNewNode ) );
   }

   // new node is now the current node
   m_ptrCurrentNode = ptrNewNode;

   return ptrNewNode != NULL;

CleanUp:
   
   // failed to serialize node
   // LLOGERR((szXsEventLogComponent, _T("Failed to serialize node: %s, error: 0x%p"), a_szTagName, hr));
   
   return ptrNewNode != NULL;
}

////////////////////////////////////////////////////////////////////////
// @mfunc <c CXmlSerializer> 
bool CXmlSerializer::Deserialize( LPCTSTR a_szTagName )
{
   MSXML2::IXMLDOMElementPtr ptrNode = NULL; 

   // if we do not have a root node yet
   if ( NULL == m_ptrCurrentNode  )
   {
      ptrNode = m_ptrDoc->selectSingleNode( _bstr_t(a_szTagName));
   }
   else
   {
      ptrNode = m_ptrCurrentNode->selectSingleNode( _bstr_t(a_szTagName));
   }

   if ( NULL != ptrNode)
   {
      m_ptrCurrentNode = ptrNode;
   }
   else
   {
      // LLOGDEBUG((szXsEventLogComponent, _T("Node '%s' not found"), a_szTagName));
   }

   return ptrNode != NULL;
}


// @cmember,mfunc default constructor
CXmlSerializerEx::CXmlSerializerEx()
{
}

 // @cmember,mfunc copy constructor
CXmlSerializerEx::CXmlSerializerEx(const CXmlSerializerEx & rhs)  : CXmlSerializer( rhs )
{
}


 // @cmember,mfunc assignment
CXmlSerializerEx::CXmlSerializerEx(const CXmlSerializer & src)  : CXmlSerializer( src )
{
}

// @cmember,mfunc intialize object from an XML string
eXsRetValue CXmlSerializerEx::InitFromXml( const CString& a_strXML )
{
   HRESULT hr = E_FAIL;

   ResetData();

   // create DOM
   #if (_MSC_VER < 1910 )
      if (FAILED(hr = m_ptrDoc.CreateInstance(__uuidof(MSXML2::DOMDocument60))))
   #else
      if (FAILED(hr = m_ptrDoc.CreateInstance(_T("Msxml2.DOMDocument.4.0"))))
   #endif
   //if ( FAILED(hr = m_ptrDoc.CreateInstance(_T("Msxml2.DOMDocument.4.0")) ) )
   {
      // LLOGERR((szXsEventLogComponent, _T("Failed to create DOMDocument. hr=%0x0"), hr));
      goto CleanUp;
   }

   // load DOM from file
   if ( (m_ptrDoc->loadXML(_bstr_t(a_strXML))) == VARIANT_TRUE )
   {
      //// LLOGDEBUG((szXsEventLogComponent, _T("DOMDocument loaded successfully from XML string."))); 
      // leaving following line for debug.
      // printf("XML DOM loaded from %s:\n%s\n", a_szFilename, (LPCSTR)m_ptrDoc->xml);
   }
   else
   {
      // Failed to load DOM from file
      // not catastrophic as this may be the first time we need a file.
      // LLOGWARN((szXsEventLogComponent, _T("Warning DOMDocument not initiaized from XML string."))); 
   }

   return eXsSuccess;

   // cleanup
   CleanUp:

      ResetData();

   return eXsFailure;
}


// @cmember,mfunc deserialize element tag and value
eXsRetValue CXmlSerializerEx::DeserializeEx( LPCTSTR a_szTagName, CString& a_szValue )
{
   if ( NULL == m_ptrCurrentNode )
   {
      // LLOGERR((szXsEventLogComponent, _T("No Current Node for deserializing node: %s"), a_szTagName));
      return eXsFailure;
   }

   MSXML2::IXMLDOMElementPtr ptrNode = NULL;

   ptrNode = m_ptrCurrentNode->selectSingleNode( _bstr_t(a_szTagName));

   if ( ptrNode )
   {
      a_szValue  = (LPCSTR)ptrNode->Gettext();
   }
   else
   {
      // LLOGWARN((szXsEventLogComponent, _T("Could not find node : %s"), a_szTagName));
   }

   return eXsSuccess;
}

  // @cmember,mfunc deserialize attribute tag and data
eXsRetValue CXmlSerializerEx::DeserializeAttributeEx( LPCTSTR a_szTagName, CString  &a_szValue )
{
   if ( NULL == m_ptrCurrentNode )
   {
      // LLOGERR((szXsEventLogComponent, _T("No Current Node for deserializing attribute: %s"), a_szTagName));
      return eXsFailure;
   }

   _variant_t var;

   var =  m_ptrCurrentNode->getAttribute( a_szTagName );

   if ( var.vt == VT_BSTR )
   {
      _bstr_t bstrt = var;
      a_szValue = (LPCTSTR) bstrt;
   }

   return eXsSuccess;
}


// @cmember,mfunc 
CString CXmlSerializerEx::GetXmlEx( void )
{
   if ( NULL == m_ptrDoc )
   {
      // LLOGERR((szXsEventLogComponent, _T("Object not initialized")));
      return CString("");
   }

   return CString((LPCSTR)m_ptrDoc->Getxml());
}

#pragma warning( pop ) 


//////////////////////////////////////////////////////////////////////////////
// Module identification
static const char RCSID_XML_SERIALIZER_CPP[] = "$Header: p:/archives/POS/common/XmlUtils/XmlSerializer.cpp 1.17 2015/09/27 13:41:54Z jo185036 Exp $";
