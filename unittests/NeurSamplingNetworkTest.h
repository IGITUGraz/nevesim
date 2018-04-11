#ifndef NEURSAMPLINGNETWORKTEST_H_
#define NEURSAMPLINGNETWORKTEST_H_

// includes from cppunit
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>

// includes for our own extensions to cppunit
#include "CppUnitMain.h"

// include what ever you want to test
#include "NeurSamplingNetwork.h"

class NeurSamplingNetworkTest : public CppUnit::TestFixture
{

public:

    CPPUNIT_TEST_SUITE( NeurSamplingNetworkTest );

    CPPUNIT_TEST( testSimple2NeuronNetwork );

    CPPUNIT_TEST_SUITE_END();

    void testSimple2NeuronNetwork();

public:
    void setUp();
    void tearDown();

private :

};

#endif /* NEURSAMPLINGNETWORKTEST_H_ */
