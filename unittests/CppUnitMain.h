/*! \file  CppUnitMain.h
**  \brief Definition of the test suite hierarchy for the evesim C++ core
*/

/*!

\page cppunit Unit tests for evesim


  For evesm we introduce the following test suite hierarchy:

\verbatim
  "All Tests"
		 +------ AutoBuildLocalTests
		 +------ NightlyLocalTests
\endverbatim

 CppUnitMain can be used to run either the AutoBuild or the Nightly part of the local or mpi tests:

   - \b CppUnitMain   runs *all* tests
   - \b CppUnitMain \b AutoBuild   runs the local auto-build tests
   - \b CppUnitMain \b Nightly   runs the local nightly tests

 One can also directly specify a single test. E.g.
 \code CppUnitMain AutoBuild BufferTest \endcode
 runs only BufferTest.
 
 The idea is to have fast tests in the auto-build suites since this suites will be run on each
 \b make \b all   command which is issued by the auto build of Eclipse.

 Tests which take very long should be put into the nightly suites.

 Use one of the following macros to add tests to a certain test suite:
   
   - SIM_LOCAL_AUTOBUILD_TEST_SUITE
   - SIM_LOCAL_NIGHTLY_TEST_SUITE

*/

#ifndef CPPUNITMAIN_H_
#define CPPUNITMAIN_H_

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/ui/text/TestRunner.h>

#include <string>
using std::string;


#define SIM_LOCAL_TEST_SUITE( ts )           CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( ts, SimTestSuites::localTests() );
#define SIM_LOCAL_AUTOBUILD_TEST_SUITE( ts ) CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( ts, SimTestSuites::autoBuildLocalTests() );
#define SIM_LOCAL_NIGHTLY_TEST_SUITE( ts )   CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( ts, SimTestSuites::nightlyLocalTests() );

class SimTestSuites
{
public:
    static string localTests() { return string("LocalTests"); }
    static string autoBuildLocalTests() { return string("AutoBuildLocalTests"); }
    static string nightlyLocalTests() { return string("NightlyLocalTests"); }
};

CPPUNIT_REGISTRY_ADD_TO_DEFAULT( SimTestSuites::localTests() );
CPPUNIT_REGISTRY_ADD( SimTestSuites::autoBuildLocalTests(), SimTestSuites::localTests() );
CPPUNIT_REGISTRY_ADD( SimTestSuites::nightlyLocalTests(),   SimTestSuites::localTests() );

#define SIM_CPPUNIT_TEST( T ) CPPUNIT_TEST( T ); public: void T(void);

#define CPPUNIT_ASSERT_VECTOR_EQUAL( V1, V2 )                                                \
	CPPUNIT_ASSERT_EQUAL( V1.size(), V2.size() );											 \
	for (unsigned i_i_i = 0; i_i_i < (unsigned)V1.size(); ++i_i_i) {                         \
	     CPPUNIT_ASSERT_EQUAL( V1[i_i_i], V2[i_i_i] );                                       \
	}                                                                                        \

	
#define CPPUNIT_ASSERT_VECTOR_DOUBLES_EQUAL( V1, V2, delta )								 \
	CPPUNIT_ASSERT_EQUAL( (V1).size(), (V2).size() );				    					 \
	for (unsigned i_i_i = 0; i_i_i < (unsigned)(V1).size(); ++i_i_i) {                       \
	     CPPUNIT_ASSERT_DOUBLES_EQUAL( (V1)[i_i_i], (V2)[i_i_i], delta );  					 \
	}                                                                                        \





#endif /*CPPUNITMAIN_H_*/
