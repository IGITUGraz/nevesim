#ifndef RESETRECTSPIKERESPONSETEST_H_
#define RESETRECTSPIKERESPONSETEST_H_

// includes from cppunit
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>

// includes for our own extensions to cppunit
#include "CppUnitMain.h"

// include what ever you want to test
#include "ResetRectSpikeResponse.h"

class ResetRectSpikeResponseTest : public CppUnit::TestFixture
{

public:

    CPPUNIT_TEST_SUITE( ResetRectSpikeResponseTest );

    CPPUNIT_TEST( testSimple );

    CPPUNIT_TEST_SUITE_END();

    void testSimple();

public:
    void setUp();
    void tearDown();

private :

};

#endif /* RESETRECTSPIKERESPONSETEST_H_ */
