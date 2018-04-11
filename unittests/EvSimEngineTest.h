#ifndef EVSIMENGINETEST_H_
#define EVSIMENGINETEST_H_

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
#include "EvSimEngine.h"

class EvSimEngineTest : public CppUnit::TestFixture
{
public:

    CPPUNIT_TEST_SUITE( EvSimEngineTest );

    CPPUNIT_TEST( testRegisterProjOutPorts );

    CPPUNIT_TEST( testDeliverScheduledEvent );

    CPPUNIT_TEST( testScheduleEvent );

    CPPUNIT_TEST_SUITE_END();

public:
    void testRegisterProjOutPorts();

    void testDeliverScheduledEvent();

    void testScheduleEvent();

    void setUp();
    void tearDown();

private:

};

#endif /* EVSIMENGINETEST_H_ */
