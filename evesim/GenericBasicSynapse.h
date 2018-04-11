#ifndef GenericBasicSynapse_H_
#define GenericBasicSynapse_H_

#include "EvSimObject.h"
#include "ResetRectSpikeResponse.h"
#include "EvSimObjectTypeUtils.h"
#include "Synapse.h"
#include "SimException.h"
#include "Neuron.h"

#ifndef SWIG

#include <boost/format.hpp>
using boost::str;

#endif

template<class BaseSyn>
class GenericBasicSynapse : public BaseSyn {
public:

	GenericBasicSynapse(float W = 1)
	{
		this->W = W;
		this->syn_resp = 0;
	};

	virtual ~GenericBasicSynapse() {};

	double W;

	virtual void reset(SimContext &sim_ctxt)
	{
		BaseSyn::resetPSRs();
	};


	virtual void setPostSynResponse(SpikeResponse *response) {
		syn_resp = response;
	}

	virtual void updateState(SimContext &sim_ctxt, Time time, update_id_t upd_id)
	{
		BaseSyn::updatePSR(W*syn_resp->get(), time);
	}

protected:
	SpikeResponse *syn_resp;
};

#endif /* GenericBasicSynapse_H_ */
