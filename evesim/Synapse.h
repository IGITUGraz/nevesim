#ifndef Synapse_H_
#define Synapse_H_

#include "EvSimObject.h"
#include "SpikeResponse.h"
#include "Neuron.h"
#include "EvSimNetwork.h"
#include "SimException.h"

#ifndef SWIG

#include <iostream>
using std::cerr; using std::endl;

#endif

//! Basic synapse factory class
class SynapseFactory : public virtual EvSimObjectFactory {
public:

    virtual ~SynapseFactory() {};

    virtual SpikeResponse *createSpikeResponseObject() const {return 0;};
    virtual SpikeResponse *createBackpropAPResponseObject() const {return 0;};

    virtual bool usesSharedPSResponse() const {return false;};

    virtual bool usesSharedBackpropAPResponse() const {return false;};

    virtual bool managesDelay() const {
        return false;
    }

    virtual Time getManagedDelay() const {
        return 0;
    }

    virtual Time getBackpropAPDelay() const {
        return 0;
    }

};

//! Base class to all synapses
class Synapse : public EvSimObject, public SynapseFactory {
public:

    double getPSR() {return psr;};

    virtual void setBackpropAPResponse(SpikeResponse *bap_response) {};

    virtual void setPostSynResponse(SpikeResponse *response) {};

    virtual bool needsPostSynResponse() {return true;};

    virtual bool needsBackpropAPResponse() {return true;};

    virtual bool isPlastic() { return false; };

    virtual bool isActive() {return false;}


    virtual void postMountAsMountedObj(EvSimNetwork &net, EvSimObject::ID self, EvSimObject::ID mountpoint)
    {
        mountToNeuron(net,self, mountpoint);
    }

    virtual ~Synapse();

protected:

    void updatePSR(double new_psr, Time) {
        psr = new_psr;
    }

    void resetPSRs() {
        psr = 0;
    }


    Neuron * mountToNeuron(EvSimNetwork &net, EvSimObject::ID &self, EvSimObject::ID &nrn) {
        Neuron *nrn_obj = dynamic_cast<Neuron *>(net.getObject(nrn));
        if (!nrn_obj) {
            throw evesim::ConstructionException("ActiveSynapse::mountToNeuron",
                               "Can not mount a Synapse object. The mountpoint must be of 'Neuron' type.");
        }
        if (nrn_obj->requiresActiveSynapse() && !isActive()) {
            throw evesim::ConstructionException("ActiveSynapse::mountToNeuron",
                                           "Can not mount the Synapse object which is not active. The mountpoint Neuron requires an active synapse.");
        }

        update_id_t update_id = nrn_obj->registerInputSynapse(dynamic_cast<EvSimObject *>(this));
        net.causalUpdateLink(self, nrn, update_id);
        return nrn_obj;
    }

    double psr;
};


//! Base class to active synapses
/*! Active synapses report PSP changes to their target neuron
    using the \ref Neuron::updatePSR and Neuron::updatePSRSlope
    interface
 */
class ActiveSynapse : public Synapse {
public:

    ~ActiveSynapse();

    virtual void postMountAsMountedObj(EvSimNetwork &net, EvSimObject::ID self, EvSimObject::ID mountpoint) {
        target_nrn = mountToNeuron(net, self, mountpoint);
    }

    virtual bool isActive() {
        return true;
    }


protected:
    Neuron *target_nrn;

    void updatePSR(double new_psr, Time time) {
        psr_old = psr;
        psr = new_psr;
        target_nrn->updatePSR(psr, psr - psr_old, time);
    }

    void resetPSRs() {
        psr = psr_old = 0;
    }

    double psr_old;
};



#endif /* Synapse_H_ */
