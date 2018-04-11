#include "NeurSamplingNetwork.h"
#include "EvSimObject.h"
#include <boost/format.hpp>
#include "SimException.h"

NeurSamplingNetwork::~NeurSamplingNetwork()
{}

EvSimObject::ID NeurSamplingNetwork::associateSpikeResponse(EvSimObject::ID id, const EvSimObjectFactory &resp_factory, Time delay)
{
	EvSimObject::ID resp_id = create(resp_factory);
	assoc_spike_resp[id] = resp_id;
	EvSimNetwork::connect(id, 0, resp_id, 0, delay);
	return resp_id;
}

EvSimObject::ID NeurSamplingNetwork::getAssociatedSpikeResponse(EvSimObject::ID id)
{
	AssociatedSpikeResponsesMapType::const_iterator it;
	if ((it = assoc_spike_resp.find(id)) != assoc_spike_resp.end())
		return it->second;
	return INVALID_EVSIMOBJECT_ID;
}



SpikeResponse * NeurSamplingNetwork::createNewPostSynResponse(const SynapseFactory &syn_factory, EvSimObject::ID & psr_id)
{
	EvSimObject *psr_obj = syn_factory.createSpikeResponseObject();
	if (psr_obj)
		psr_id = add(psr_obj);
	else
		throw evesim::ConstructionException("void connect(EvSimObject::ID src_id, EvSimObject::ID dest_id, const SynapseFactory &syn_factory,"
												 " bool sharedPSResponse = false, bool sharedBAPtrace = false);",
											str( boost::format("Cannot create a new post-synaptic response: the synapse factory can not create a valid spike response.") ));
	return dynamic_cast<SpikeResponse *>(psr_obj);
}

SpikeResponse * NeurSamplingNetwork::createNewBackpropAPResponse(const SynapseFactory &syn_factory, EvSimObject::ID & bap_trace_id)
{
	EvSimObject *bap_trace_obj = syn_factory.createBackpropAPResponseObject();
	if (bap_trace_obj)
		bap_trace_id = add(bap_trace_obj);
	else
		throw evesim::ConstructionException("void connect(EvSimObject::ID src_id, EvSimObject::ID dest_id, const SynapseFactory &syn_factory,"
												 " bool sharedPSResponse = false, bool sharedBAPtrace = false);",
											str( boost::format("Cannot create a new backpropagating action potential response: the synapse factory can not create a valid spike response.") ));
	return dynamic_cast<SpikeResponse *>(bap_trace_obj);
}


EvSimObject::ID NeurSamplingNetwork::connect(EvSimObject::ID src_id, EvSimObject::ID dest_id,
				        const SynapseFactory& syn_factory, Time delay)
{
	EvSimObject::ID psr_id, bap_trace_id;
	return connect(src_id, dest_id, syn_factory, psr_id, bap_trace_id, delay);
}


EvSimObject::ID NeurSamplingNetwork::connect(EvSimObject::ID src_id, EvSimObject::ID dest_id,
		               const SynapseFactory& syn_factory, EvSimObject::ID &psr_id, EvSimObject::ID &bap_trace_id, Time delay)
{

	EvSimObject::ID syn_id = create(syn_factory);
	Synapse *syn_obj = dynamic_cast<Synapse *>(getObject(syn_id));

	Time syn_delay = delay;

	if (syn_factory.managesDelay())
		 syn_delay = syn_factory.getManagedDelay();


	SpikeResponse *psr_obj;

	if (!syn_obj->needsPostSynResponse()) {
		EvSimNetwork::connect(src_id, 0, syn_id, 0, syn_delay);
	}
	else {
		if (syn_factory.usesSharedPSResponse()) {
			psr_id = getAssociatedSpikeResponse(src_id);
			if (psr_id != INVALID_EVSIMOBJECT_ID)
				psr_obj = dynamic_cast<SpikeResponse *>(getObject(psr_id));
			else {
				psr_obj = createNewPostSynResponse(syn_factory, psr_id);
				assoc_spike_resp[src_id] = psr_id;
				EvSimNetwork::connect(src_id, 0, psr_id, 0, syn_delay);
			}
		}
		else {
			psr_obj = createNewPostSynResponse(syn_factory, psr_id);
			EvSimNetwork::connect(src_id, 0, psr_id, 0, syn_delay);
		}
		syn_obj->setPostSynResponse(psr_obj);
		causalUpdateLink(psr_id, syn_id, 0);
	}

	SpikeResponse *bap_trace_obj;

	if (syn_obj->isPlastic()) {
		if (!syn_obj->needsBackpropAPResponse()) {
			EvSimNetwork::connect(dest_id, (port_id_t)0, syn_id, (port_id_t)1);
		}
		else {
			if (syn_factory.usesSharedBackpropAPResponse()) {
				bap_trace_id = getAssociatedSpikeResponse(dest_id);
				if (bap_trace_id != INVALID_EVSIMOBJECT_ID)
					bap_trace_obj = dynamic_cast<SpikeResponse *>(getObject(bap_trace_id));
				else {
					bap_trace_obj = createNewBackpropAPResponse(syn_factory, bap_trace_id);
					assoc_spike_resp[dest_id] = bap_trace_id;
					connect(dest_id, 0, bap_trace_id, 0, syn_factory.getBackpropAPDelay());
				}
			}
			else {
				bap_trace_obj = createNewBackpropAPResponse(syn_factory, bap_trace_id);
				connect(dest_id, 0, bap_trace_id, 0, syn_factory.getBackpropAPDelay());
			}
			syn_obj->setBackpropAPResponse(bap_trace_obj);
			causalUpdateLink(bap_trace_id, syn_id, 1);
		}
	}

	mount(syn_id, dest_id);
	return syn_id;
}

