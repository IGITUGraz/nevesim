#include "BiasAdaptiveHomeostMechanism.h"
#include "EvSimNetwork.h"

EV_SIM_OBJECT_POST( BiasAdaptiveHomeostMechanism )

FIELD_REGISTRATOR( BiasAdaptiveHomeostMechanism )
{
	REGISTER_FIELD( BiasAdaptiveHomeostMechanism, eta )
	REGISTER_FIELD( BiasAdaptiveHomeostMechanism, eta_rt )
	REGISTER_FIELD( BiasAdaptiveHomeostMechanism, rTarget )
}


BiasAdaptiveHomeostMechanism::~BiasAdaptiveHomeostMechanism()
{
	// NOOP
}


void BiasAdaptiveHomeostMechanism::reset(SimContext &sim_ctxt) {
	last_bias_upd_time = 0;
	last_rtgt_upd_time = 0;
	rTarget = 0;
	prev_vm = *vm_ptr;
};


void BiasAdaptiveHomeostMechanism::updateRtarget(Time time)
{
	if (isActive) {
		rTarget += eta_rt * ( mu * (prev_vm > Vthresh) - rTarget ) * (time - last_rtgt_upd_time);
		last_rtgt_upd_time = time;
		prev_vm = *vm_ptr;
	}
}


void BiasAdaptiveHomeostMechanism::eventHit(SimContext & sim_ctxt, Time time, port_id_t port) {
	if (port == 0) {
		if (isActive) {
			double new_bias = *bias_ptr + eta*(rTarget*(time - last_bias_upd_time)- 1);
			if (new_bias <= biasMin)
				new_bias = biasMin;
			else if (new_bias >= biasMax)
				new_bias = biasMax;
			*bias_ptr = new_bias;
			last_bias_upd_time = time;
		}
	}
	else if (port == 1) {
			if (isActive) {
				double new_bias = *bias_ptr + eta*rTarget*(time - last_bias_upd_time);
				if (new_bias <= biasMin)
					new_bias = biasMin;
				else if (new_bias >= biasMax)
					new_bias = biasMax;
				*bias_ptr = new_bias;
				last_bias_upd_time = time;
			}
			updateRtarget(time);
	}
	else if (port == 2) {
		isActive = !isActive;
		if (isActive) {
			last_bias_upd_time = time;
			last_rtgt_upd_time = time;
			prev_vm = *vm_ptr;
		}
	}
};


void BiasAdaptiveHomeostMechanism::updateState(SimContext &sim_context, Time time, update_id_t upd_id)
{
	if (upd_id == 0) {
		updateRtarget(time);
	}
}

void BiasAdaptiveHomeostMechanism::setActive(bool active, Time time)
{
	isActive = active;
	if (isActive) {
		last_bias_upd_time = time;
		last_rtgt_upd_time = time;
		prev_vm = *vm_ptr;
	}
}

void BiasAdaptiveHomeostMechanism::postMountAsMountedObj(EvSimNetwork &net, EvSimObject::ID self, EvSimObject::ID mountpoint)
{
	EvSimObject *tgt = net.getObject(mountpoint);
	bias_ptr = (float *)tgt->getVariablePtr("bias");
	vm_ptr = (double *)tgt->getVariablePtr("Vm");
	net.connect(mountpoint, 0, self, 0);
	net.causalUpdateLink(mountpoint, self, 0u);
}
