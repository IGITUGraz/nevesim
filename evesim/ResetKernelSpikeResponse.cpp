#include "ResetKernelSpikeResponse.h"

#include <boost/lambda/lambda.hpp>
using namespace boost::lambda;


EV_SIM_OBJECT_POST(ResetKernelSpikeResponse)

FIELD_REGISTRATOR(ResetKernelSpikeResponse)
{
	REGISTER_FIELD(ResetKernelSpikeResponse, r)
	REGISTER_FIELD(ResetKernelSpikeResponse, r_old)
	REGISTER_FIELD(ResetKernelSpikeResponse, next_expected_time);
	REGISTER_FIELD(ResetKernelSpikeResponse, curr_timepoint);
}


void ResetKernelSpikeResponse::eventHit(SimContext& sim_ctxt, Time time, port_id_t port) {
	if ( port == 0 ) {
		curr_timepoint = 0;
	}
	else {
		if ( time == next_expected_time )
			curr_timepoint++;
		else
			return;
	}
	if ((resp_shape_timepoints == 0) || curr_timepoint >= resp_shape_timepoints->size()) {
		curr_timepoint = RESPONSE_NOT_ACTIVE;
		next_expected_time = MAX_TIME;
		r_old = r;
		r = 0;
		return;
	}
	r_old = r;
	r = (*resp_shape_amplitudes)[curr_timepoint];
	next_expected_time = time+(*resp_shape_timepoints)[curr_timepoint];
	outputEvent(sim_ctxt, port_id_t(0), next_expected_time);
}


void ResetKernelSpikeResponse::postAddedInNetwork(EvSimNetwork &net, EvSimObject::ID self)
{
	net.connect(self, 0, self, 1);
}

