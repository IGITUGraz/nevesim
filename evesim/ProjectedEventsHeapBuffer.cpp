#include "ProjectedEventsHeapBuffer.h"

ProjectedEventsHeapBuffer::ProjectedEventsHeapBuffer() {
	// TODO Auto-generated constructor stub

}

ProjectedEventsHeapBuffer::~ProjectedEventsHeapBuffer() {
	// TODO Auto-generated destructor stub
}

void ProjectedEventsHeapBuffer::setGlobalOutputPort(
		global_proj_out_port_id_t proj_port_id,
		global_out_port_id_t global_out_port_id) {
	projected_port_vec[proj_port_id].global_port_id = global_out_port_id;
}


