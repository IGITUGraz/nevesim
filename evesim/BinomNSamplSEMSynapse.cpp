#include "BinomNSamplSEMSynapse.h"


EV_SIM_OBJECT_POST( BinomNSamplSEMSynapse )

FIELD_REGISTRATOR( BinomNSamplSEMSynapse )
{
	REGISTER_FIELD( BinomNSamplSEMSynapse, timeOfLastChange )
	REGISTER_FIELD( BinomNSamplSEMSynapse, W)
	REGISTER_FIELD( BinomNSamplSEMSynapse, eta)
	REGISTER_FIELD( BinomNSamplSEMSynapse, T)
}

EV_SIM_OBJECT_POST( BinomNSamplSEMActiveSynapse )

FIELD_REGISTRATOR( BinomNSamplSEMActiveSynapse )
{
	REGISTER_FIELD( BinomNSamplSEMActiveSynapse, timeOfLastChange )
	REGISTER_FIELD( BinomNSamplSEMActiveSynapse, W)
	REGISTER_FIELD( BinomNSamplSEMActiveSynapse, eta)
	REGISTER_FIELD( BinomNSamplSEMActiveSynapse, T)
}
