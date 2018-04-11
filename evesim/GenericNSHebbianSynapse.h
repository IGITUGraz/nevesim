#ifndef GENERICNSHEBBIANSYNAPSE_H_
#define GENERICNSHEBBIANSYNAPSE_H_

#include "SpikeResponse.h"
#include "EvSimObjectTypeUtils.h"

template<class Base>
class GenericNSHebbianSynapse : public Base {

public:
	GenericNSHebbianSynapse(float Winit,
							float eta,
							bool activePlasticity,
							float maxW,
							float minW,
							short sign,
							SpikeResponse *response,
							SpikeResponse *bap_trace)
	{

		this->Winit = Winit;
		this->eta = eta;
		this->sign=sign;
		this->activePlasticity = activePlasticity;
		this->Base::syn_resp = (response) ? dynamic_cast<SpikeResponse *>(response->copy()) : 0;
		this->bap_response = (bap_trace) ? dynamic_cast<SpikeResponse *>(bap_trace->copy()) : 0;
		this->maxW = maxW;
		this->minW = minW;

	}

	virtual ~GenericNSHebbianSynapse() {};

	float eta;

	float Winit;

	short sign;

	bool activePlasticity;

	float maxW;

	float minW;

	virtual void setBackpropAPResponse(SpikeResponse *response) {
		bap_response = response;
	};

	virtual bool propagatesUpdate(update_id_t upd_id) const {
		if (upd_id == 1)
			return false;
		return true;
	};

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
void GenericNSHebbianSynapse<Base>::updateState(SimContext& sim_ctxt, Time time, update_id_t upd_id) {
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
		double newW = Base::W + eta * sign *post_trace * pre_trace * (time - timeOfLastChange);
		if (newW < maxW && newW > minW) Base::W = newW;
		timeOfLastChange = time;
	}
	if (upd_id == 0)
		Base::updateState(sim_ctxt, time, upd_id);
}

#endif /* GENERICNSHEBBIANSYNAPSE_H_ */
