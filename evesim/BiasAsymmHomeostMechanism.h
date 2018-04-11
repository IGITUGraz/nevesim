#ifndef BiasAsymmHomeostMECHANISM_H_
#define BiasAsymmHomeostMECHANISM_H_

#include "EvSimObject.h"
#include "EvSimObjectTypeUtils.h"

class BiasAsymmHomeostMechanism : public EvSimObject {

	EV_SIM_OBJECT( BiasAsymmHomeostMechanism )

public:
	BiasAsymmHomeostMechanism(float eta_plus = 1e-5,
							  float eta_minus = 1e-5,
							  float rTarget = 20,
							  float tau = 5.0,
							  bool isActive = true)
	{
		this->eta_plus = eta_plus;
		this->eta_minus = eta_minus;
		this->tau = tau;
		this->rTarget = rTarget;
		this->isActive = isActive;
	}

	float eta_plus;

	float eta_minus;

	float tau;

	float rTarget;

	bool isActive;

	virtual ~BiasAsymmHomeostMechanism();

	virtual void postMountAsMountedObj(EvSimNetwork &net, EvSimObject::ID self, EvSimObject::ID mountpoint);

	virtual void reset(SimContext &sim_ctxt);

	virtual void eventHit(SimContext & sim_ctxt, Time time, port_id_t port);

protected:

	inline double C( double const& delta ) {
		return exp( -delta/tau );
	};

	float r_estim;

	float *bias_ptr;

	double last_update_time;
};

#endif /* BiasAsymmHomeostMECHANISM_H_ */
