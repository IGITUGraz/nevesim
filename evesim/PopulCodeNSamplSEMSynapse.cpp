#include "PopulCodeNSamplSEMSynapse.h"

EV_SIM_OBJECT_POST( PopulCodeNSamplSEMSynapse )

FIELD_REGISTRATOR( PopulCodeNSamplSEMSynapse )
{
	REGISTER_FIELD( PopulCodeNSamplSEMSynapse, timeOfLastChange )
	REGISTER_FIELD( PopulCodeNSamplSEMSynapse, W)
	REGISTER_FIELD( PopulCodeNSamplSEMSynapse, eta)
	REGISTER_FIELD( PopulCodeNSamplSEMSynapse, T)
}

EV_SIM_OBJECT_POST( PopulCodeNSamplSEMActiveSynapse )

FIELD_REGISTRATOR( PopulCodeNSamplSEMActiveSynapse )
{
	REGISTER_FIELD( PopulCodeNSamplSEMActiveSynapse, timeOfLastChange )
	REGISTER_FIELD( PopulCodeNSamplSEMActiveSynapse, W)
	REGISTER_FIELD( PopulCodeNSamplSEMActiveSynapse, eta)
	REGISTER_FIELD( PopulCodeNSamplSEMActiveSynapse, T)
}
