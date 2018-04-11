#include "ExpPoissonNeuron.h"

EV_SIM_OBJECT_POST( ExpPoissonNeuron )

FIELD_REGISTRATOR( ExpPoissonNeuron )
{
	REGISTER_FIELD( ExpPoissonNeuron, Vm)
	REGISTER_FIELD( ExpPoissonNeuron, bias )
	REGISTER_FIELD( ExpPoissonNeuron, C )
}


