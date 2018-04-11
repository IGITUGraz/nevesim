#include "EvSimRegularClock.h"

EV_SIM_OBJECT_POST(EvSimRegularClock)

FIELD_REGISTRATOR(EvSimRegularClock)
{
	REGISTER_FIELD(EvSimRegularClock, tic_interval)
}

EvSimRegularClock::EvSimRegularClock(Time tic_interval, bool isActive, Time start_time, Time end_time)
                : isActive(isActive), end_time(end_time), tic_interval(tic_interval), start_time(start_time) {
}

EvSimRegularClock::~EvSimRegularClock() {
}

void EvSimRegularClock::reset(SimContext &sim_ctxt) {
	outputEvent(sim_ctxt, 1, start_time);
}

void EvSimRegularClock::eventHit(SimContext & sim_ctxt, Time time, port_id_t port)
{
	if (time+tic_interval < end_time)
		outputEvent(sim_ctxt, 1, time+tic_interval);
	if (isActive)
		outputEvent(sim_ctxt, 0, time);
}


void EvSimRegularClock::postAddedInNetwork(EvSimNetwork &net, EvSimObject::ID self)
{
	net.connect(self, 1, self, 0);
}


