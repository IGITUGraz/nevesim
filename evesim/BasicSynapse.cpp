#include "BasicSynapse.h"


EV_SIM_OBJECT_POST(BasicSynapse)

FIELD_REGISTRATOR(BasicSynapse)
{
	REGISTER_FIELD(BasicSynapse, psr)
	REGISTER_FIELD(BasicSynapse, W)
}

EV_SIM_OBJECT_POST(BasicActiveSynapse)

FIELD_REGISTRATOR(BasicActiveSynapse)
{
	REGISTER_FIELD(BasicActiveSynapse, psr)
	REGISTER_FIELD(BasicActiveSynapse, W)
}




