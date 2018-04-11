#include "BinomSEMSynapse.h"


EV_SIM_OBJECT_POST( BinomSEMSynapse )

FIELD_REGISTRATOR( BinomSEMSynapse )
{
	REGISTER_FIELD( BinomSEMSynapse, W)
	REGISTER_FIELD( BinomSEMSynapse, eta)
	REGISTER_FIELD( BinomSEMSynapse, T)
}

EV_SIM_OBJECT_POST( BinomSEMActiveSynapse )

FIELD_REGISTRATOR( BinomSEMActiveSynapse )
{
	REGISTER_FIELD( BinomSEMActiveSynapse, W)
	REGISTER_FIELD( BinomSEMActiveSynapse, eta)
	REGISTER_FIELD( BinomSEMActiveSynapse, T)
}

