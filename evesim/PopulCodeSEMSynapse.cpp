#include "PopulCodeSEMSynapse.h"


EV_SIM_OBJECT_POST( PopulCodeSEMSynapse )

FIELD_REGISTRATOR( PopulCodeSEMSynapse )
{
	REGISTER_FIELD( PopulCodeSEMSynapse, W)
	REGISTER_FIELD( PopulCodeSEMSynapse, eta)
	REGISTER_FIELD( PopulCodeSEMSynapse, T)
}

EV_SIM_OBJECT_POST( PopulCodeSEMActiveSynapse )

FIELD_REGISTRATOR( PopulCodeSEMActiveSynapse )
{
	REGISTER_FIELD( PopulCodeSEMActiveSynapse, W)
	REGISTER_FIELD( PopulCodeSEMActiveSynapse, eta)
	REGISTER_FIELD( PopulCodeSEMActiveSynapse, T)
}
