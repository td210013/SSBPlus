#ifndef _PaConfigMsg_TEST_SUITE_H
#define _PaConfigMsg_TEST_SUITE_H

#include "UnitTestBase.h"


class CPaConfigMsgTestSuite : public CUnitTestBase
{
    CPPUNIT_TEST_SUITE(CPaConfigMsgTestSuite);
    CPPUNIT_TEST(Test_Constructor);
    CPPUNIT_TEST(Test_Set1);
    CPPUNIT_TEST(Test_Get1);
    CPPUNIT_TEST_SUITE_END();

public:

    CPaConfigMsgTestSuite();
    virtual ~CPaConfigMsgTestSuite();

    virtual void setUp();
    virtual void tearDown();

    void Test_Constructor();
    void Test_Set1();
    void Test_Get1();

private:
    // CTestingConfigObject *m_pConfigObject;
    // CFakeScotWinApp *m_pApp;
};

#endif

