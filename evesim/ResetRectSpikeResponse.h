#ifndef ResetRectSpikeResponse_H_
#define ResetRectSpikeResponse_H_

#include "EvSimObject.h"
#include "DirectEventSender.h"
#include "SpikeResponse.h"
#include "EvSimObjectTypeUtils.h"


class ResetRectSpikeResponse : public SpikeResponse, public DirectEventSender {

	EV_SIM_OBJECT( ResetRectSpikeResponse )

public:
	ResetRectSpikeResponse(float tau = 10e-3) {
		this->tau = tau;
	}

	virtual ~ResetRectSpikeResponse() {};

	float tau;

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
		r = r_old = 0;
		spk_counter = 0;
	};

	virtual void postAddedInNetwork(EvSimNetwork &net, EvSimObject::ID self) {
		net.connect(self, 0, self, 1);
	};

protected:
	unsigned spk_counter;
};


#endif /* ResetRectSpikeResponse_H_ */
