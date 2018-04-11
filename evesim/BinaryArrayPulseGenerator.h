/*
 * BinaryArrayPulseGenerator.h
 *
 *  Created on: Jul 24, 2012
 *      Author: dejan
 */

#ifndef BINARYARRAYPULSEGENERATOR_H_
#define BINARYARRAYPULSEGENERATOR_H_

#include "EvSimObject.h"
#include "EvSimObjectTypeUtils.h"
#include "DirectEventSender.h"

class BinaryArrayPulseGenerator : public EvSimObject, public DirectEventSender {

	EV_SIM_OBJECT( BinaryArrayPulseGenerator )

public:

	BinaryArrayPulseGenerator(const std::vector<bool> & bin_array = vector<bool>(),
			                  bool isActive = true) :
			                  isActive(isActive), bin_array(bin_array)
			                  {}


	bool isActive;

	const std::vector<bool> & getBinaryArray() {
		return bin_array;
	}

	virtual unsigned numOutputPorts() const
	{
		return 2;
	};

	virtual EvSimObject::PortType outputPortType(port_id_t p) const
	{
		if (p == 0 || p == 1)
			return EvSimObject::eventPortDirect;
		return EvSimObject::portUndefined;
	}


	void setBinaryArray(const std::vector<bool> & bin_array) {
		this->bin_array = bin_array;
	}

	virtual ~BinaryArrayPulseGenerator() {}


	virtual void reset(SimContext &sim_ctxt);

	virtual void eventHit(SimContext & sim_ctxt, Time time, port_id_t port);

protected:
	vector<bool> bin_array;
	unsigned curr_idx;
};


#endif /* BINARYARRAYPULSEGENERATOR_H_ */
