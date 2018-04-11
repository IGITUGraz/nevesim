/*
 * ExternalEventGenerator.h
 *
 *  Created on: Jul 24, 2012
 *      Author: dejan
 */

#ifndef EXTERNALEVENTGENERATOR_H_
#define EXTERNALEVENTGENERATOR_H_

#include "EvSimObject.h"
#include "EvSimObjectTypeUtils.h"
#include "DirectEventSender.h"

class ExternalEventGenerator : public EvSimObject, public DirectEventSender {

	EV_SIM_OBJECT( ExternalEventGenerator )

public:
	ExternalEventGenerator(unsigned  num_out_ports) : num_out_ports(num_out_ports) {};
	virtual ~ExternalEventGenerator();

	virtual unsigned numOutputPorts() const
	{
		return num_out_ports;
	};

	virtual PortType getOutputPortType(port_id_t p) const
	{
		if (p < num_out_ports)
			return eventPortDirect;
		return portUndefined;
	};

	virtual void reset(SimContext &sim_ctxt)
	{
		this->sim_ctxt = sim_ctxt;
	};


	virtual void generateEvent(port_id_t out_port, Time time);

	virtual void generateEvent(port_id_t out_port);

protected:
	SimContext sim_ctxt;
	unsigned num_out_ports;
};

#endif /* EXTERNALEVENTGENERATOR_H_ */
