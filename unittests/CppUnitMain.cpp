#include <cppunit/CompilerOutputter.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/TextOutputter.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/TextTestRunner.h>
#include <cppunit/TextTestProgressListener.h>
#include <cppunit/BriefTestProgressListener.h>
#include <stdexcept>
#include "CppUnitMain.h"

using std::cerr;
using std::endl;
using std::string;

int main( int argc, char* argv[] )
{

    //*******************************************
    // *  get the test suite from the command line
    // ******************************************
    string subSuite = (argc > 1) ? string(argv[1]) : "";
    string testPath = (argc > 2) ? string(argv[2]) : "";

    //*******************************************
    // *  Do local tests
    // ******************************************

    cerr.flush();

    cerr << endl << "-------------------------- Running LOCAL Tests --------------------------" << endl << endl;
    cerr.flush();


    bool localResult = true;


	// create progress listener
	CppUnit::BriefTestProgressListener progress;

	// Add the local suite to the test runner
	CppUnit::TextTestRunner runner;
	runner.addTest( CppUnit::TestFactoryRegistry::getRegistry( subSuite + "LocalTests" ).makeTest() );
	CppUnit::CompilerOutputter *outputter = CppUnit::CompilerOutputter::defaultOutputter( &runner.result(), cerr );
	outputter->setLocationFormat( "%f:%l: " );
	runner.setOutputter( outputter ) ;
	runner.eventManager().addListener( &progress );

	try  {
		std::cerr.flush();
		runner.run( testPath, false, true, false);
	} catch ( std::invalid_argument &e ) {
		cerr << e.what() << endl;
	}
	localResult = runner.result().wasSuccessful();

    std::cout.flush();
    cerr.flush();

}
