#ifndef PLASTICSYNAPSE_H_
#define PLASTICSYNAPSE_H_

#include "Synapse.h"

class PlasticNSSynapse : public Synapse {
public:

protected:
	void updatePSR(double new_psr, Time) {
		psr_old = psr;
		psr = new_psr;
	}

	double psr_old;
};

class PlasticNSActiveSynapse : public ActiveSynapse {

};


#endif /* PLASTICSYNAPSE_H_ */
