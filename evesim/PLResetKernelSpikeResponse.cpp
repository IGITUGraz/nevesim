#include "PLResetKernelSpikeResponse.h"

#include <boost/lambda/lambda.hpp>
using namespace boost::lambda;


EV_SIM_OBJECT_POST(PLResetKernelSpikeResponse)

FIELD_REGISTRATOR(PLResetKernelSpikeResponse)
{
    REGISTER_FIELD(PLResetKernelSpikeResponse, r)
    REGISTER_FIELD(PLResetKernelSpikeResponse, r_old)
    REGISTER_FIELD(PLResetKernelSpikeResponse, slope)
    REGISTER_FIELD(PLResetKernelSpikeResponse, next_expected_time);
    REGISTER_FIELD(PLResetKernelSpikeResponse, curr_timepoint);
}

void PLResetKernelSpikeResponse::eventHit(SimContext& sim_ctxt, Time time, port_id_t port) {

	if ( port == 0 ) {
		curr_timepoint = 0;
	}
	else {
		if ( time == next_expected_time )
			curr_timepoint++;
		else {
            updateState(sim_ctxt, time, 0);
            return;
        }
	}
	if ((resp_shape_timepoints == 0) || curr_timepoint >= resp_shape_timepoints->size()) {
		curr_timepoint = RESPONSE_NOT_ACTIVE;
		next_expected_time = MAX_TIME;
		r_old = r;
		r = 0;
		slope = 0;
		cur_time = time;
		return;
	}
	r_old = r;
	r = (*resp_shape_amplitudes)[curr_timepoint];
	cur_time = time;
	double t_offs = (*resp_shape_timepoints)[curr_timepoint];
    if ((resp_shape_amplitudes == 0) || (curr_timepoint+1) >= resp_shape_amplitudes->size()) {
        slope = 0;
    }
    else {
        if (t_offs > 0.0)
            slope = ((*resp_shape_amplitudes)[curr_timepoint+1] - r)/t_offs;
        else
            slope = 0;
    }
	next_expected_time = time + t_offs;
    outputEvent(sim_ctxt, port_id_t(0), next_expected_time);
}

void PLResetKernelSpikeResponse::updateState(SimContext &sim_info, Time time, update_id_t upd_id) {
    if (time > cur_time) {
        r_old = r;
        r += (time-cur_time)*slope;
        cur_time = time;
    }
}

