#include "PLExpPoissonNeuron.h"

EV_SIM_OBJECT_POST( PLExpPoissonNeuron )

FIELD_REGISTRATOR( PLExpPoissonNeuron )
{
	REGISTER_FIELD( PLExpPoissonNeuron, Vm)
	REGISTER_FIELD( PLExpPoissonNeuron, bias )
	REGISTER_FIELD( PLExpPoissonNeuron, slope )
	REGISTER_FIELD( PLExpPoissonNeuron, C )
}


