/*
 * MultiValArrayPulseGenerator.h
 *
 *  Created on: Jul 24, 2012
 *      Author: dejan
 */

#ifndef MultiValArrayPULSEGENERATOR_H_
#define MultiValArrayPULSEGENERATOR_H_

#include "EvSimObject.h"
#include "EvSimObjectTypeUtils.h"
#include "DirectEventSender.h"

class MultiValArrayPulseGenerator : public EvSimObject, public DirectEventSender {

	EV_SIM_OBJECT( MultiValArrayPulseGenerator )

public:

	MultiValArrayPulseGenerator(unsigned short num_val = 2,
								const std::vector<unsigned short> & array = vector<unsigned short>(),
			                    bool isActive = true) :
			                    isActive(isActive), array(array), num_val(num_val)
			                  {}


	bool isActive;

	const std::vector<unsigned short> & getMultiValArray() {
		return array;
	}

	virtual unsigned numOutputPorts() const
	{
		return num_val;
	};

	virtual EvSimObject::PortType outputPortType(port_id_t p) const
	{
		if (p < num_val)
			return EvSimObject::eventPortDirect;
		return EvSimObject::portUndefined;
	}


	void setMultiValArray(const std::vector<unsigned short> & array) {
		this->array = array;
	}

	virtual ~MultiValArrayPulseGenerator() {}

	virtual void reset(SimContext &sim_ctxt);

	virtual void eventHit(SimContext & sim_ctxt, Time time, port_id_t port);

protected:
	vector<unsigned short> array;
	unsigned short num_val;
	unsigned curr_idx;
};


#endif /* MultiValArrayPULSEGENERATOR_H_ */
