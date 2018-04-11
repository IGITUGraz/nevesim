#ifndef EVSIMOBJECTPOOLTEST_H_
#define EVSIMOBJECTPOOLTEST_H_

// includes from cppunit
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>

// includes for our own extensions to cppunit
#include "CppUnitMain.h"

// include what ever you want to test
#include "EvSimObjectPool.h"

class EvSimObjectPoolTest : public CppUnit::TestFixture
{

public:

    CPPUNIT_TEST_SUITE( EvSimObjectPoolTest );

    CPPUNIT_TEST( test );

    CPPUNIT_TEST_SUITE_END();

    void test();

public:
    void setUp();
    void tearDown();

private :

};


#endif /* EVSIMOBJECTPOOLTEST_H_ */
