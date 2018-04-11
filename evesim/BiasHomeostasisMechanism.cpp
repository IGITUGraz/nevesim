#include "BiasHomeostasisMechanism.h"
#include "EvSimNetwork.h"

EV_SIM_OBJECT_POST( BiasHomeostasisMechanism )


BiasHomeostasisMechanism::~BiasHomeostasisMechanism()
{
	// NOOP
}


void BiasHomeostasisMechanism::reset(SimContext &sim_ctxt) {
	last_update_time = 0;
};


void BiasHomeostasisMechanism::eventHit(SimContext & sim_ctxt, Time time, port_id_t port) {
	if (port == 0) {
		if (isActive) {
			*bias_ptr += eta*(rTarget*(time - last_update_time)- 1);
			last_update_time = time;
		}
	}
	else if (port == 1) {
		if (isActive) {
			*bias_ptr += eta*rTarget*(time - last_update_time);
			last_update_time = time;
		}
	}
	else if (port == 2) {
		isActive = !isActive;
	}

};

void BiasHomeostasisMechanism::postMountAsMountedObj(EvSimNetwork &net, EvSimObject::ID self, EvSimObject::ID mountpoint)
{
	EvSimObject *tgt = net.getObject(mountpoint);
	bias_ptr = (float *)tgt->getVariablePtr("bias");
	net.connect(mountpoint, 0, self, 0);
}
