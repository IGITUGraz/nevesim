#ifndef PopulCodeSEMSYNAPSE_H_
#define PopulCodeSEMSYNAPSE_H_

#include "PlasticSynapse.h"
#include "GenericBasicSynapse.h"
#include "GenericPopulCodeSEMSynapse.h"

class PopulCodeSEMSynapse : public GenericPopulCodeSEMSynapse<GenericBasicSynapse<PlasticNSSynapse> >
{
	EV_SIM_OBJECT( PopulCodeSEMSynapse )
public:

	PopulCodeSEMSynapse(float Winit = 1,
					float eta = 1e-5,
					bool withHessian = false,
					bool activePlasticity = true,
					float T = 1.0,
					float Wnull = 0.0,
					float maxW = 5,
					float minW = -5,
					SpikeResponse *response = 0,
					SpikeResponse *bap_trace = 0)
	: GenericPopulCodeSEMSynapse<GenericBasicSynapse<PlasticNSSynapse> >(Winit,eta,withHessian,activePlasticity,
			                                                                 T, Wnull, maxW, minW, response, bap_trace) {}

	virtual ~PopulCodeSEMSynapse() {};

};

class PopulCodeSEMActiveSynapse : public GenericPopulCodeSEMSynapse<
                                             GenericBasicSynapse< PlasticNSActiveSynapse > >
{
	EV_SIM_OBJECT( PopulCodeSEMActiveSynapse )
public:

	PopulCodeSEMActiveSynapse(float Winit = 1,
					float eta = 1e-5,
					bool withHessian = false,
					bool activePlasticity = true,
					float T = 1.0,
					float Wnull = 0.0,
					float maxW = 5,
					float minW = -5,
					SpikeResponse *response = 0,
					SpikeResponse *bap_trace = 0)
	: GenericPopulCodeSEMSynapse<
	  	  	  GenericBasicSynapse<PlasticNSActiveSynapse > >(Winit,eta,withHessian,activePlasticity,
	  	  			                                                  T, Wnull, maxW, minW, response, bap_trace) {}

	virtual ~PopulCodeSEMActiveSynapse() {};

};


#endif /* PopulCodeSEMSYNAPSE_H_ */
