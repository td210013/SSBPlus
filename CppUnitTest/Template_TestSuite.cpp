#include "StdAfx.h"
#include "Template_TestSuite.h"
#include "Template.h"


CPPUNIT_TEST_SUITE_REGISTRATION(CTemplateTestSuite);

CTemplateTestSuite::CTemplateTestSuite() 
{
}

CTemplateTestSuite::~CTemplateTestSuite()
{
}

void CTemplateTestSuite::setUp() 
{
    CUnitTestBase::setUp(); 
}

void CTemplateTestSuite::tearDown() 
{
    CUnitTestBase::tearDown();
}

//*****************************************************************************
// 
// NewTEst
// 
//*****************************************************************************

void CTemplateTestSuite::NewTest(void)
{
    bool bIsEqual = false;

    CTemplate oTemplate1;

    oTemplate1.m_nMemberVar++;

    // Test copy constructor...
    //
    CTemplate oTemplate2(oTemplate1);

    bIsEqual = oTemplate1.IsEqual(oTemplate2);

    CPPUNIT_ASSERT_EQUAL(true, bIsEqual );

    CTemplate oTemplate3;

    bIsEqual = oTemplate3.IsEqual( oTemplate1 );

    CPPUNIT_ASSERT_EQUAL(false, bIsEqual );

    // Test assignment operator...
    //
    oTemplate3 = oTemplate1;

    bIsEqual = oTemplate3.IsEqual( oTemplate1 );

    CPPUNIT_ASSERT_EQUAL(true, bIsEqual );

}

