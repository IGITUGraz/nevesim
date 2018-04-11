#ifndef BINOMSEMSYNAPSE_H_
#define BINOMSEMSYNAPSE_H_

#include "PlasticSynapse.h"
#include "GenericBasicSynapse.h"
#include "GenericBinomSEMSynapse.h"

class BinomSEMSynapse : public GenericBinomSEMSynapse<GenericBasicSynapse<PlasticNSSynapse> >
{
	EV_SIM_OBJECT( BinomSEMSynapse )
public:

	BinomSEMSynapse(float Winit = 1,
					float eta = 1e-5,
					bool withHessian = false,
					bool activePlasticity = true,
					float T = 1,
					float Wnull = 0,
					float maxW = 5,
					float minW = -5,
					SpikeResponse *response = 0,
					SpikeResponse *bap_trace = 0)
	: GenericBinomSEMSynapse<GenericBasicSynapse<PlasticNSSynapse> >(Winit, eta, withHessian, activePlasticity,
																	           T, Wnull, maxW, minW, response, bap_trace) {}

	virtual ~BinomSEMSynapse() {};

};

class BinomSEMActiveSynapse : public GenericBinomSEMSynapse<
                                             GenericBasicSynapse< PlasticNSActiveSynapse > >
{
	EV_SIM_OBJECT( BinomSEMActiveSynapse )
public:

	BinomSEMActiveSynapse(float Winit = 1,
					float eta = 1e-5,
					bool withHessian = false,
					bool activePlasticity = true,
					float T = 1,
					float Wnull = 0,
					float maxW = 5,
					float minW = -5,
					SpikeResponse *response = 0,
					SpikeResponse *bap_trace = 0)
	: GenericBinomSEMSynapse<
	  	  	  GenericBasicSynapse<PlasticNSActiveSynapse > >(Winit, eta, withHessian, activePlasticity,
	  	  			  	  	  	  	  	  	  	  	  	  	  	  	  	  	   T, Wnull, maxW ,minW, response, bap_trace) {}

};


#endif /* BINOMSEMSYNAPSE_H_ */
