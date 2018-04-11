#ifndef PLBASICSYNAPSE_H_
#define PLBASICSYNAPSE_H_

#include "EvSimObjectTypeUtils.h"
#include "GenericPLBasicSynapse.h"


//! Synapse that supports piecewise linear spike responses.
/*! The PSP amplitude trace of this synapse is a piecewise linear
    function if a piecewise linear spike response is mounted
    to it.
    \note Neurons this synapse is mounted to should implement the
          \rev Neuron::updatePSRSlope method
    \see PLResetKernelSpikeResponse
    \see PLAdditiveKernelSpikeResponse
    \see PLExpPoissonNeuron
 */
class PLBasicSynapse : public GenericPLBasicSynapse< PLSynapse > {

	EV_SIM_OBJECT( PLBasicSynapse )

public:
	PLBasicSynapse(float W = 1) :
		GenericPLBasicSynapse< PLSynapse >(W) {};

	~PLBasicSynapse() {};
};

//! Active synapse version of PLBasicSynapse
/*! \see PLBasicSynapse
 */
class PLBasicActiveSynapse : public GenericPLBasicSynapse< PLActiveSynapse > {

	EV_SIM_OBJECT( PLBasicActiveSynapse )

public:
	PLBasicActiveSynapse(float W = 1) :
		GenericPLBasicSynapse< PLActiveSynapse >(W) {};

	~PLBasicActiveSynapse() {};
};

#endif /* PLBASICSYNAPSE_H_ */
