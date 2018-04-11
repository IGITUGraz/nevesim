#include "ResetKernelSpikeResponseTest.h"
#include "ArrayInputNeuron.h"
#include "VariableRecorder.h"
#include "VariableRecorderTypes.h"
#include "EvSimRegularClock.h"

#include <boost/lambda/lambda.hpp>
using namespace boost::lambda;

#include <algorithm>

#include <cmath>

#include <iostream>
using std::cerr;
using std::endl;

// choose one from the following
SIM_LOCAL_AUTOBUILD_TEST_SUITE( ResetKernelSpikeResponseTest )
// SIM_LOCAL_NIGHTLY_TEST_SUITE( ResetKernelSpikeResponseTest )

void ResetKernelSpikeResponseTest::setUp()
{
}

void ResetKernelSpikeResponseTest::tearDown()
{
}

void ResetKernelSpikeResponseTest::testMultipleTimePointsKernel()
{

	EvSimNetwork net(SimParameters(6234098, 1e-3));

	EvSimObject::ID inp_nrn_id = net.create(ArrayInputNeuron());
	EvSimObject::ID rec_clock_id = net.create(EvSimRegularClock(1e-3, true, 1e-3));


	double resp_amplitudes_arr[] = {4,3,2,1};
	SharedDblArray resp_amplitudes(resp_amplitudes_arr, 4);
	double resp_timepoints_arr[] = {1e-3,2e-3,3e-3,4e-3};
	SharedDblArray resp_timepoints(resp_timepoints_arr, 4);

	EvSimObject::ID resp_id = net.create(ResetKernelSpikeResponse(resp_amplitudes, resp_timepoints));
	net.connect(inp_nrn_id, 0, resp_id, 0);
	EvSimObject::ID rec_r_id = net.record(resp_id, "r", rec_clock_id);
	EvSimObject::ID rec_rold_id = net.record(resp_id, "r_old", rec_clock_id);
	EvSimObject::ID rec_next_time_id = net.record(resp_id, "next_expected_time", rec_clock_id);
	EvSimObject::ID rec_cnt_id = net.record(resp_id, "curr_timepoint", rec_clock_id);


	Time inp_ev_times_arr[] = {2.5e-3, 7.5e-3, 8.1e-3, 20.5e-3};
	vector<Time> inp_ev_times(inp_ev_times_arr, inp_ev_times_arr + sizeof(inp_ev_times_arr)/sizeof(Time));

	dynamic_cast<ArrayInputNeuron *>(net.getObject(inp_nrn_id))->setInputEvents(inp_ev_times);

	net.advance(31.1e-3);

	const vector<double> & rec_r_values = dynamic_cast<DoubleVariableRecorder *>(net.getObject(rec_r_id))->getRecordedValues();
	const vector<double> & rec_rold_values = dynamic_cast<DoubleVariableRecorder *>(net.getObject(rec_rold_id))->getRecordedValues();
	const vector<unsigned> &rec_cnt_values = dynamic_cast<UnsignedVariableRecorder *>(net.getObject(rec_cnt_id))->getRecordedValues();
	const vector<double> &rec_next_time_values = dynamic_cast<DoubleVariableRecorder *>(net.getObject(rec_next_time_id))->getRecordedValues();

	double expected_r_values_arr[] = {0,0,4,3,3,  2,2,4,4,3,
	                                  3,2,2,2,1,  1,1,1,0,0,
	                                  4,3,3,2,2,  2,1,1,1,1, 0};
	vector<double> expected_r_values(expected_r_values_arr, expected_r_values_arr + sizeof(expected_r_values_arr)/sizeof(Time));

	double expected_rold_values_arr[] = {0,0,0,4,4,  3,3,2,4,4,
						    		     4,3,3,3,2,  2,2,2,1,1,
									     0,4,4,3,3,  3,2,2,2,2,1};
	vector<double> expected_rold_values(expected_rold_values_arr, expected_rold_values_arr + sizeof(expected_rold_values_arr)/sizeof(Time));

	const unsigned N = ~(unsigned(0));

	unsigned expected_cnt_values_arr[] = {N,N,0,1,1,  2,2,0,0,1,
            							  1,2,2,2,3,  3,3,3,N,N,
            							  0,1,1,2,2,  2,3,3,3,3,N};
	vector<unsigned> expected_cnt_values(expected_cnt_values_arr, expected_cnt_values_arr + sizeof(expected_cnt_values_arr)/sizeof(unsigned));

	const double M = MAX_TIME;


	double expected_next_time_values_arr[] = {M, M, 3.5e-3, 5.5e-3, 5.5e-3,    8.5e-3,8.5e-3,8.5e-3,9.1e-3,11.1e-3,
       										  11.1e-3, 14.1e-3, 14.1e-3, 14.1e-3, 18.1e-3,        18.1e-3,18.1e-3,18.1e-3,M,M,
										      21.5e-3, 23.5e-3, 23.5e-3, 26.5e-3,26.5e-3,  26.5e-3,30.5e-3,30.5e-3,30.5e-3,30.5e-3, M};

	vector<double> expected_next_time_values(expected_next_time_values_arr, expected_next_time_values_arr + sizeof(expected_next_time_values_arr)/sizeof(double));

//	cerr << "rec_r_values :" << endl;
//	std::for_each(rec_r_values.begin(), rec_r_values.end(), cerr << _1 << " , " ); cerr << endl;


	CPPUNIT_ASSERT_VECTOR_DOUBLES_EQUAL(expected_r_values, rec_r_values, 1e-8);

	CPPUNIT_ASSERT_VECTOR_DOUBLES_EQUAL(expected_rold_values, rec_rold_values, 1e-8);

	CPPUNIT_ASSERT_VECTOR_EQUAL(expected_cnt_values, rec_cnt_values);

	CPPUNIT_ASSERT_VECTOR_DOUBLES_EQUAL(expected_next_time_values, rec_next_time_values, 1e-8);

}


