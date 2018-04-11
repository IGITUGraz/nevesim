#ifndef NEURON_H_
#define NEURON_H_

#include "EvSimObject.h"

//! Base class to all neuron types
/*! Provides the interface for updating the membrane potential
    triggered by synapses mounted to the neuron.
 */
class Neuron : public EvSimObject {
public:
    //! Constructor
    Neuron();
    
    //! Destructor 
    virtual ~Neuron();

    //! Triggered when the PSP of an active input synapse changes
    /*! Reports a jump of the PSP of an input synapse
        \param psr The new value of the postsynaptic response
        \param psr_diff Recent change of the postsynaptic response
        \param time The simulation time at which the change occurred
        \see ActiveSynapse
     */
    virtual void updatePSR(double psr, double psr_diff, Time time) {};
    
    //! Triggered when the PSP slope of an active input synapse changes
    /*! Reports a change of the PSP slope (derivative of the PSP amplitude)
        of an input synapse
        \param psr_slope The new value of the slope
        \param psr_slope_diff Recent change of the slope
        \param time The simulation time at which the change occurred
        \see ActiveSynapse
     */
    virtual void updatePSRSlope(double psr_slope, double prs_slope_diff, Time time) {};

    //! Triggered when a input synapse is mounted to the neuron
    virtual unsigned registerInputSynapse(EvSimObject *input_synapse) { return 0; };

    //! Returns true if the neuron type requires active input synapses
    virtual bool requiresActiveSynapse() {
        return false;
    }
};

#endif /* NEURON_H_ */
