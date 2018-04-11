/*
 * BinaryArrayPulseGenerator.cpp
 *
 *  Created on: Jul 24, 2012
 *      Author: dejan
 */

#include "BinaryArrayPulseGenerator.h"

EV_SIM_OBJECT_POST( BinaryArrayPulseGenerator )


void BinaryArrayPulseGenerator::eventHit(SimContext & sim_ctxt, Time time, port_id_t port)
{
	if (port == 0) {
			if (curr_idx == bin_array.size()) curr_idx = 0;
			if (isActive)
				outputEvent(sim_ctxt, (bin_array[curr_idx]) ? 1 : 0, time);
			curr_idx++;
	}
}


void BinaryArrayPulseGenerator::reset(SimContext &sim_ctxt)
{
	curr_idx = 0;
}


