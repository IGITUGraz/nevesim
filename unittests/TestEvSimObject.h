#ifndef TESTEVSIMOBJECT_H_
#define TESTEVSIMOBJECT_H_

#include "EvSimObject.h"
#include "EvSimObjectTypeUtils.h"
#include "DirectEventSender.h"

#include <vector>
#include <utility>

using std::vector;
using std::pair;


class TestEvSimObject : public EvSimObject {

   EV_SIM_OBJECT( TestEvSimObject )

public:

	TestEvSimObject() {};
	virtual ~TestEvSimObject() {};

	void setNumProjOutputPorts(unsigned n) {
		num_proj_out_ports = n;
	}

	virtual unsigned numProjOutputPorts() const
	{
		return num_proj_out_ports;
	}

	void setNumOutputPorts(unsigned n) {
		num_out_ports = n;
	}

	virtual unsigned numOutputPorts() const
	{
		return num_out_ports;
	}

	virtual PortType getOutputPortType(port_id_t) const
	{
		return eventPortDirect;
	};

	virtual void eventHit(SimContext & sim_ctxt, Time time, port_id_t port) {
		recv_events.push_back(std::make_pair(port, time));
	};

	virtual void updateState(SimContext &sim_ctxt, Time time, update_id_t upd_id) {
		recv_updates.push_back(std::make_pair(upd_id, time));
	};

	const vector<pair<port_id_t, Time> > & getRecvEvents() const {
		return recv_events;
	}

	const vector<pair<update_id_t, Time> > & getRecvUpdates() const {
		return recv_updates;
	}

protected:
	int num_proj_out_ports;
	int num_out_ports;
	vector<pair<port_id_t, Time> > recv_events;
	vector<pair<update_id_t, Time> > recv_updates;

};

class TesterDirectEventSender : public TestEvSimObject, public DirectEventSender
{
	EV_SIM_OBJECT( TesterDirectEventSender )
public:
	TesterDirectEventSender() {};
	virtual ~TesterDirectEventSender() {};

};



#endif /* TESTEVSIMOBJECT_H_ */
