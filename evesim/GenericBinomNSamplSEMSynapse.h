#ifndef GenericBinomNSamplSEMSynapse_H_
#define GenericBinomNSamplSEMSynapse_H_

#include "EvSimObject.h"
#include "SpikeResponse.h"

template<class Base>
class GenericBinomNSamplSEMSynapse : public Base {

public:
	GenericBinomNSamplSEMSynapse(float Winit,
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

	virtual ~GenericBinomNSamplSEMSynapse() {};

	float eta;

	float Winit;

	bool withHessian;

	bool activePlasticity;

	float T;

	float Wnull;

	float maxW;

	float minW;

	virtual void setBackpropAPResponse(SpikeResponse *response) {
		bap_response = response;
	}

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

	virtual SpikeResponse *createBackpropAPResponseObject() {return bap_response;};

	virtual void updateState(SimContext &sim_ctxt, Time time, update_id_t upd_id);

	virtual bool isPlastic() { return true; };

protected:
	SpikeResponse *bap_response;
	Time timeOfLastChange;
};


template<class Base>
void GenericBinomNSamplSEMSynapse<Base>::updateState(SimContext& sim_ctxt, Time time, update_id_t upd_id) {
	double pre_trace, post_trace;
	if (upd_id == 0) {
		pre_trace = Base::syn_resp->getOld();
		post_trace = bap_response->get();
	}
	else {
		pre_trace = Base::syn_resp->get();
		post_trace = bap_response->getOld();
	}
	if (activePlasticity) {
		double m = 1;
		double Wpar = T*Base::W + Wnull;
		if (withHessian) {
			m = ( 2 + exp(Wpar) + exp(-Wpar));
		}
		double newW = Base::W + eta * m * post_trace * ( pre_trace - exp(Wpar)/(1 + exp(Wpar)) ) * (time - timeOfLastChange);
		if (newW < maxW && newW > minW) Base::W = newW;
		timeOfLastChange = time;
	}
	if (upd_id == 0)
		Base::updateState(sim_ctxt, time, upd_id);
}


#endif /* GenericBinomNSamplSEMSynapse_H_ */
