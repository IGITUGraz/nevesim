#include "BinomNSamplSEMSynapseTest.h"

#include <cmath>

#include <iostream>
using std::cerr;
using std::endl;

#include <vector>
using std::vector;

#include <boost/lambda/lambda.hpp>
using namespace boost::lambda;

#include <algorithm>

// choose one from the following
SIM_LOCAL_AUTOBUILD_TEST_SUITE( BinomNSamplSEMSynapseTest )
// SIM_LOCAL_NIGHTLY_TEST_SUITE( BinomNSamplSEMSynapseTest )

#include "ExpPoissonNeuron.h"
#include "NeurSamplingNetwork.h"
#include "ArrayInputNeuron.h"
#include "BinaryArrayInputSynapse.h"
#include "ExpPoissonNeuron.h"
#include "VariableRecorderTypes.h"
#include "EventTimesRecorder.h"

void BinomNSamplSEMSynapseTest::setUp()
{

}

void BinomNSamplSEMSynapseTest::tearDown()
{

}

void BinomNSamplSEMSynapseTest::test()
{
	NeurSamplingNetwork net(SimParameters(14028402, 10));


	double pre_spikes_arr[] = { 20e-3, 60e-3, 160e-3 };
	double switcher_spikes_arr[] = { 30e-3, 35e-3, 100e-3, 105e-3, 150e-3, 155e-3 };

	vector<double> pre_spikes(pre_spikes_arr, pre_spikes_arr + 3);
	vector<double> switcher_spikes(switcher_spikes_arr, switcher_spikes_arr + 6);

	EvSimObject::ID pre_nrn = net.create(ArrayInputNeuron(pre_spikes));
	EvSimObject::ID switcher_nrn = net.create(ArrayInputNeuron(switcher_spikes));

	EvSimObject::ID post_nrn = net.create(ExpPoissonNeuron(50, -10, 10e-3));

	bool switching_arr[] = {true, false, true, false, true, false};
	vector<bool> switching_vec(switching_arr, switching_arr+6);

	EvSimObject::ID bin_syn = net.mount(BinaryArrInpActiveSynapse(switching_vec, 20), post_nrn);

	net.connect(switcher_nrn, 0, bin_syn, 0);

	EvSimObject::ID syn_id = net.connect(pre_nrn, post_nrn,
						           CompositeSynapse(BinomNSamplSEMActiveSynapse(1,100),
								                    ResetRectSpikeResponse(20e-3),
								                        ResetRectSpikeResponse(20e-3)));

	double rec_clock_ticks_arr[] = { 35e-3, 45e-3, 55e-3, 65e-3, 75e-3, 105e-3, 125e-3, 155e-3, 165e-3, 175e-3, 185e-3 };
	vector<double> rec_clock_ticks(rec_clock_ticks_arr, rec_clock_ticks_arr + sizeof(rec_clock_ticks_arr)/sizeof(double));

	EvSimObject::ID rec_clock_id = net.create(ArrayInputNeuron(rec_clock_ticks));

	EvSimObject::ID rec_id = net.record(syn_id, "W", rec_clock_id);

	EvSimObject::ID post_rec_id = net.record(post_nrn);

	net.simulate(200e-3);

	const vector<double> & rec_vals = dynamic_cast<DoubleVariableRecorder *>(net.getObject(rec_id))->getRecordedValues();

	const std::vector<Time> & post_rec_vals = dynamic_cast<EventTimesRecorder *>(net.getObject(post_rec_id))->getRecordedValues();

	double expected_rec_values_arr[] = { 1, 1.26893, 0.488325, 0.488325, 0.488325, 0.488325, -0.751098, -0.751098, -1.07161, -0.326544, -0.326544};
	vector<double> expected_rec_values(expected_rec_values_arr,
			          expected_rec_values_arr + sizeof(expected_rec_values_arr)/sizeof(double));

	CPPUNIT_ASSERT_VECTOR_DOUBLES_EQUAL( expected_rec_values, rec_vals, 1e-3 );

/*
	cerr << "1st expected values:" << endl;
	std::copy(expected_rec_values.begin(), expected_rec_values.end(),
			std::ostream_iterator<double>(std::cerr, " "));
	cerr << endl;

	cerr << "1st actual_values:" << endl;
		std::copy(rec_vals.begin(), rec_vals.end(),
				std::ostream_iterator<double>(std::cerr, " "));
	cerr << endl;
*/

	double expected_post_rec_val_arr[] = { 30e-3, 100e-3, 150e-3 };
	vector<double> expected_post_rec_val(expected_post_rec_val_arr,
					  expected_post_rec_val_arr + sizeof(expected_post_rec_val_arr)/sizeof(double));

	CPPUNIT_ASSERT_VECTOR_DOUBLES_EQUAL( expected_post_rec_val, post_rec_vals, 1e-3 );
/*
	cerr << "expected values:" << endl;
	std::copy(expected_post_rec_val.begin(), expected_post_rec_val.end(),
	        std::ostream_iterator<double>(std::cerr, " "));
	cerr << endl;

	cerr << "actual_values:" << endl;
		std::copy(post_rec_vals.begin(), post_rec_vals.end(),
		        std::ostream_iterator<double>(std::cerr, " "));
	cerr << endl;
*/
}
