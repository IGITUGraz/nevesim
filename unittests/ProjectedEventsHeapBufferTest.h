#ifndef PROJECTEDEVENTSHEAPBUFFERTTEST_H_
#define PROJECTEDEVENTSHEAPBUFFERTTEST_H_

// includes from cppunit
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>

// includes for our own extensions to cppunit
#include "CppUnitMain.h"

// include what ever you want to test
#include "ProjectedEventsHeapBuffer.h"

class ProjectedEventsHeapBufferTest : public CppUnit::TestFixture
{

public:

    CPPUNIT_TEST_SUITE( ProjectedEventsHeapBufferTest );

    CPPUNIT_TEST( testOneObject );

    CPPUNIT_TEST( testRandom );

    CPPUNIT_TEST_SUITE_END();

    void testOneObject();

    void testRandom();

public:
    void setUp();
    void tearDown();

private :

};

#endif /* PROJECTEDEVENTSHEAPBUFFERTTEST_H_ */
