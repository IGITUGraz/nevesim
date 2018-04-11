#include "EventTimesRecorder.h"

EV_SIM_OBJECT_POST( EventTimesRecorder )

EventTimesRecorder::EventTimesRecorder(bool isActive)
      : isActive(isActive)
{

}

EventTimesRecorder::~EventTimesRecorder() {}

void EventTimesRecorder::eventHit(SimContext & sim_info, Time time, port_id_t port)
{
	if (isActive)
		recorded_events.push_back(time);
}

const std::vector<Time> & EventTimesRecorder::getRecordedValues()
{
	return recorded_events;
}
