#ifndef BinomNSamplSEMSYNAPSETEST_H_
#define BinomNSamplSEMSYNAPSETEST_H_

// includes from cppunit
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>

// includes for our own extensions to cppunit
#include "CppUnitMain.h"

// include what ever you want to test
#include "BinomNSamplSEMSynapse.h"

class BinomNSamplSEMSynapseTest : public CppUnit::TestFixture
{

public:

    CPPUNIT_TEST_SUITE( BinomNSamplSEMSynapseTest );

    CPPUNIT_TEST( test );

    CPPUNIT_TEST_SUITE_END();

    void test();

public:
    void setUp();
    void tearDown();

private :

};

#endif /* BinomNSamplSEMSYNAPSETEST_H_ */
