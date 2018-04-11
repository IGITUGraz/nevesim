/*
 * BinaryInputSynapse.h
 *
 *  Created on: Jul 24, 2012
 *      Author: dejan
 */

#ifndef BINARYINPUTSYNAPSE_H_
#define BINARYINPUTSYNAPSE_H_

#include "EvSimObjectTypeUtils.h"
#include "Synapse.h"
#include "GenericBinaryInputSynapse.h"


class BinaryInputSynapse : public GenericBinaryInputSynapse<Synapse> {

	EV_SIM_OBJECT( BinaryInputSynapse )

public:
	BinaryInputSynapse(float psr_level_hi = 2,
			   	   	   float psr_level_lo = 0,
			   	   	   bool isActive = true) :
		GenericBinaryInputSynapse<Synapse>(psr_level_hi, psr_level_lo, isActive)
	{}

};

class BinaryInputActiveSynapse : public GenericBinaryInputSynapse<ActiveSynapse> {

	EV_SIM_OBJECT( BinaryInputActiveSynapse )

public:
	BinaryInputActiveSynapse(float psr_level_hi = 5,
			   	   	   	     float psr_level_lo = 0,
			   	   	   	     bool isActive = true) :
		GenericBinaryInputSynapse<ActiveSynapse>(psr_level_hi, psr_level_lo, isActive)
	{}

};

#endif /* BINARYINPUTSYNAPSE_H_ */
