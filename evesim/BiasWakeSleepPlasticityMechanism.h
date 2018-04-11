#ifndef BIASWAKESLEEPPLASTICITYMECHANISM_H_
#define BIASWAKESLEEPPLASTICITYMECHANISM_H_

#include "EvSimObject.h"
#include "EvSimObjectTypeUtils.h"

class BiasWakeSleepPlasticityMechanism : public EvSimObject {

	EV_SIM_OBJECT( BiasWakeSleepPlasticityMechanism )

public:
	BiasWakeSleepPlasticityMechanism(float eta = 1e-5,
									 short sign = 1,
							 	 	 bool isActive = true,
				 			 	 	 float eta_ratio = 1,
							 	 	 float biasMax = 6,
							 	 	 float biasMin = -6) {
		this->eta = eta;
		this->eta_ratio = eta_ratio;
		this->biasMax = biasMax;
		this->biasMin = biasMin;
		this->isActive = isActive;
		this->sign = sign;
	}

	float eta;

	float eta_ratio;

	float biasMax;

	float biasMin;

	bool isActive;

	short sign;

	virtual ~BiasWakeSleepPlasticityMechanism();

	virtual void postMountAsMountedObj(EvSimNetwork &net, EvSimObject::ID self, EvSimObject::ID mountpoint);

	virtual void reset(SimContext &sim_ctxt);

	virtual void eventHit(SimContext & sim_ctxt, Time time, port_id_t port);

protected:
	float *bias_ptr;
};

#endif /* BIASWAKESLEEPPLASTICITYMECHANISM_H_ */
