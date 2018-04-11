#ifndef PROPAGATEDUPDATELISTTEST_H_
#define PROPAGATEDUPDATELISTTEST_H_

// includes from cppunit
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>

// includes for our own extensions to cppunit
#include "CppUnitMain.h"

// include what ever you want to test
#include "PropagatedUpdateList.h"

class PropagatedUpdateListTest : public CppUnit::TestFixture
{

    CPPUNIT_TEST_SUITE( PropagatedUpdateListTest );

    CPPUNIT_TEST( test2neurons );

    CPPUNIT_TEST( testManySynapsesToOneNeuron );

    CPPUNIT_TEST( testRandom );

    CPPUNIT_TEST( testCircle );


    CPPUNIT_TEST_SUITE_END();

public:
    void test2neurons();

    void testManySynapsesToOneNeuron();

    void testRandom();

    void testCircle();


    void setUp();
    void tearDown();

protected:


    void initUpdateBinaryMap(vector<vector<bool > > &upd_bin_map, unsigned nRows, unsigned nCols);
};

#endif /* PROPAGATEDUPDATELISTTEST_H_ */
