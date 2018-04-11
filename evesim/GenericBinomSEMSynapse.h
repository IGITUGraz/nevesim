#ifndef GenericBinomSEMSynapse_H_
#define GenericBinomSEMSynapse_H_

#include "EvSimObject.h"
#include "SpikeResponse.h"

template<class Base>
class GenericBinomSEMSynapse : public Base {

public:
	GenericBinomSEMSynapse(float Winit,
						   float eta,
						   bool withHessian,
						   bool activePlasticity,
						   float T,
						   float Wnull,
						   float maxW,
						   float minW,
						   SpikeResponse *response,
						   SpikeResponse *bap_trace)
	{
		this->Winit = Winit;
		this->eta = eta;
		this->withHessian = withHessian;
		this->activePlasticity = activePlasticity;
		this->T = T;
		this->Wnull = Wnull;
		this->maxW = maxW;
		this->minW = minW;

		this->Base::syn_resp = (response) ? dynamic_cast<SpikeResponse *>(response->copy()) : 0;
		this->bap_response = (bap_trace) ? dynamic_cast<SpikeResponse *>(bap_trace->copy()) : 0;
	}

	virtual ~GenericBinomSEMSynapse() {};

	float Winit;

	float eta;

	bool withHessian;

	bool activePlasticity;

	float T;

	float Wnull;

	float maxW;

	float minW;

	virtual bool needsBackpropAPResponse() {return false;};

	virtual bool propagatesUpdate(update_id_t upd_id) const {
		if (upd_id == 1)
			return false;
		return true;
	}

	virtual void reset(SimContext &sim_ctxt)
	{
		Base::W = Winit;
		Base::resetPSRs();
	};

	void eventHit(SimContext & sim_ctxt, Time time, port_id_t port);

	virtual bool isPlastic() { return true; };

protected:
	SpikeResponse *bap_response;
};


template<class Base>
void GenericBinomSEMSynapse<Base>::eventHit(SimContext & sim_ctxt, Time time, port_id_t port) {
	double pre_trace = Base::syn_resp->get();
	if (port == 1) {
		if (activePlasticity) {
			double Wpar = T * Base::W + Wnull;
			double m = 1;
			if (withHessian) {
				m = ( 2 + exp(Wpar) + exp(-Wpar));
			}
			double newW = Base::W + eta * m * ( pre_trace - exp(Wpar)/(1 + exp(Wpar)) );
			if (newW > maxW)
				Base::W = maxW;
			else
			if (newW < minW)
				Base::W = minW;
			else Base::W = newW;
		}
	}
	Base::eventHit(sim_ctxt, time, port);
}


#endif /* GenericBinomSEMSynapse_H_ */
