#ifndef ArrayInputNeuron_H_
#define ArrayInputNeuron_H_

#include "EvSimObject.h"
#include "EvSimObjectTypeUtils.h"
#include "SharedArray.h"
#include "DirectEventSender.h"

#include <vector>
using std::vector;

class ArrayInputNeuron : public EvSimObject, public DirectEventSender
{

	EV_SIM_OBJECT( ArrayInputNeuron )

public:

	ArrayInputNeuron(const std::vector<double> &input_events = vector<Time>()) :
		input_events(new vector<Time>(input_events)) {
		checkValidInputEvents(*this->input_events);
	}

	ArrayInputNeuron(const EventTimesVector & input_events) :
		input_events(input_events) {
		if (input_events != 0) {
			checkValidInputEvents(*input_events);
		}
	}

	virtual ~ArrayInputNeuron() {};

	virtual void eventHit(SimContext & sim_info, Time time, port_id_t port);

	virtual void updateState(SimContext &sim_info, Time time, update_id_t upd_id) {};

	void setInputEvents(const EventTimesVector &input_events) {
		checkValidInputEvents(*input_events);
		this->input_events = input_events;
	}

	void setInputEvents(const std::vector<Time> &input_events) {
		checkValidInputEvents(input_events);
		this->input_events = EventTimesVector(new vector<Time>(input_events));
	}

	const EventTimesVector &getEventArray2() const {
		return input_events;
	}

	const std::vector<double> &getEventArray() const {
			return *input_events;
		}

	virtual unsigned numOutputPorts() const
	{
		return 1;
	};

	virtual PortType getOutputPortType(port_id_t p) const
	{
		if (p == 0)
			return eventPortDirect;
		return portUndefined;
	};

	virtual void reset(SimContext &sim_ctxt);

	virtual void postAddedInNetwork(EvSimNetwork &net, EvSimObject::ID self);

protected:
	void checkValidInputEvents(const vector<Time> &inp_events);

	EventTimesVector input_events;

	unsigned curr_event_idx;
};



#endif /* ArrayInputNeuron_H_ */
