/*
 * GenericBinaryInputSynapse.h
 *
 *  Created on: Jul 24, 2012
 *      Author: dejan
 */

#ifndef GENERICBINARYINPUTSYNAPSE_H_
#define GENERICBINARYINPUTSYNAPSE_H_


#include "globals.h"
#include "SimContext.h"


template<class BaseSyn>
class GenericBinaryInputSynapse : public BaseSyn {

public:
	GenericBinaryInputSynapse(float psr_level_hi = 5,
							  float psr_level_lo = 0,
							  bool isActive = true) {
		this->psr_level_hi = psr_level_hi;
		this->psr_level_lo = psr_level_lo;
		this->isActive = isActive;
	}

	float psr_level_hi;

	float psr_level_lo;

	bool isActive;

	virtual ~GenericBinaryInputSynapse() {}

	virtual void reset(SimContext &sim_ctxt);

	virtual void eventHit(SimContext & sim_ctxt, Time time, port_id_t port);

protected:
	enum { lvl_hi, lvl_lo } last_level;

};

template<class BaseSyn>
void GenericBinaryInputSynapse<BaseSyn>::eventHit(SimContext & sim_ctxt, Time time, port_id_t port)
{
	if (port == 0) { // set level to low
		last_level = lvl_lo;
		if (isActive) {
			BaseSyn::updatePSR(psr_level_lo, time);
		}
	}
	else if (port == 1) {  // set level to high
		last_level = lvl_hi;
		if (isActive) {
			BaseSyn::updatePSR(psr_level_hi, time);
		}
	}
	else if (port == 2) { // activate
		if (!isActive) {
			BaseSyn::updatePSR((last_level == lvl_hi) ? psr_level_hi : psr_level_lo, time);
			isActive = true;
		}

	}
	else if (port == 3) { // inactivate
		if (isActive) {
			BaseSyn::updatePSR(0, time);
			isActive = false;
		}
	}
	else if (port == 4) { // change from active to inactive and vice-versa
		if (isActive) {
			isActive = false;
			BaseSyn::updatePSR(0, time);
		}
		else {
			isActive = true;
			BaseSyn::updatePSR((last_level == lvl_hi) ? psr_level_hi : psr_level_lo, time);
		}
	}
}


template<class BaseSyn>
void GenericBinaryInputSynapse<BaseSyn>::reset(SimContext &sim_ctxt)
{
	BaseSyn::resetPSRs();
}

#endif /* GENERICBINARYINPUTSYNAPSE_H_ */
