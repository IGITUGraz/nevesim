#include "ProjectedOutputPortIterator.h"
#include "ProjectedEventSender.h"


ProjectedOutputPortIterator::~ProjectedOutputPortIterator() {
	// TODO Auto-generated destructor stub
}


ProjectedOutputPortIterator::ProjectedOutputPortIterator(MultiOutputProjectedEventSender &sender)
   : port_map(&sender.proj_out_ports_global_ids)
{
	it = port_map->begin();
	end_it = port_map->end();
	MultiOutputProjectedEventSender::ProjOutPortsGlobalIDMap::const_iterator it2 = it;
}

