#include "NSHebbianSynapse.h"

EV_SIM_OBJECT_POST( NSHebbianSynapse )

EV_SIM_OBJECT_POST( NSHebbianActiveSynapse )

FIELD_REGISTRATOR( NSHebbianSynapse )
{
	REGISTER_FIELD( NSHebbianSynapse, eta)
	REGISTER_FIELD( NSHebbianSynapse, sign)
	REGISTER_FIELD( NSHebbianSynapse, W)
}

FIELD_REGISTRATOR( NSHebbianActiveSynapse )
{
	REGISTER_FIELD( NSHebbianActiveSynapse, eta)
	REGISTER_FIELD( NSHebbianActiveSynapse, sign)
	REGISTER_FIELD( NSHebbianActiveSynapse, W)
}
