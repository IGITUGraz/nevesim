#ifndef EVENTTIMESRECORDER_H_
#define EVENTTIMESRECORDER_H_

#include "EvSimObject.h"
#include "EvSimObjectTypeUtils.h"

#include <vector>
using std::vector;

#include <boost/shared_ptr.hpp>

class EventTimesRecorder : public EvSimObject {

	EV_SIM_OBJECT( EventTimesRecorder )

public:

	EventTimesRecorder(bool isActive = true);
	virtual ~EventTimesRecorder();

	bool isActive;

	virtual unsigned numInputPorts()
	{
		return 1;
	}

	virtual void eventHit(SimContext & sim_info, Time time, port_id_t port);

	virtual const std::vector<Time> & getRecordedValues();

	void clearRecordedValues() {
		recorded_events.clear();
	}

protected:
	vector<Time> recorded_events;
};

#endif /* EVENTTIMESRECORDER_H_ */