void ResetKernelSpikeResponseTest::testOneTimePointKernel()
{
	EvSimNetwork net(SimParameters(6234098, 1e-3));

	EvSimObject::ID rec_clock_id = net.create(EvSimRegularClock(1e-3, true, 1e-3));

	EvSimObject::ID inp_nrn_id = net.create(ArrayInputNeuron());
	double resp_amplitudes_arr[] = {1};
	SharedDblArray resp_amplitudes(resp_amplitudes_arr, 1);
	double resp_timepoints_arr[] = {4e-3};
	SharedDblArray resp_timepoints(resp_timepoints_arr, 1);

	EvSimObject::ID resp_id = net.create(ResetKernelSpikeResponse(resp_amplitudes, resp_timepoints));
	net.connect(inp_nrn_id, 0, resp_id, 0);
	EvSimObject::ID rec_r_id = net.record(resp_id, "r", rec_clock_id);
	EvSimObject::ID rec_rold_id = net.record(resp_id, "r_old", rec_clock_id);


	Time inp_ev_times_arr[] = {2.5e-3, 4.5e-3, 11.5e-3, 17.5e-3};
	vector<Time> inp_ev_times(inp_ev_times_arr, inp_ev_times_arr + sizeof(inp_ev_times_arr)/sizeof(Time));

	dynamic_cast<ArrayInputNeuron *>(net.getObject(inp_nrn_id))->setInputEvents(inp_ev_times);

	net.advance(25.1e-3);

	const vector<double> & rec_r_values = dynamic_cast<DoubleVariableRecorder *>(net.getObject(rec_r_id))->getRecordedValues();
	const vector<double> & rec_rold_values = dynamic_cast<DoubleVariableRecorder *>(net.getObject(rec_rold_id))->getRecordedValues();

	double expected_r_values_arr[] = {0,0,1,1,1,  1,1,1,0,0,
	                                0,1,1,1,1,  0,0,1,1,1,
	                                1,0,0,0,0};
	vector<double> expected_r_values(expected_r_values_arr, expected_r_values_arr + sizeof(expected_r_values_arr)/sizeof(Time));

	double expected_rold_values_arr[] = {0,0,0,0,1,  1,1,1,1,1,
						    		   1,0,0,0,0,  1,1,0,0,0,
									   0,1,1,1,1};
	vector<double> expected_rold_values(expected_rold_values_arr, expected_rold_values_arr + sizeof(expected_rold_values_arr)/sizeof(Time));


	CPPUNIT_ASSERT_VECTOR_DOUBLES_EQUAL(expected_r_values, rec_r_values, 1e-6);

	CPPUNIT_ASSERT_VECTOR_DOUBLES_EQUAL(expected_rold_values, rec_rold_values, 1e-6);

}
