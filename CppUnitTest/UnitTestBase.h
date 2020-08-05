#ifndef _CUNIT_TEST_BASE_H
#define _CUNIT_TEST_BASE_H

#include "cppunit/extensions/HelperMacros.h"
#include "cppunit/TestFixture.h"
#include "cppunit/Test.h"

// #include "FakeCommon.h"

using namespace CppUnit;

// class CFakeRegistry;

/**
 * \class CUnitTestBase 
 * \brief Common base class for unit tests.  Things that are common to 
 *        all of our tests go here.
 */


class CUnitTestBase : public TestFixture
{
public:
    CUnitTestBase();
    virtual ~CUnitTestBase();

    virtual void setUp();
    virtual void tearDown();

protected:
    // CFakeRegistry *m_pFakeRegistry; // Fake registry.
};

#endif // _CUNIT_TEST_BASE_H
