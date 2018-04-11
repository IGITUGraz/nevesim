#ifndef EVSIMNETWORKTEST_H_
#define EVSIMNETWORKTEST_H_

// includes from cppunit
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>

// includes for our own extensions to cppunit
#include "CppUnitMain.h"

// include what ever you want to test
#include "EvSimNetwork.h"

class EvSimNetworkTest : public CppUnit::TestFixture
{

public:

    CPPUNIT_TEST_SUITE( EvSimNetworkTest );

    CPPUNIT_TEST( testCreateAddConnect );

    CPPUNIT_TEST( testAdvanceAndRecordEvents );

    CPPUNIT_TEST( testAdvanceWithProjectedPorts );

    CPPUNIT_TEST( testRecordVariables );

    CPPUNIT_TEST( testAdvanceRandomized );


    CPPUNIT_TEST_SUITE_END();

    void testCreateAddConnect();

    void testAdvanceAndRecordEvents();

    void testRecordVariables();

    void testAdvanceWithProjectedPorts();

    void testAdvanceRandomized();

public:
    void setUp();
    void tearDown();

private :

};

#endif /* EVSIMNETWORKTEST_H_ */
