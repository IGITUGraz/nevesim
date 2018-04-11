#include "DblExpSpikeResponse.h"

EV_SIM_OBJECT_POST(DblExpSpikeResponse)

FIELD_REGISTRATOR(DblExpSpikeResponse)
{
	REGISTER_FIELD(DblExpSpikeResponse, r)
	REGISTER_FIELD(DblExpSpikeResponse, r_old)
	REGISTER_FIELD(DblExpSpikeResponse, r_rise)
	REGISTER_FIELD(DblExpSpikeResponse, r_fall)
	REGISTER_FIELD(DblExpSpikeResponse, last_update_time);
}

const double DblExpSpikeResponse::MIN_VM = 10E-5;

void DblExpSpikeResponse::eventHit(SimContext& sim_ctxt, Time time, port_id_t port) {

	Time t_diff = time - last_update_time;
	last_update_time = time;
	r_rise *= exp(-t_diff/tau_rise);
	r_fall *= exp(-t_diff/tau_fall);
	r_old = r;

	if ( port == 0 ) {
		r_rise += 1.0;
		r_fall += 1.0;
	}
	
	if ( (r_rise<MIN_VM) && (r_fall<MIN_VM) ) {
		r = r_rise = r_fall = 0;
	}
	else {
		r = r_fall - r_rise;
		outputEvent(sim_ctxt, port_id_t(0), time+t_update);
	}
}


void DblExpSpikeResponse::postAddedInNetwork(EvSimNetwork &net, EvSimObject::ID self)
{
	net.connect(self, 0, self, 1);
}

