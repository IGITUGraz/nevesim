#ifndef PROJECTEDEVENTSENDER_H_
#define PROJECTEDEVENTSENDER_H_

#include "globals.h"
#include "EvSimObject.h"
#include "SimContext.h"
#include "DirectEventSender.h"

#include "evesim_hash.h"

#ifndef SWIG

#include <utility>
using std::make_pair;

#endif

class ProjectedOutputPortIterator;


class ProjectedEventSender : public DirectEventSender {
public:
	virtual ~ProjectedEventSender();

	virtual global_proj_out_port_id_t getProjOutPortGlobalID(port_id_t port_id) const = 0;

	virtual void setFirstProjOutPortGlobalID(global_proj_out_port_id_t port_id) = 0;

	virtual void changeProjectedEventTime(SimContext &ai, port_id_t port_id,Time new_time);

	virtual void setupProjectedOutPorts() = 0;

	virtual ProjectedOutputPortIterator getProjectedPortIterator() = 0;

	virtual pair<port_id_t, global_proj_out_port_id_t> getFirstProjectedPortInfo() = 0;

};




class SingleOutputProjectedEventSender : public ProjectedEventSender {
public:
	virtual ~SingleOutputProjectedEventSender();

	virtual global_proj_out_port_id_t getProjOutPortGlobalID(port_id_t port_id) const;

	virtual void setFirstProjOutPortGlobalID(global_proj_out_port_id_t port_id);

	virtual ProjectedOutputPortIterator getProjectedPortIterator();

	virtual pair<port_id_t, global_proj_out_port_id_t> getFirstProjectedPortInfo();

	virtual void setupProjectedOutPorts() {};

protected:
	global_proj_out_port_id_t first_proj_port_global_id;
};


class MultiOutputProjectedEventSender : public ProjectedEventSender {

public:
	virtual ~MultiOutputProjectedEventSender() {};

	virtual global_proj_out_port_id_t getProjOutPortGlobalID(port_id_t port_id) const;

	virtual void setFirstProjOutPortGlobalID(global_proj_out_port_id_t port_id);

	virtual void setupProjectedOutPorts();

	virtual ProjectedOutputPortIterator getProjectedPortIterator();

	virtual pair<port_id_t, global_proj_out_port_id_t> getFirstProjectedPortInfo();

protected:
	typedef unordered_map< port_id_t, global_proj_out_port_id_t, evesim::hash<port_id_t> > ProjOutPortsGlobalIDMap;

	friend class ProjectedOutputPortIterator;

	ProjOutPortsGlobalIDMap proj_out_ports_global_ids;
};


#endif /* PROJECTEDEVENTSENDER_H_ */
