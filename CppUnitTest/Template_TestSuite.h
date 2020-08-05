#ifndef _TEMPLATE_TEST_SUITE_H
#define _TEMPLATE_TEST_SUITE_H

#include "UnitTestBase.h"


class CTemplateTestSuite : public CUnitTestBase
{
    CPPUNIT_TEST_SUITE(CTemplateTestSuite);
    CPPUNIT_TEST(NewTest);
    CPPUNIT_TEST_SUITE_END();

public:

    CTemplateTestSuite();
    virtual ~CTemplateTestSuite();

    virtual void setUp();
    virtual void tearDown();

    void NewTest();

private:
    // CTestingConfigObject *m_pConfigObject;
    // CFakeScotWinApp *m_pApp;
};

#endif

