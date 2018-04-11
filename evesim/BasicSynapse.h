#ifndef BASICSYNAPSE_H_
#define BASICSYNAPSE_H_

#include "EvSimObjectTypeUtils.h"
#include "GenericBasicSynapse.h"

//! A basic synapse type
class BasicSynapse : public GenericBasicSynapse<Synapse > {

	EV_SIM_OBJECT( BasicSynapse )

public:
	BasicSynapse(float W = 1) :
		GenericBasicSynapse<Synapse >(W) {};

	~BasicSynapse() {};
};

//! A basic active synapse type
class BasicActiveSynapse : public GenericBasicSynapse< ActiveSynapse > {

	EV_SIM_OBJECT( BasicActiveSynapse )

public:
	BasicActiveSynapse(float W = 1) :
		GenericBasicSynapse< ActiveSynapse >(W) {};

	~BasicActiveSynapse() {};
};


#endif /* BASICSYNAPSE_H_ */
