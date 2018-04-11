#ifndef ScheduledEventsQueueTest_H_
#define ScheduledEventsQueueTest_H_

#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>

#include "ScheduledEventsQueue.h"

class ScheduledEventsQueueTest : public CppUnit::TestFixture
{

public:

    CPPUNIT_TEST_SUITE( ScheduledEventsQueueTest );

    CPPUNIT_TEST( testSimple );

    CPPUNIT_TEST( testManyEvents );

    CPPUNIT_TEST_SUITE_END();

public:

    void setUp();

    void tearDown();

    void testSimple();

    void testManyEvents();
private :
    ScheduledEventsQueue *queue;

};

#endif /*ScheduledEventsQueueTest_H_*/
