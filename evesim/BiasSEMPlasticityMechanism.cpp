#include "BiasSEMPlasticityMechanism.h"
#include "EvSimNetwork.h"

#include <algorithm>

using std::min;

EV_SIM_OBJECT_POST( BiasSEMPlasticityMechanism )

FIELD_REGISTRATOR( BiasSEMPlasticityMechanism )
{
	REGISTER_FIELD( BiasSEMPlasticityMechanism, eta )
}

BiasSEMPlasticityMechanism::~BiasSEMPlasticityMechanism()
{
	// NOOP
}


void BiasSEMPlasticityMechanism::reset(SimContext &sim_ctxt) {
	last_update_time = 0;
};


void BiasSEMPlasticityMechanism::eventHit(SimContext & sim_ctxt, Time time, port_id_t port) {
	if (port == 0) {
		if (isActive) {
			double b = T *(*bias_ptr) + L;
			double m = 1;
			if (withHessian) {
				m = exp(-b);
			}
			*bias_ptr -= min( eta*m*exp(b)*(time - last_update_time), double(maxDelta));
			*bias_ptr += min( eta*m*tau, double(maxDelta));
			if (*bias_ptr > biasMax) *bias_ptr = biasMax;
			if (*bias_ptr < biasMin) *bias_ptr = biasMin;
			last_update_time = time;
		}
	}
	else if (port == 1) {
		if (isActive) {
			double b = T*(*bias_ptr) + L;
			double m = 1;
			if (withHessian) {
				m = exp(-b);
			}
			*bias_ptr -= min( eta*m*exp(b)*(time - last_update_time), double(maxDelta));
			if (*bias_ptr > biasMax) *bias_ptr = biasMax;
			if (*bias_ptr < biasMin) *bias_ptr = biasMin;
			last_update_time = time;
		}
	}
	else if (port == 2) {
		isActive = !isActive;
		last_update_time = time;
	}
};

void BiasSEMPlasticityMechanism::activate()
{
	Time time = thread_active_sim_ctxt->t;
	if (!isActive) {
		last_update_time = time;
	}
	isActive = true;
}


void BiasSEMPlasticityMechanism::postMountAsMountedObj(EvSimNetwork &net, EvSimObject::ID self, EvSimObject::ID mountpoint)
{
	EvSimObject *tgt = net.getObject(mountpoint);
	bias_ptr = (float *)tgt->getVariablePtr("bias");
	net.connect(mountpoint, 0, self, 0);
}
