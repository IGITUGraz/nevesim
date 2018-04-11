#ifndef TESTERPROJECTEDEVENTSENDER_H_
#define TESTERPROJECTEDEVENTSENDER_H_

#include "TestEvSimObject.h"
#include "ProjectedEventSender.h"


class TesterSingleOutProjectedEventSender : public TestEvSimObject, public SingleOutputProjectedEventSender {

EV_SIM_OBJECT( TesterSingleOutProjectedEventSender )

public:
	TesterSingleOutProjectedEventSender();
	virtual ~TesterSingleOutProjectedEventSender();

	virtual PortType getOutputPortType(port_id_t p) {
		if (p == 0)
			return eventPortProjected;
		return portUndefined;
	}

};

class TesterMultiOutProjectedEventSender : public TestEvSimObject, public MultiOutputProjectedEventSender {

	EV_SIM_OBJECT( TesterMultiOutProjectedEventSender )

public:
	TesterMultiOutProjectedEventSender();
	virtual ~TesterMultiOutProjectedEventSender();

	void setOutPortTypes(vector<PortType> port_types) {
		this->port_types = port_types;
	}

	virtual PortType getOutputPortType(port_id_t p) const {
		if (p >= port_types.size())
			return portUndefined;
		return port_types[p];
	}

protected:
	vector<PortType> port_types;
};



#endif /* TESTERPROJECTEDEVENTSENDER_H_ */
