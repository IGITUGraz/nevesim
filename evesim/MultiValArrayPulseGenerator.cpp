/*
 * MultiValArrayPulseGenerator.cpp
 *
 *  Created on: Jul 24, 2012
 *      Author: dejan
 */

#include "MultiValArrayPulseGenerator.h"

EV_SIM_OBJECT_POST( MultiValArrayPulseGenerator )


void MultiValArrayPulseGenerator::eventHit(SimContext & sim_ctxt, Time time, port_id_t port)
{
	if (port == 0) {
			if (curr_idx == array.size()) curr_idx = 0;
			if (isActive)
				outputEvent(sim_ctxt, array[curr_idx], time);
			curr_idx++;
	}
}


void MultiValArrayPulseGenerator::reset(SimContext &sim_ctxt)
{
	curr_idx = 0;
}


