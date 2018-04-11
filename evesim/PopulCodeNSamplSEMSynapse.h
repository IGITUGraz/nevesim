#ifndef PopulCodeNSamplSEMSynapse_H_
#define PopulCodeNSamplSEMSynapse_H_

#include "PlasticSynapse.h"
#include "GenericBasicSynapse.h"
#include "GenericPopulCodeNSamplSEMSynapse.h"

class PopulCodeNSamplSEMSynapse : public GenericPopulCodeNSamplSEMSynapse<GenericBasicSynapse<PlasticNSSynapse> >
{
	EV_SIM_OBJECT( PopulCodeNSamplSEMSynapse )
public:

	PopulCodeNSamplSEMSynapse(float Winit = 1,
					float eta = 1e-5,
					bool withHessian = false,
					bool activePlasticity = true,
					float T = 1.0,
					float Wnull = 0.0,
					float maxW = 5,
					float minW = -5,
					SpikeResponse *response = 0,
					SpikeResponse *bap_trace = 0)
	: GenericPopulCodeNSamplSEMSynapse<GenericBasicSynapse<PlasticNSSynapse> >(Winit,eta,withHessian,activePlasticity,
			                                                                        T, Wnull, maxW, minW, response, bap_trace) {}

	virtual ~PopulCodeNSamplSEMSynapse() {};

};

class PopulCodeNSamplSEMActiveSynapse : public GenericPopulCodeNSamplSEMSynapse<
                                             GenericBasicSynapse< PlasticNSActiveSynapse > >
{
	EV_SIM_OBJECT( PopulCodeNSamplSEMActiveSynapse )
public:

	PopulCodeNSamplSEMActiveSynapse(float Winit = 1,
					float eta = 1e-5,
					bool withHessian = false,
					bool activePlasticity = true,
					float T = 1.0,
					float Wnull = 0.0,
					float maxW = 5,
					float minW = -5,
					SpikeResponse *response = 0,
					SpikeResponse *bap_trace = 0)
	: GenericPopulCodeNSamplSEMSynapse<
	  	  	  GenericBasicSynapse<PlasticNSActiveSynapse > >(Winit, eta, withHessian, activePlasticity,
	  	  			                                                  T, Wnull, maxW,minW, response, bap_trace) {}

};


#endif /* PopulCodeNSamplSEMSynapse_H_ */
