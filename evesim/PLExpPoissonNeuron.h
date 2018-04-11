#ifndef PLEXPPOISSONNEURON_H_
#define PLEXPPOISSONNEURON_H_

#include "GenericPLPoissonNeuron.h"


//! Exponential Poisson neuron that supports piecewise linear input synapses.
/*! The neuron's spike rate is determined by \f$exp(u(t))\f$, where \f$u(t)\f$
    is a the membrane potential. If piecewise linear synapses are mounted
    to this neuron, \f$u(t)\f$ will be a piecewise linear function, but also
    jumps on the membrane potential are allowed (e.g. by mounting a piecewise
    constant synapse to the neuron).
    \see PLBasicSynapse
 */
class PLExpPoissonNeuron : public GenericPoissonNeuronComplete<
                                            GenericPLExpPoissonNeuron<
                                              GenericPLPoissNeuronFast< PoissonNeuronBase > > >
{
    EV_SIM_OBJECT( PLExpPoissonNeuron )
public:

    PLExpPoissonNeuron(float A = 50,
                       float bias = 0,
                       float Trefract = 10e-3,
                       float C = 1) {
        this->A = A;
        this->C = C;
        this->bias = bias;
        this->Trefract = Trefract;
    }

};

#endif /* PLEXPPOISSONNEURON_H_ */
