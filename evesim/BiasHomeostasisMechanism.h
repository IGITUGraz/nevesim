#ifndef BIASHOMEOSTASISMECHANISM_H_
#define BIASHOMEOSTASISMECHANISM_H_

#include "EvSimObject.h"
#include "EvSimObjectTypeUtils.h"

class BiasHomeostasisMechanism : public EvSimObject {

	EV_SIM_OBJECT( BiasHomeostasisMechanism )

public:
	BiasHomeostasisMechanism(float eta = 1e-5,
							 float rTarget = 20,
							 bool isActive = true)
	{
		this->eta = eta;
		this->rTarget = rTarget;
		this->isActive = isActive;
	}

	float eta;

	float rTarget;

	bool isActive;

	virtual ~BiasHomeostasisMechanism();

	virtual void postMountAsMountedObj(EvSimNetwork &net, EvSimObject::ID self, EvSimObject::ID mountpoint);

	virtual void reset(SimContext &sim_ctxt);

	virtual void eventHit(SimContext & sim_ctxt, Time time, port_id_t port);


protected:
	float *bias_ptr;
	double last_update_time;
};

#endif /* BIASHOMEOSTASISMECHANISM_H_ */
