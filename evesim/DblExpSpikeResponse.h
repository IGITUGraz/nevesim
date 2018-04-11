#ifndef DblExpSpikeResponse_H_
#define DblExpSpikeResponse_H_

#include "EvSimObject.h"
#include "DirectEventSender.h"
#include "SpikeResponse.h"
#include "EvSimObjectTypeUtils.h"


class DblExpSpikeResponse : public SpikeResponse, public DirectEventSender {

	EV_SIM_OBJECT( DblExpSpikeResponse )

public:

	DblExpSpikeResponse(double tau_rise, double tau_fall, Time t_update)
		: t_update(t_update), tau_rise(tau_rise), tau_fall(tau_fall)
	{}

	virtual ~DblExpSpikeResponse() {};

	virtual void eventHit(SimContext & sim_info, Time time, port_id_t port);

	virtual void updateState(SimContext &sim_info, Time time, update_id_t upd_id) {};

	virtual unsigned numInputPorts() const
	{
		return 2;
	};

	virtual unsigned numOutputPorts() const
	{
		return 1;
	};


	virtual PortType getInputPortType(port_id_t p) const
	{
		if (p < 2)
			return eventPortDirect;
		return portUndefined;
	};

	virtual PortType getOutputPortType(port_id_t p) const
	{
		if (p == 0)
			return eventPortDirect;
		return portUndefined;
	};

	virtual void reset(SimContext &sim_ctxt)
	{
		last_update_time = 0;
		r = r_old = r_rise = r_fall = 0;
	};

	virtual void postAddedInNetwork(EvSimNetwork &net, EvSimObject::ID self);

protected:
	static const double MIN_VM;
	const Time t_update;
	const double tau_rise;
	const double tau_fall;

	double r_rise;
	double r_fall;

	Time last_update_time;
};

#endif /* DblExpSpikeResponse_H_ */

