#ifndef NEURSAMPLINGNETWORK_H_
#define NEURSAMPLINGNETWORK_H_

#include "EvSimObject.h"
#include "Synapse.h"
#include "CompositeSynapse.h"
#include "EvSimNetwork.h"
#include "evesim_hash.h"

class NeurSamplingNetwork: public EvSimNetwork {
public:

	NeurSamplingNetwork(SimParameters sim_params = SimParameters()) : EvSimNetwork(sim_params)
	{};

	virtual ~NeurSamplingNetwork();

	EvSimObject::ID associateSpikeResponse(EvSimObject::ID id, const EvSimObjectFactory &resp_factory, Time delay = 0);

	EvSimObject::ID getAssociatedSpikeResponse(EvSimObject::ID id);

	using EvSimNetwork::connect;

	EvSimObject::IDVector connectAll2All(EvSimObject::IDVector src_vec, EvSimObject::IDVector dest_vec,
			const SynapseFactory& syn_factory, EvSimObject::IDVector &psr_id_vec, EvSimObject::IDVector &bap_trace_id_vec, Time delay = 0);

	EvSimObject::IDVector connectAll2All(EvSimObject::IDVector src_vec, EvSimObject::IDVector dest_vec,
																			const SynapseFactory& syn_factory);

	EvSimObject::ID connect(EvSimObject::ID src_id, EvSimObject::ID dest_id,
							const SynapseFactory& syn_factory, EvSimObject::ID &psr_id, EvSimObject::ID &bap_trace_id, Time delay = 0);

	EvSimObject::ID connect(EvSimObject::ID src_id, EvSimObject::ID dest_id,
				     const SynapseFactory& syn_factory, Time delay = 0);

	EvSimObject::IDVector connect(EvSimObject::IDVector src_vec, EvSimObject::IDVector dest_vec,
				     const SynapseFactory& syn_factory, Time delay = 0);


protected:
	typedef unordered_map< EvSimObject::ID , EvSimObject::ID, evesim::hash<EvSimObject::ID> > AssociatedSpikeResponsesMapType;

	 AssociatedSpikeResponsesMapType assoc_spike_resp;

	 SpikeResponse * createNewPostSynResponse(const SynapseFactory &syn_factory, EvSimObject::ID & psr_id);
	 SpikeResponse * createNewBackpropAPResponse(const SynapseFactory &syn_factory, EvSimObject::ID & bap_trace_id);

};

#endif /* NEURSAMPLINGNETWORK_H_ */
