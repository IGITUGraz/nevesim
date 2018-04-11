#ifndef NSHEBBIANSYNAPSE_H_
#define NSHEBBIANSYNAPSE_H_

#include "PlasticSynapse.h"
#include "GenericBasicSynapse.h"
#include "GenericNSHebbianSynapse.h"
#include "EvSimObjectTypeUtils.h"


class NSHebbianSynapse : public GenericNSHebbianSynapse<GenericBasicSynapse<PlasticNSSynapse> >
{
	EV_SIM_OBJECT( NSHebbianSynapse )
public:

	NSHebbianSynapse(float Winit = 1,
					float eta = 1e-5,
					bool activePlasticity = true,
					float maxW = 5,
					float minW = -5,
					short sign = 1,
					SpikeResponse *response = 0,
					SpikeResponse *bap_trace = 0)
	: GenericNSHebbianSynapse<GenericBasicSynapse<PlasticNSSynapse> >(Winit, eta, activePlasticity,
																maxW, minW, sign, response, bap_trace) {}

	virtual ~NSHebbianSynapse() {};

};

class NSHebbianActiveSynapse : public GenericNSHebbianSynapse<
                                             GenericBasicSynapse<PlasticNSActiveSynapse > >
{
	EV_SIM_OBJECT( NSHebbianActiveSynapse )
public:

	NSHebbianActiveSynapse(float Winit = 1,
					float eta = 1e-5,
					bool activePlasticity = true,
					float maxW = 5,
					float minW = -5,
					short sign = 1,
					SpikeResponse *response = 0,
					SpikeResponse *bap_trace = 0)
	    : GenericNSHebbianSynapse<
	  	  	    GenericBasicSynapse<PlasticNSActiveSynapse > >(Winit, eta, activePlasticity,
	  	  	    		                              maxW, minW, sign, response, bap_trace) {}

};


#endif /* NSHEBBIANSYNAPSE_H_ */
