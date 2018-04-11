/*
 * BinaryInputSynapse.cpp
 *
 *  Created on: Jul 24, 2012
 *      Author: dejan
 */

#include "BinaryInputSynapse.h"

EV_SIM_OBJECT_POST( BinaryInputSynapse )

FIELD_REGISTRATOR( BinaryInputSynapse )
{
	REGISTER_FIELD( BinaryInputSynapse, psr )

}


EV_SIM_OBJECT_POST( BinaryInputActiveSynapse )


FIELD_REGISTRATOR( BinaryInputActiveSynapse )
{
	REGISTER_FIELD( BinaryInputActiveSynapse, psr )
}

