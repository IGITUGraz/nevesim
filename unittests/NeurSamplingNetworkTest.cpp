#include "NeurSamplingNetworkTest.h"

#include "GenericPoissonNeuron.h"
#include "ExpPoissonNeuron.h"
#include "BasicSynapse.h"
#include "ResetRectSpikeResponse.h"
#include "CompositeSynapse.h"
#include "EventTimesRecorder.h"


#include <cmath>

#include <numeric>
#include <algorithm>

#include <iostream>
using std::cerr;
using std::endl;

#include <boost/lambda/lambda.hpp>
using namespace boost::lambda;

// choose one from the following
SIM_LOCAL_AUTOBUILD_TEST_SUITE( NeurSamplingNetworkTest )
// SIM_LOCAL_NIGHTLY_TEST_SUITE( NeurSamplingNetworkTest )

void NeurSamplingNetworkTest::setUp()
{
}

void NeurSamplingNetworkTest::tearDown()
{
}

void NeurSamplingNetworkTest::testSimple2NeuronNetwork()
{
	SimParameters sim_params(1230129381, 1.0);
	NeurSamplingNetwork net(sim_params);

	const double tau = 20e-3;

	EvSimObject::IDVector nrn_ids = net.create(ExpPoissonNeuron(1.0/tau, log(2), tau), 2);
	dynamic_cast<ExpPoissonNeuron *>(net.getObject((*nrn_ids)[1]))->bias = 0;


	net.connect((*nrn_ids)[0],(*nrn_ids)[1], CompositeSynapse(BasicActiveSynapse(log(3)),
			                            		              ResetRectSpikeResponse(tau)));

	net.connect((*nrn_ids)[1],(*nrn_ids)[0], CompositeSynapse(BasicActiveSynapse(log(3)),
				                            		           ResetRectSpikeResponse(tau)));

	EvSimObject::IDVector rec_ids = net.record(nrn_ids);

	const double sim_time = 3000;

	net.simulate(sim_time);

	const std::vector<Time> & spike_times1 = dynamic_cast<EventTimesRecorder *>(net.getObject((*rec_ids)[0]))->getRecordedValues();

	const std::vector<Time> & spike_times2 = dynamic_cast<EventTimesRecorder *>(net.getObject((*rec_ids)[1]))->getRecordedValues();

	vector<double> intervals1, intervals2;


	std::adjacent_difference(spike_times1.begin(), spike_times1.end(), std::back_inserter(intervals1));
	vector<double>::iterator min_it = std::min_element(intervals1.begin()+1, intervals1.end());

	CPPUNIT_ASSERT( *min_it > 20e-3 );

	std::adjacent_difference(spike_times2.begin(), spike_times2.end(), std::back_inserter(intervals2));
	min_it = std::min_element(intervals2.begin()+1, intervals2.end());

	CPPUNIT_ASSERT( *min_it > 20e-3 );


	// then just count the spikes
	double p_1 = spike_times1.size() * tau / sim_time;

	double p_2 = spike_times2.size() * tau / sim_time;

	CPPUNIT_ASSERT_DOUBLES_EQUAL( 8.0/10.0, p_1, 1e-3);

	CPPUNIT_ASSERT_DOUBLES_EQUAL( 7.0/10.0, p_2, 1e-3);

}
