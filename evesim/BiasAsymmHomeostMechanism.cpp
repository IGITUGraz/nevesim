#include "BiasAsymmHomeostMechanism.h"
#include "EvSimNetwork.h"

EV_SIM_OBJECT_POST( BiasAsymmHomeostMechanism )

FIELD_REGISTRATOR( BiasAsymmHomeostMechanism )
{
	REGISTER_FIELD( BiasAsymmHomeostMechanism, eta_plus )
	REGISTER_FIELD( BiasAsymmHomeostMechanism, eta_minus )
	REGISTER_FIELD( BiasAsymmHomeostMechanism, rTarget )
	REGISTER_FIELD( BiasAsymmHomeostMechanism, r_estim )
}


BiasAsymmHomeostMechanism::~BiasAsymmHomeostMechanism()
{
	// NOOP
}


void BiasAsymmHomeostMechanism::reset(SimContext &sim_ctxt) {
	last_update_time = 0;
	r_estim = 0;
};


void BiasAsymmHomeostMechanism::eventHit(SimContext & sim_ctxt, Time time, port_id_t port) {
	if (port == 0 || port == 1) {
		if (isActive) {
			r_estim *= C(time - last_update_time);
			if (port == 0)
				r_estim += 1.0/tau;

			double eta = (rTarget > r_estim) ? eta_plus: eta_minus;

			*bias_ptr += eta*(rTarget - r_estim);
			last_update_time = time;
		}
	}
	else if (port == 2) {
		isActive = !isActive;
	}
};

void BiasAsymmHomeostMechanism::postMountAsMountedObj(EvSimNetwork &net, EvSimObject::ID self, EvSimObject::ID mountpoint)
{
	EvSimObject *tgt = net.getObject(mountpoint);
	bias_ptr = (float *)tgt->getVariablePtr("bias");
	net.connect(mountpoint, 0, self, 0);
}
