#include "ResetRectSpikeResponseTest.h"
#include "ArrayInputNeuron.h"
#include "VariableRecorder.h"
#include "VariableRecorderTypes.h"

#include <cmath>

#include <iostream>
using std::cerr;
using std::endl;

#include <vector>
using std::vector;

#include <algorithm>
#include <boost/lambda/lambda.hpp>

using namespace boost::lambda;

SIM_LOCAL_AUTOBUILD_TEST_SUITE( ResetRectSpikeResponseTest )
// SIM_LOCAL_NIGHTLY_TEST_SUITE( ResetRectSpikeResponseTest )

void ResetRectSpikeResponseTest::setUp()
{
}

void ResetRectSpikeResponseTest::tearDown()
{
}

void ResetRectSpikeResponseTest::testSimple()
{
	EvSimNetwork net(SimParameters(6234098, 1e-3));

	EvSimObject::ID inp_nrn_id = net.create(ArrayInputNeuron());
	EvSimObject::ID resp_id = net.create(ResetRectSpikeResponse(4e-3));
	net.connect(inp_nrn_id, 0, resp_id, 0);
	EvSimObject::ID rec_r_id = net.record(resp_id, "r");
	EvSimObject::ID rec_rold_id = net.record(resp_id, "r_old");
	EvSimObject::ID rec_cnt_id = net.record(resp_id, "spk_counter");


	Time inp_ev_times_arr[] = {2.5e-3, 4.5e-3, 11.5e-3, 17.5e-3};
	vector<Time> inp_ev_times(inp_ev_times_arr, inp_ev_times_arr + sizeof(inp_ev_times_arr)/sizeof(Time));

	dynamic_cast<ArrayInputNeuron *>(net.getObject(inp_nrn_id))->setInputEvents(inp_ev_times);

	net.advance(25.1e-3);

	const vector<double> & rec_r_values = dynamic_cast<DoubleVariableRecorder *>(net.getObject(rec_r_id))->getRecordedValues();
	const vector<double> & rec_rold_values = dynamic_cast<DoubleVariableRecorder *>(net.getObject(rec_rold_id))->getRecordedValues();
	const vector<unsigned> &rec_cnt_values = dynamic_cast<UnsignedVariableRecorder *>(net.getObject(rec_cnt_id))->getRecordedValues();

	double expected_r_values_arr[] = {0,   0,0,1,1,1,  1,1,1,0,0,
	                                       0,1,1,1,1,  0,0,1,1,1,
	                                       1,0,0,0,0};
	vector<double> expected_r_values(expected_r_values_arr, expected_r_values_arr + sizeof(expected_r_values_arr)/sizeof(Time));

	double expected_rold_values_arr[] = {0,   0,0,0,0,1,  1,1,1,1,1,
						    		          1,0,0,0,0,  1,1,0,0,0,
									          0,1,1,1,1};
	vector<double> expected_rold_values(expected_rold_values_arr, expected_rold_values_arr + sizeof(expected_rold_values_arr)/sizeof(Time));

	unsigned expected_cnt_values_arr[] = {0,  0,0,1,1,2,  2,1,1,0,0,
            							       0,1,1,1,1,  0,0,1,1,1,
            							       1,0,0,0,0};
	vector<unsigned> expected_cnt_values(expected_cnt_values_arr, expected_cnt_values_arr + sizeof(expected_cnt_values_arr)/sizeof(unsigned));

	CPPUNIT_ASSERT_VECTOR_DOUBLES_EQUAL(expected_r_values, rec_r_values, 1e-8);

	CPPUNIT_ASSERT_VECTOR_DOUBLES_EQUAL(expected_rold_values, rec_rold_values, 1e-8);

	CPPUNIT_ASSERT_VECTOR_EQUAL(expected_cnt_values, rec_cnt_values);

}
