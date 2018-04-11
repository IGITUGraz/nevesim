#ifndef EVSIMENGINE_H_
#define EVSIMENGINE_H_

#include "EvSimNetwork.h"
#include "globals.h"
#include "EvSimObjectPool.h"
#include "EvSimObject.h"
#include "PropagatedUpdateList.h"
#include "ProjectedEventsHeapBuffer.h"
#include "ScheduledEventsQueue.h"
#include "RoutingDelayMap.h"
#include "RandomEngine.h"
#include "SimContext.h"


//! Provides the core functionality for managing events
class EvSimEngine {

public:
	EvSimEngine(EvSimNetwork &net, EvSimObjectPool &obj_pool, RoutingDelayMap &delay_map,
			EventTargetGroupPool &etg_pool, UpdateLinksMap &upd_links_map) :
				obj_pool(obj_pool), delay_map(delay_map), etg_pool(etg_pool),
				upd_list(etg_pool, upd_links_map, obj_pool)
	{
		sim_context = SimContext(&net, this, &proj_events_buf);
		sim_random_eng.seed(net.sim_params.rnd_seed);
	};

	virtual ~EvSimEngine();

	void scheduleEvent( global_out_port_id_t port_id, Time t);

	void init();

	void reset();

	void simulate(Time t);

	void advance(Time t);

	void setRNDSeed(unsigned seed) {
		sim_random_eng.seed(seed);
	}

	void registerProjectedOutputPorts(EvSimObject *obj);

	void deliverScheduledEvent(event_target_group_id_t etg, Time time);

protected:

	friend class EvSimNetwork;

	typedef enum { evProjected, evScheduled }  EventType;

	Time last_event_time;
	Time curr_sim_clock;

	EvSimObjectPool &obj_pool;

	ProjectedEventsHeapBuffer proj_events_buf;

	ScheduledEventsQueue scheduled_events_queue;

	RoutingDelayMap & delay_map;

	EventTargetGroupPool & etg_pool;

	PropagatedUpdateList upd_list;

	SimContext sim_context;

	void processEmittedEvent(global_out_port_id_t out_port_id, Time time);

	MersenneTwister19937 sim_random_eng;
};

inline void EvSimEngine::scheduleEvent( global_out_port_id_t port_id, Time t)
{
	processEmittedEvent(port_id, t);
}


extern RandomEngine * global_sim_random_eng;
extern SimContext * thread_active_sim_ctxt;

#endif /* EVSIMENGINE_H_ */
