#include "EvSimEngine.h"
#include "SimException.h"
#include <boost/format.hpp>
#include "ProjectedOutputPortIterator.h"

#include <algorithm>
using std::min;

RandomEngine * global_sim_random_eng;
SimContext * thread_active_sim_ctxt;

EvSimEngine::~EvSimEngine() {
}

void EvSimEngine::processEmittedEvent(global_out_port_id_t out_port_id, Time t)
{
	RoutingDelayMap::const_iterator delay_it;
	for (delay_it = delay_map.begin(out_port_id);
			delay_it != delay_map.end(out_port_id); ++delay_it) {
		if (t + delay_it->first == curr_sim_clock && (curr_sim_clock != 0) ) {
			deliverScheduledEvent(delay_it->second, curr_sim_clock);
		}
		else {
			scheduled_events_queue.scheduleEvent(delay_it->second, t + delay_it->first);
		}
	}
}

void EvSimEngine::deliverScheduledEvent(event_target_group_id_t etg, Time time)
{
	EventTargetGroupPool::const_iterator obj_it;
	for (obj_it = etg_pool.begin(etg) ; obj_it != etg_pool.end(etg) ; ++obj_it) {
		obj_it->receiver->eventHit(sim_context, time, obj_it->port);
	}

	PropagatedUpdateList::const_iterator upd_obj_it;
	for (upd_obj_it = upd_list.begin(etg) ; upd_obj_it != upd_list.end(etg) ; ++upd_obj_it) {
		upd_obj_it->object->updateState(sim_context, time, upd_obj_it->update_id);
		/*
		if (upd_obj_it->is_single_update_id) {
			upd_obj_it->object->updateState(sim_context, time, upd_obj_it->update_id);
		}
		else {
			UpdateIDVector *update_id_vec = upd_obj_it->update_id_vec;
			EvSimObject *obj = upd_obj_it->object;
			UpdateIDVector::const_iterator update_id_it = update_id_vec->begin();
			UpdateIDVector::const_iterator update_id_it_end = update_id_vec->end();
			for ( ; update_id_it != update_id_it_end; ++update_id_it) {
				obj->updateState(sim_context, time, *update_id_it);
			}
		}*/
	}
}



void EvSimEngine::advance(Time t)
{
	thread_active_sim_ctxt = &sim_context;
	global_sim_random_eng = static_cast<RandomEngine *>(&sim_random_eng);

	if (t == MAX_TIME) {
		throw evesim::Exception( "EvSimEngine::advance(Time t)",
				                  boost::str( boost::format("You have chosen an invalid end simulation time."
				                		             "The end simulation time must be smaller than the maximum value of type double equal to %1%" ) % MAX_TIME ) );
	}

	sim_context.t = curr_sim_clock;

	// first check if time t is larger than current state of the simulation clock
	if (t < curr_sim_clock) {
		throw (evesim::Exception( "EvSimEngine::advance", boost::str( boost::format( "Can not simulate the network up to time t=%1%s,"
				                  "because current simulation clock t = %2% is larger." ) % t % curr_sim_clock) ) );
	}

	Time next_sched_time;
	Time next_proj_time;

	while(true) {
		next_proj_time = next_sched_time = MAX_TIME;
		if (!scheduled_events_queue.empty())
			next_sched_time = scheduled_events_queue.topTime();
		if (!proj_events_buf.empty())
			next_proj_time = proj_events_buf.topTime();

		// if next event time is larger than t, we are done
		if (min(next_sched_time, next_proj_time) > t) {
			sim_context.t = curr_sim_clock = t;
			break;
		}

		if (next_proj_time < next_sched_time) {
			pair<EvSimObject*, global_out_port_id_t> proj_ev_info = proj_events_buf.topSender();
			proj_ev_info.first->announceEvEmitted(sim_context, next_proj_time,  proj_ev_info.second);
			processEmittedEvent(proj_ev_info.second, next_proj_time);
			last_event_time = next_proj_time;
		}
		else {
			event_target_group_id_t etg = scheduled_events_queue.top()->etg;
			scheduled_events_queue.pop();
			deliverScheduledEvent(etg, next_sched_time);
			last_event_time = next_sched_time;
		}
	}
}

void EvSimEngine::simulate(Time t)
{
	if (MAX_TIME - curr_sim_clock < t)
		throw evesim::Exception("EvSimEngine::simulate", boost::str( boost::format("Can not simulate the network for %1% sec because the simulation time will"
				                                                             "exceed maximum allowed double precision floating point value.") % t ) );
	advance(curr_sim_clock + t);
}

void EvSimEngine::registerProjectedOutputPorts(EvSimObject *obj)
{
	ProjectedEventSender *proj_ev_sender = dynamic_cast<ProjectedEventSender *>(obj);
	if (proj_ev_sender) {
		global_proj_out_port_id_t first_proj_port_id = proj_events_buf.registerProjectedOutPorts(obj);
		proj_ev_sender->setFirstProjOutPortGlobalID(first_proj_port_id);
		proj_ev_sender->setupProjectedOutPorts();
		global_out_port_id_t first_out_port_global_id = proj_ev_sender->getFirstOutPortGlobalID();
		if (obj->numProjOutputPorts() == 1) {
			pair<port_id_t, global_proj_out_port_id_t>  port_info = proj_ev_sender->getFirstProjectedPortInfo();
			proj_events_buf.setGlobalOutputPort(port_info.second, first_out_port_global_id + port_info.first);
		}
		else {
			ProjectedOutputPortIterator proj_port_it = proj_ev_sender->getProjectedPortIterator();
			while (proj_port_it.hasNextPort()) {
				pair<port_id_t, global_proj_out_port_id_t>  port_info = proj_port_it.nextPort();
				proj_events_buf.setGlobalOutputPort(port_info.second, first_out_port_global_id + port_info.first);
			}
		}
	}
}

void EvSimEngine::init()
{
	proj_events_buf.makeHeap();
	upd_list.prepare();
}

void EvSimEngine::reset() {
	global_sim_random_eng = static_cast<RandomEngine *>(&sim_random_eng);
	sim_context.t = Time(0);
	curr_sim_clock = 0;
	scheduled_events_queue.reset();
	obj_pool.resetObjects(sim_context);
}



