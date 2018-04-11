#ifndef EXPPOISSONNEURON_H_
#define EXPPOISSONNEURON_H_

#include "GenericPoissonNeuron.h"


class ExpPoissonNeuron : public GenericPoissonNeuronComplete<
                                            GenericExpPoissonNeuron<
                                              GenericPoissNeuronFast< PoissonNeuronBase > > >
{
	EV_SIM_OBJECT( ExpPoissonNeuron )
public:

	ExpPoissonNeuron(float A = 50,
					 float bias = 0,
					 float Trefract = 10e-3,
					 float C = 1) {
		this->A = A;
		this->C = C;
		this->bias = bias;
		this->Trefract = Trefract;
	}
};

#endif /* EXPPOISSONNEURON_H_ */
