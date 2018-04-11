#ifndef COMPOSITESYNAPSE_H_
#define COMPOSITESYNAPSE_H_

#include "Synapse.h"

class CompositeSynapse : public SynapseFactory {

public:
	CompositeSynapse(const Synapse &synapse,
			         const SpikeResponse &post_syn_response, const SpikeResponse &backprop_ap_response,
					 bool post_syn_resp_is_shared = true, bool bap_response_is_shared = true,
					 Time delay = 0, Time bap_delay = 0)
		:
		  delay(delay),
		  bap_delay(bap_delay),
		  synapse(dynamic_cast<Synapse *>(synapse.copy())),
		  post_syn_response(dynamic_cast<SpikeResponse *>(post_syn_response.copy())),
		  backprop_ap_response(dynamic_cast<SpikeResponse *>(backprop_ap_response.copy())),
		  psr_is_shared(post_syn_resp_is_shared),
		  bap_response_is_shared(bap_response_is_shared)

	 {}

	CompositeSynapse(const Synapse &synapse, const SpikeResponse &post_syn_response,
			         bool post_syn_resp_is_shared = true, Time delay = 0)
		:
		  delay(delay),
		  bap_delay(-1),
		  synapse(dynamic_cast<Synapse *>(synapse.copy())),
		  post_syn_response(dynamic_cast<SpikeResponse *>(post_syn_response.copy())),
		  backprop_ap_response(0),
		  psr_is_shared(post_syn_resp_is_shared)
	{

	}


	virtual ~CompositeSynapse() {
		delete synapse;
		delete post_syn_response;
		delete backprop_ap_response;
	}

	virtual EvSimObject* create( void ) const {
		return synapse->copy(); };

	virtual EvSimObject* create( RandomEngine *rng ) const {
		return synapse->copy();
	};

	virtual SpikeResponse *createSpikeResponseObject() const {
		if (post_syn_response)
			return dynamic_cast<SpikeResponse *>(post_syn_response->copy());
		else
			return 0;
	};
	virtual SpikeResponse *createBackpropAPResponseObject() const {
		if (backprop_ap_response)
			return dynamic_cast<SpikeResponse *>(backprop_ap_response->copy());
		else
			return 0;
	};

	virtual void setSharedPSResponse(bool shared_psr) {
		psr_is_shared = shared_psr;
	}

	virtual void setSharedBackpropAPResponse(bool shared_bap_response) {
		bap_response_is_shared = shared_bap_response;
	}


	virtual bool usesSharedPSResponse() const {return psr_is_shared;};

	virtual bool usesSharedBackpropAPResponse() const {return bap_response_is_shared;};


	bool managesDelay() const {
		return true;
	}

	Time getManagedDelay() const {
		return delay;
	}

	Time getBackpropAPDelay() const {
		return bap_delay;
	}

protected:
	Time delay;
	Time bap_delay;
	Synapse *synapse;
	SpikeResponse *post_syn_response;
	SpikeResponse *backprop_ap_response;
	bool psr_is_shared;
	bool bap_response_is_shared;
};




#endif /* COMPOSITESYNAPSE_H_ */
