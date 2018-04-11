#ifndef BinomNSamplSEMSYNAPSE_H_
#define BinomNSamplSEMSYNAPSE_H_

#include "PlasticSynapse.h"
#include "GenericBasicSynapse.h"
#include "GenericBinomNSamplSEMSynapse.h"

class BinomNSamplSEMSynapse : public GenericBinomNSamplSEMSynapse<GenericBasicSynapse<PlasticNSSynapse> >
{
	EV_SIM_OBJECT( BinomNSamplSEMSynapse )
public:

	BinomNSamplSEMSynapse(float Winit = 1,
					float eta = 1e-5,
					bool withHessian = false,
					bool activePlasticity = true,
					float T = 1,
					float Wnull = 0,
					float maxW = 5,
					float minW = -5,
					SpikeResponse *response = 0,
					SpikeResponse *bap_trace = 0)
	: GenericBinomNSamplSEMSynapse<GenericBasicSynapse<PlasticNSSynapse> >(Winit,eta,withHessian,activePlasticity,
			 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 	 T, Wnull, maxW,minW, response, bap_trace) {}

	virtual ~BinomNSamplSEMSynapse() {};

};

class BinomNSamplSEMActiveSynapse : public GenericBinomNSamplSEMSynapse<
                                             GenericBasicSynapse< PlasticNSActiveSynapse > >
{
	EV_SIM_OBJECT( BinomNSamplSEMActiveSynapse )
public:

	BinomNSamplSEMActiveSynapse(float Winit = 1,
					float eta = 1e-5,
					bool withHessian = false,
					bool activePlasticity = true,
					float T = 1,
					float Wnull = 0,
					float maxW = 5,
					float minW = -5,
					SpikeResponse *response = 0,
					SpikeResponse *bap_trace = 0)
	: GenericBinomNSamplSEMSynapse<
	  	  	  GenericBasicSynapse<PlasticNSActiveSynapse > >(Winit,eta,withHessian,activePlasticity, T, Wnull,
	  	  			                                                					maxW,minW, response, bap_trace) {}

};


#endif /* BinomNSamplSEMSYNAPSE_H_ */
