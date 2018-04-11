#include "ProjectedPortsTestSimObject.h"

EV_SIM_OBJECT_POST( ProjectedPortsTestSimObject )

ProjectedPortsTestSimObject::~ProjectedPortsTestSimObject()
{
}

void ProjectedPortsTestSimObject::reset(SimContext &sim_ctxt)
{

	proj_ev_idx = 0;
	update_times_idx = 0;
	if (projected_event_times.size())
		changeProjectedEventTime(sim_ctxt, port_id_t(0), projected_event_times[0]);
}

void ProjectedPortsTestSimObject::eventHit(SimContext & sim_ctxt, Time time, port_id_t port)
{
	if (update_times_idx == proj_ev_idx) {
		if (proj_ev_idx < projected_event_times.size() - 1) {
			proj_ev_idx++;
			changeProjectedEventTime(sim_ctxt, port_id_t(0), projected_event_times[proj_ev_idx]);
		}
		else {
			changeProjectedEventTime(sim_ctxt, port_id_t(0), MAX_TIME);
			return ;
		}
	}
	update_times_idx++;
}

void ProjectedPortsTestSimObject::announceEvEmitted(SimContext &sim_ctxt, Time time, global_proj_out_port_id_t port_id)
{
	if (proj_ev_idx < projected_event_times.size() - 1) {
		proj_ev_idx++;
		changeProjectedEventTime(sim_ctxt, port_id_t(0), projected_event_times[proj_ev_idx]);
	}
	else {
		changeProjectedEventTime(sim_ctxt, port_id_t(0), MAX_TIME);
	}
}
