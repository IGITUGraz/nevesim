#ifndef PROJECTEDPORTSTESTSIMOBJECT_H_
#define PROJECTEDPORTSTESTSIMOBJECT_H_

#include "EvSimObject.h"
#include "ProjectedEventSender.h"
#include "EvSimObjectTypeUtils.h"

#include <vector>

class ProjectedPortsTestSimObject : public EvSimObject, public SingleOutputProjectedEventSender {
	EV_SIM_OBJECT( ProjectedPortsTestSimObject )

public:
	ProjectedPortsTestSimObject(const vector<Time> & projected_event_times = vector<Time>())
	    : projected_event_times(projected_event_times)
	{}

	virtual ~ProjectedPortsTestSimObject();

	virtual void reset(SimContext &sim_ctxt);

	void setProjectedEventTimes(const vector<Time> &projected_event_times) {
		this->projected_event_times = projected_event_times;
	}

	virtual unsigned numOutputPorts() const
	{
		return 1;
	};

	virtual unsigned numProjOutputPorts() const
	{
		return 1;
	};


	virtual PortType getOutputPortType(port_id_t p) const
	{
		if (p == 0)
			return eventPortProjected;
		return portUndefined;
	};


	virtual void eventHit(SimContext & sim_ctxt, Time time, port_id_t port);

	virtual void announceEvEmitted(SimContext & sim_ctxt, Time time, global_proj_out_port_id_t port_id);

protected:
	unsigned proj_ev_idx;
	unsigned update_times_idx;
	vector<Time> projected_event_times;
};

#endif /* PROJECTEDPORTSTESTSIMOBJECT_H_ */