EvSimObject::IDVector NeurSamplingNetwork::connect(EvSimObject::IDVector src_vec, EvSimObject::IDVector dest_vec,
				        const SynapseFactory& syn_factory, Time delay)
{
	EvSimObject::IDVector syn_id_vec = EvSimObject::IDVector(new vector<EvSimObject::ID>());
	EvSimObject::ID syn_id;

	EvSimObject::IDVector::element_type::const_iterator src_it = src_vec->begin();
	EvSimObject::IDVector::element_type::const_iterator dest_it = dest_vec->begin();
	
	for (;src_it != src_vec->end(); ++src_it) {	
    	syn_id = connect(*src_it, *dest_it, syn_factory, delay);
		syn_id_vec->push_back(syn_id);
		++dest_it;
	}
	
	return syn_id_vec;
}

EvSimObject::IDVector NeurSamplingNetwork::connectAll2All(EvSimObject::IDVector src_vec, EvSimObject::IDVector dest_vec,
			const SynapseFactory& syn_factory, EvSimObject::IDVector &psr_id_vec, EvSimObject::IDVector &bap_trace_id_vec, Time delay)
{
	EvSimObject::IDVector syn_id_vec = EvSimObject::IDVector(new vector<EvSimObject::ID>());
	EvSimObject::ID psr_id;
	EvSimObject::ID bap_trace_id;
	EvSimObject::ID syn_id;

	EvSimObject::IDVector::element_type::const_iterator src_it = src_vec->begin();
	for (;src_it != src_vec->end(); ++src_it) {
		EvSimObject::IDVector::element_type::const_iterator dest_it = dest_vec->begin();
		for ( ;dest_it != dest_vec->end(); ++dest_it) {
			syn_id = connect(*src_it, *dest_it, syn_factory, psr_id, bap_trace_id, delay);
			psr_id_vec->push_back(psr_id);
			bap_trace_id_vec->push_back(bap_trace_id);
			syn_id_vec->push_back(syn_id);
		}
	}
	return syn_id_vec;
}

EvSimObject::IDVector NeurSamplingNetwork::connectAll2All(EvSimObject::IDVector src_vec, EvSimObject::IDVector dest_vec,
																			const SynapseFactory& syn_factory)
{
	EvSimObject::IDVector syn_id_vec = EvSimObject::IDVector(new vector<EvSimObject::ID>());
	EvSimObject::ID syn_id;

	EvSimObject::IDVector::element_type::const_iterator src_it = src_vec->begin();
	for (;src_it != src_vec->end(); ++src_it) {
		EvSimObject::IDVector::element_type::const_iterator dest_it = dest_vec->begin();
		for ( ;dest_it != dest_vec->end(); ++dest_it) {
			syn_id = connect(*src_it, *dest_it, syn_factory);
			syn_id_vec->push_back(syn_id);
		}
	}
	return syn_id_vec;
}



