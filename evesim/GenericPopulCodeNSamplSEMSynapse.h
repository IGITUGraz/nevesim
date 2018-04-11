#ifndef GenericPopulCodeNSamplSEMSynapse_H_
#define GenericPopulCodeNSamplSEMSynapse_H_

#include "EvSimObject.h"
#include "SpikeResponse.h"

template<class Base>
class GenericPopulCodeNSamplSEMSynapse : public Base {

public:
	GenericPopulCodeNSamplSEMSynapse(float Winit,
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

	virtual ~GenericPopulCodeNSamplSEMSynapse() {};

	float eta;

	float Winit;

	bool withHessian;

	bool activePlasticity;

	float C;

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
void GenericPopulCodeNSamplSEMSynapse<Base>::updateState(SimContext& sim_ctxt, Time time, update_id_t upd_id) {
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
		double Wpar = T * Base::W + Wnull;
		double m = 1;
		if (withHessian) {
			m = exp(-Wpar);
		}
		double newW = Base::W + eta * m * post_trace * (pre_trace - exp(Wpar)) * (time - timeOfLastChange);
		if (newW > maxW)
			Base::W = maxW;
		else
		if (newW < minW)
			Base::W = minW;
		else Base::W = newW;
		timeOfLastChange = time;
	}
	if (upd_id == 0)
		Base::updateState(sim_ctxt, time, upd_id);
}


#endif /* GenericPopulCodeNSamplSEMSynapse_H_ */
