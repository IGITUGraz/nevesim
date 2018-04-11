#ifndef BiasSEMPlasticityMECHANISM_H_
#define BiasSEMPlasticityMECHANISM_H_

#include "EvSimObject.h"
#include "EvSimObjectTypeUtils.h"
#include "EvSimEngine.h"

class BiasSEMPlasticityMechanism : public EvSimObject {

	EV_SIM_OBJECT( BiasSEMPlasticityMechanism )

public:
	BiasSEMPlasticityMechanism(float eta = 1e-5,
							   float tau = 20e-3,
							   float T = 1.0,
							   float L = 0,
							   float biasMax = 30.0,
							   float biasMin = -30.0,
							   float maxDelta = 60.0,
							   bool withHessian = true,
							   bool isActive = true) {
		this->eta = eta;
		this->tau = tau;
		this->L = L;
		this->T = T;
		this->biasMax = biasMax;
		this->biasMin = biasMin;
		this->maxDelta = maxDelta;
		this->isActive = isActive;
		this->withHessian = withHessian;

	}

	float eta;

	float tau;

	float L;

	float T;

	float biasMax;

	float biasMin;

	float maxDelta;

	bool withHessian;

	bool isActive;

	virtual ~BiasSEMPlasticityMechanism();

	virtual void postMountAsMountedObj(EvSimNetwork &net, EvSimObject::ID self, EvSimObject::ID mountpoint);

	virtual void reset(SimContext &sim_ctxt);

	virtual void eventHit(SimContext & sim_ctxt, Time time, port_id_t port);

	virtual void activate();

protected:
	float *bias_ptr;
	double last_update_time;
};

#endif /* BiasSEMPlasticityMECHANISM_H_ */
