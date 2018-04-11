#ifndef SPIKERESPONSE_H_
#define SPIKERESPONSE_H_

#include "EvSimObject.h"

class SpikeResponse : public EvSimObject {

public:
	virtual ~SpikeResponse();

	inline double getLastChange() {return r - r_old;};

	inline double get() { return r; };

	inline double getOld(){ return r_old;}
	
	virtual double getSlope() { return 0; }

protected:

	double r;
	double r_old;
};

#endif /* SPIKERESPONSE_H_ */
