#ifndef RESETKERNELSPIKERESPONSETEST_H_
#define RESETKERNELSPIKERESPONSETEST_H_

// includes from cppunit
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>

// includes for our own extensions to cppunit
#include "CppUnitMain.h"

// include what ever you want to test
#include "ResetKernelSpikeResponse.h"

class ResetKernelSpikeResponseTest : public CppUnit::TestFixture
{

public:
    CPPUNIT_TEST_SUITE( ResetKernelSpikeResponseTest );

    CPPUNIT_TEST( testMultipleTimePointsKernel );

    CPPUNIT_TEST( testOneTimePointKernel );

    CPPUNIT_TEST_SUITE_END();

    void testMultipleTimePointsKernel();

    void testOneTimePointKernel();


public:
    void setUp();
    void tearDown();

private :

};

#endif /* RESETKERNELSPIKERESPONSETEST_H_ */
