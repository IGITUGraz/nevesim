#include "ProjectedEventSender.h"
#include "ProjectedEventsHeapBuffer.h"
#include "SimException.h"
#include "ProjectedOutputPortIterator.h"

ProjectedEventSender::~ProjectedEventSender() {
	// NOOP
}

void  ProjectedEventSender::changeProjectedEventTime(SimContext &ai, port_id_t port_id,Time new_time)
{
	global_proj_out_port_id_t global_port_id = getProjOutPortGlobalID(port_id);
	ai.projected_ev_heap_buf->update(global_port_id, new_time);
}


SingleOutputProjectedEventSender::~SingleOutputProjectedEventSender() {
	// NOOP
}


global_proj_out_port_id_t SingleOutputProjectedEventSender::getProjOutPortGlobalID(port_id_t port_id) const
{
	return first_proj_port_global_id;
}


void SingleOutputProjectedEventSender::setFirstProjOutPortGlobalID(global_proj_out_port_id_t port_id)
{
	first_proj_port_global_id = port_id;
}


ProjectedOutputPortIterator SingleOutputProjectedEventSender::getProjectedPortIterator()
{
	return ProjectedOutputPortIterator();
}


pair<port_id_t, global_proj_out_port_id_t> SingleOutputProjectedEventSender::getFirstProjectedPortInfo()
{
	return make_pair(0, first_proj_port_global_id);
}


void MultiOutputProjectedEventSender::setFirstProjOutPortGlobalID(global_proj_out_port_id_t port_id)
{
	// temporary store the first projected output port id in the map (to save space)
	proj_out_ports_global_ids.insert(make_pair(port_id, 0));
}


void MultiOutputProjectedEventSender::setupProjectedOutPorts()
{
	EvSimObject *self = dynamic_cast<EvSimObject *>(this);
	if (proj_out_ports_global_ids.size() != 1)
		throw new evesim::ConstructionException("MultiOutputProjectedEventSender::setupProjOutPorts()",
				                    "The proj_out_ports_global_ids map does not hold a single element. Probably "
				                    "there was some error in the order of construction statements by the user.");

	global_proj_out_port_id_t first_proj_port_id = proj_out_ports_global_ids.begin()->first;
	size_t num_out_ports = self->numOutputPorts();
	size_t proj_port_counter = 0;
	proj_out_ports_global_ids.clear();
	for (size_t p = 0; p < num_out_ports; ++p)
	{
		if (self->getOutputPortType(p) == EvSimObject::eventPortProjected) {
			proj_out_ports_global_ids.insert(make_pair(p, first_proj_port_id + proj_port_counter));
			proj_port_counter++;
		}
	}
}

global_proj_out_port_id_t MultiOutputProjectedEventSender::getProjOutPortGlobalID(port_id_t port_id) const
{
	ProjOutPortsGlobalIDMap::const_iterator it = proj_out_ports_global_ids.find(port_id);

	if (it == proj_out_ports_global_ids.end())
		return PORT_NOT_DEFINED;
	return it->second;
}

ProjectedOutputPortIterator MultiOutputProjectedEventSender::getProjectedPortIterator()
{
		return ProjectedOutputPortIterator(*this);
}

pair<port_id_t, global_proj_out_port_id_t> MultiOutputProjectedEventSender::getFirstProjectedPortInfo()
{
	return *proj_out_ports_global_ids.begin();
}


