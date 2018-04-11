/*
 * ExternalEventGenerator.cpp
 *
 *  Created on: Jul 24, 2012
 *      Author: dejan
 */

#include "ExternalEventGenerator.h"
#include "SimException.h"

#include <boost/format.hpp>

EV_SIM_OBJECT_POST( ExternalEventGenerator )


ExternalEventGenerator::~ExternalEventGenerator()
{
	// NOOP
}

void ExternalEventGenerator::generateEvent(port_id_t port, Time time)
{
	if (port < num_out_ports)
		outputEvent(sim_ctxt, port, time);
	else
		throw evesim::Exception("ExternalEventGenerator::generateEvent",
				        boost::str( boost::format( "Can not generate event for port %1%. The event generator has %2% output ports" ) % port % num_out_ports ));
}

void ExternalEventGenerator::generateEvent(port_id_t port)
{
	generateEvent(port, sim_ctxt.net->getCurrentSimTime());
}
