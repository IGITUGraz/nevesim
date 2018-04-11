#include "ResetRectSpikeResponse.h"

EV_SIM_OBJECT_POST(ResetRectSpikeResponse)

FIELD_REGISTRATOR(ResetRectSpikeResponse)
{
	REGISTER_FIELD(ResetRectSpikeResponse, r)
	REGISTER_FIELD(ResetRectSpikeResponse, r_old)
	REGISTER_FIELD(ResetRectSpikeResponse, spk_counter)
}


void ResetRectSpikeResponse::eventHit(SimContext& sim_ctxt, Time time, port_id_t port) {
	if (port==0) {
		r_old = r;
		r = 1;
		spk_counter++;
		outputEvent(sim_ctxt, port_id_t(0), time + tau);
	}
	else {
		r_old = r;
		spk_counter--;
		if (!spk_counter)
			r = 0;
	}
}
