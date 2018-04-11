#include "PLBasicSynapse.h"


EV_SIM_OBJECT_POST(PLBasicSynapse)

FIELD_REGISTRATOR(PLBasicSynapse)
{
	REGISTER_FIELD(PLBasicSynapse, psr)
	REGISTER_FIELD(PLBasicSynapse, W)
	REGISTER_FIELD(PLBasicSynapse, slope)
}

EV_SIM_OBJECT_POST(PLBasicActiveSynapse)

FIELD_REGISTRATOR(PLBasicActiveSynapse)
{
	REGISTER_FIELD(PLBasicActiveSynapse, psr)
	REGISTER_FIELD(PLBasicActiveSynapse, W)
	REGISTER_FIELD(PLBasicActiveSynapse, slope)
}

