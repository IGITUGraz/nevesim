#include "BiasWakeSleepPlasticityMechanism.h"
#include "EvSimNetwork.h"

EV_SIM_OBJECT_POST( BiasWakeSleepPlasticityMechanism )

FIELD_REGISTRATOR( BiasWakeSleepPlasticityMechanism )
{
	REGISTER_FIELD( BiasWakeSleepPlasticityMechanism, sign )
	REGISTER_FIELD( BiasWakeSleepPlasticityMechanism, eta )
	REGISTER_FIELD( BiasWakeSleepPlasticityMechanism, eta_ratio )
}

BiasWakeSleepPlasticityMechanism::~BiasWakeSleepPlasticityMechanism()
{
	// NOOP
}


void BiasWakeSleepPlasticityMechanism::reset(SimContext &sim_ctxt) {
};


void BiasWakeSleepPlasticityMechanism::eventHit(SimContext & sim_ctxt, Time time, port_id_t port) {
	if (port == 0) {
		if (isActive) {
			if (sign > 0)
				*bias_ptr += eta*sign;
			else
				*bias_ptr += eta*eta_ratio*sign;
			if (*bias_ptr > biasMax)
				*bias_ptr = biasMax;
			else if (*bias_ptr < biasMin)
				*bias_ptr = biasMin;
		}
	}
	else if (port == 1) {
		isActive = !isActive;
	}
	else if (port == 2) {
		sign = -sign;
	}
};

void BiasWakeSleepPlasticityMechanism::postMountAsMountedObj(EvSimNetwork &net, EvSimObject::ID self, EvSimObject::ID mountpoint)
{
	EvSimObject *tgt = net.getObject(mountpoint);
	bias_ptr = (float *)tgt->getVariablePtr("bias");
	net.connect(mountpoint, 0, self, 0);
}
