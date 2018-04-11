#include "BiasHomeostasisMechanismTest.h"
#include "EvSimNetwork.h"
#include "BiasHomeostasisMechanism.h"
#include "ExpPoissonNeuron.h"
#include "VariableRecorderTypes.h"

#include <cmath>

#include <iostream>
using std::cerr;
using std::endl;

// choose one from the following
SIM_LOCAL_AUTOBUILD_TEST_SUITE( BiasHomeostasisMechanismTest )
// SIM_LOCAL_NIGHTLY_TEST_SUITE( BiasHomeostasisMechanismTest )

void BiasHomeostasisMechanismTest::setUp()
{
}

void BiasHomeostasisMechanismTest::tearDown()
{
}

void BiasHomeostasisMechanismTest::test()
{
	EvSimNetwork net(SimParameters(123028309, 10e-3));

	float rTarget = 30;
	float Trefract = 10e-3;


	EvSimObject::ID nrn_id = net.create(ExpPoissonNeuron(1,0, Trefract, 1));

	net.mount(BiasHomeostasisMechanism(2e-3, rTarget, true), nrn_id);

	EvSimObject::ID rec_id = net.record(nrn_id, "bias");

	net.simulate(500);

	const vector<float> & rec_v = dynamic_cast<FloatVariableRecorder *>(net.getObject(rec_id))->getRecordedValues();

	CPPUNIT_ASSERT_DOUBLES_EQUAL(  log(rTarget /(1 - rTarget*Trefract)) , rec_v.back(), 5e-2 );

}
