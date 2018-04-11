#ifndef BiasAdaptiveHomeostMECHANISM_H_
#define BiasAdaptiveHomeostMECHANISM_H_

#include "EvSimObject.h"
#include "EvSimObjectTypeUtils.h"

class BiasAdaptiveHomeostMechanism : public EvSimObject {

	EV_SIM_OBJECT( BiasAdaptiveHomeostMechanism )

public:
	BiasAdaptiveHomeostMechanism(float eta = 1e-5,
								 float eta_rt = 1e-5,
								 float Vthresh = -5,
								 float mu = 0.9,
								 float biasMax = 15,
								 float biasMin = -15,
							     bool isActive = true)
	{
		this->eta = eta;
		this->eta_rt = eta_rt;
		this->Vthresh = Vthresh;
		this->mu = mu;
		this->biasMax = biasMax;
		this->biasMin = biasMin;
		this->isActive = isActive;
	}

	float eta;

	float eta_rt;

	float mu;

	float biasMax;

	float biasMin;

	float Vthresh;

	bool isActive;

	float rTarget;

	void setActive(bool active, Time time);

	void updateRtarget(Time time);

	virtual ~BiasAdaptiveHomeostMechanism();

	virtual void postMountAsMountedObj(EvSimNetwork &net, EvSimObject::ID self, EvSimObject::ID mountpoint);

	virtual void updateState(SimContext &sim_context, Time time, update_id_t upd_id);

	virtual void reset(SimContext &sim_ctxt);

	virtual void eventHit(SimContext & sim_ctxt, Time time, port_id_t port);


protected:
	float *bias_ptr;
	double *vm_ptr;
	double prev_vm;
	double last_bias_upd_time;
	double last_rtgt_upd_time;
};

#endif /* BiasAdaptiveHomeostMECHANISM_H_ */
