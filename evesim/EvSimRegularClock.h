#ifndef EVSIMRegularCLOCK_H_
#define EVSIMRegularCLOCK_H_

#include "EvSimObject.h"
#include "DirectEventSender.h"
#include "SimContext.h"
#include "EvSimObjectTypeUtils.h"

class EvSimRegularClock : public EvSimObject, public DirectEventSender {

	EV_SIM_OBJECT( EvSimRegularClock )

public:

	EvSimRegularClock(Time tic_interval = 1e-3,
			          bool isActive = true,
			          Time start_time = 0,
			          Time end_time = MAX_TIME);

	virtual ~EvSimRegularClock();

	bool isActive;

	Time end_time;

	virtual void reset(SimContext &sim_ctxt);

	virtual void eventHit(SimContext &sim_ctxt, Time time, port_id_t port);

	virtual unsigned numOutputPorts() const
	{
		return 2;
	}

	virtual EvSimObject::PortType outputPortType(port_id_t p) const
	{
		if (p == 0 || p == 1)
			return EvSimObject::eventPortDirect;
		return EvSimObject::portUndefined;
	}

	void setTickInterval(Time tic_interval)
	{
		this->tic_interval = tic_interval;
	}

	virtual void postAddedInNetwork(EvSimNetwork &net, EvSimObject::ID self);

protected:
	Time tic_interval;
	Time start_time;
};


#endif /* EVSIMRegularCLOCK_H_ */
