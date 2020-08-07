#include "StdAfx.h"
#include "PaConfigMsg_TestSuite.h"
#include "PaConfigMsg.h"


CPPUNIT_TEST_SUITE_REGISTRATION(CPaConfigMsgTestSuite);

CPaConfigMsgTestSuite::CPaConfigMsgTestSuite() 
{
}

CPaConfigMsgTestSuite::~CPaConfigMsgTestSuite()
{
}

void CPaConfigMsgTestSuite::setUp() 
{
    CoInitialize(NULL);
    CUnitTestBase::setUp(); 
}

void CPaConfigMsgTestSuite::tearDown() 
{
    CUnitTestBase::tearDown();
    CoUninitialize();

}

//*****************************************************************************
// 
// Test_Constructor
// 
//*****************************************************************************

void CPaConfigMsgTestSuite::Test_Constructor(void)
{
    CPaConfigMsg oPaConfig1;

    CPPUNIT_ASSERT_EQUAL(false, oPaConfig1.bCenterDetection);
    CPPUNIT_ASSERT_EQUAL(false, oPaConfig1.bEnable);
    CPPUNIT_ASSERT_EQUAL(false, oPaConfig1.bScaleMetric);
    CPPUNIT_ASSERT_EQUAL(oPaConfig1.eGeneralFailure, oPaConfig1.m_eStatus);
    CPPUNIT_ASSERT_EQUAL(0L, oPaConfig1.ltimeout);
    CPPUNIT_ASSERT_EQUAL(oPaConfig1.ePA_MODE_0_CONSERVATIVE, oPaConfig1.eMode);
    CPPUNIT_ASSERT_EQUAL(0, wcscmp(L"", (WCHAR*)oPaConfig1.hdr));
}

//*****************************************************************************
// 
// NewTEst
// 
//*****************************************************************************

const WCHAR PA_CONFIG_XML1[] = L"                       \
<config _name=\"PA\" msgid=\"ca\">                      \
    <hdr>                                               \
        <transID></transID>                             \
        <computerName>192.168.163.133</computerName>    \
        <sequenceID>19</sequenceID>                     \
        <lane>1</lane>                                  \
    </hdr>                                              \
    <timeout>3000</timeout>                             \
    <centerDetection>Off</centerDetection>              \
    <enable>On</enable>                                 \
    <mode>0</mode>                                      \
    <scalemetric>Off</scalemetric>                      \
</config>";

void CPaConfigMsgTestSuite::Test_Set1(void)
{
    CPaConfigMsg oPaConfig1;

    HRESULT hr = S_OK;
    IGTGenObjPtr ptrGenObj;

    hr = ptrGenObj.CreateInstance(__uuidof(CGTGenObj));

    CPPUNIT_ASSERT_EQUAL(S_OK, hr);

    hr = ptrGenObj->SetXml(L"/", PA_CONFIG_XML1);

    CPPUNIT_ASSERT_EQUAL(S_OK, hr);

    oPaConfig1.Set(ptrGenObj);

    CPPUNIT_ASSERT_EQUAL(false, oPaConfig1.bCenterDetection );
    CPPUNIT_ASSERT_EQUAL(true, oPaConfig1.bEnable);
    CPPUNIT_ASSERT_EQUAL(false, oPaConfig1.bScaleMetric );
    CPPUNIT_ASSERT_EQUAL(3000L, oPaConfig1.ltimeout);
    CPPUNIT_ASSERT_EQUAL(oPaConfig1.eStatusOk , oPaConfig1.m_eStatus);
    CPPUNIT_ASSERT_EQUAL(oPaConfig1.ePA_MODE_0_CONSERVATIVE, oPaConfig1.eMode);
    CPPUNIT_ASSERT_EQUAL(_bstr_t(L"<message name=\"hdr\"><fields><field name=\"transID\"></field><field name=\"computerName\">192.168.163.133</field><field name=\"sequenceID\">19</field><field name=\"lane\">1</field></fields></message>"), oPaConfig1.hdr);
}

// PA configuration response, note: transID node is usually populated with current data/time, this field is being masked
const WCHAR PA_CONFIG_RESPONSE1[] = L"\
<configResponse _name=\"PA\" msgid=\"cf\">\
<status>success</status>\
<hdr name=\"hdr\">\
<transID name=\"transID\">*</transID>\
<computerName name=\"computerName\">192.168.163.133</computerName>\
<sequenceID name=\"sequenceID\">19</sequenceID>\
<lane name=\"lane\">1</lane>\
</hdr>\
<timeout>3000</timeout>\
<centerDetection>off</centerDetection>\
<enable>on</enable>\
<scalemetric>off</scalemetric>\
<mode>0</mode>\
</configResponse>\
";

void CPaConfigMsgTestSuite::Test_Get1(void)
{
    CPaConfigMsg oPaConfig1;

    oPaConfig1.bCenterDetection = false;

    oPaConfig1.bEnable = true;
    oPaConfig1.bScaleMetric = false;
    oPaConfig1.ltimeout = 3000L;
    oPaConfig1.m_eStatus = oPaConfig1.eStatusOk;
    oPaConfig1.eMode = oPaConfig1.ePA_MODE_0_CONSERVATIVE;
    oPaConfig1.hdr = L"<message name=\"hdr\"><fields><field name=\"transID\"></field><field name=\"computerName\">192.168.163.133</field><field name=\"sequenceID\">19</field><field name=\"lane\">1</field></fields></message>";
    oPaConfig1.bMaskTransId = true;

    HRESULT hr = S_OK;

    IGTGenObjPtr ptrGenObj;

    ptrGenObj = oPaConfig1.Get();

    _bstr_t bstrtXml = ptrGenObj->GetXml(L"/");

    CPPUNIT_ASSERT_EQUAL(_bstr_t(PA_CONFIG_RESPONSE1), bstrtXml);

    // expiremental code...

    IGTGenObjPtr ptrGenObj2;

    hr = ptrGenObj2.CreateInstance(__uuidof(CGTGenObj));

    ptrGenObj2->SetXml(L"", bstrtXml);

    _bstr_t bstrtXml2 = ptrGenObj->GetXml(L"/");


    /*
    hr = ptrGenObj->SetXml(L"/", PA_CONFIG_XML1);

    CPPUNIT_ASSERT_EQUAL(S_OK, hr);

    oPaConfig1.Set(ptrGenObj);

    CPPUNIT_ASSERT_EQUAL(false, oPaConfig1.bCenterDetection);
    CPPUNIT_ASSERT_EQUAL(true, oPaConfig1.bEnable);
    CPPUNIT_ASSERT_EQUAL(false, oPaConfig1.bScaleMetric);
    CPPUNIT_ASSERT_EQUAL(3000L, oPaConfig1.ltimeout);
    CPPUNIT_ASSERT_EQUAL(oPaConfig1.eStatusOk, oPaConfig1.m_eStatus);
    CPPUNIT_ASSERT_EQUAL(oPaConfig1.ePA_MODE_0_CONSERVATIVE, oPaConfig1.eMode);
    CPPUNIT_ASSERT_EQUAL(0, wcscmp(L"<message name=\"hdr\"><fields><field name=\"transID\"></field><field name=\"computerName\">192.168.163.133</field><field name=\"sequenceID\">19</field><field name=\"lane\">1</field></fields></message>", (WCHAR*)oPaConfig1.hdr));
    */
}


