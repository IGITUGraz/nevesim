#include "GenericPoissonNeuron.h"
#include "RandomDistribution.h"
#include <cmath>

#include <algorithm>



PoissonNeuronBase::PoissonNeuronBase() {
	// NOOP

}

PoissonNeuronBase::~PoissonNeuronBase() {
	// NOOP
}


void PoissonNeuronBase::announceEvEmitted(SimContext &sim_ctxt, Time time, global_out_port_id_t port_id)
{
	lastSpike = time;
	in_refractory = true;
	changeProjectedEventTime(sim_ctxt, (port_id_t)0, MAX_TIME);
}







