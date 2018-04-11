#ifndef DirectEventSender_H_
#define DirectEventSender_H_

#include <cstdlib>

#include "globals.h"
#include "SimContext.h"

#include  "EvSimObject.h"
#include "evesim_hash.h"
#include "EvSimEngine.h"

class DirectEventSender
{
public:

    virtual ~DirectEventSender() {};

    void outputEvent(SimContext & sim_context, port_id_t port, Time evtime);

    void setFirstOutPortGlobalID(global_out_port_id_t port_id);

    global_out_port_id_t getFirstOutPortGlobalID() const;

    global_out_port_id_t getOutPortGlobalID(port_id_t port) const;
protected:
    global_out_port_id_t first_out_port_global_id;
};

inline void DirectEventSender::outputEvent(SimContext & sim_context, port_id_t port, Time evtime)
{
  sim_context.eng->scheduleEvent(first_out_port_global_id + port, evtime);
}

inline void DirectEventSender::setFirstOutPortGlobalID(global_proj_out_port_id_t port_id)
{
	first_out_port_global_id = port_id;
}

inline global_out_port_id_t DirectEventSender::getFirstOutPortGlobalID() const
{
	return first_out_port_global_id;
}

inline global_out_port_id_t DirectEventSender::getOutPortGlobalID(port_id_t port) const
{
	EvSimObject *obj = dynamic_cast<EvSimObject *>(const_cast<DirectEventSender *>(this));
	if (port < obj->numOutputPorts())
		return first_out_port_global_id + port;
	else
		return PORT_NOT_DEFINED;
}

#endif /*DirectEventSender_H_*/
