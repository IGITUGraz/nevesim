#ifndef PROJECTEDOUTPUTPORTITERATOR_H_
#define PROJECTEDOUTPUTPORTITERATOR_H_

#include "ProjectedEventSender.h"

#include <utility>
using std::pair;
using std::make_pair;


class ProjectedOutputPortIterator {
public:

	ProjectedOutputPortIterator(MultiOutputProjectedEventSender &sender);

	ProjectedOutputPortIterator() : port_map(0) {};

	virtual ~ProjectedOutputPortIterator();

	bool hasNextPort() {
		return it != end_it;
	}

	pair<port_id_t, global_proj_out_port_id_t> nextPort()
	{
		return *(it++);
	}

protected:

	MultiOutputProjectedEventSender::ProjOutPortsGlobalIDMap * port_map;
	MultiOutputProjectedEventSender::ProjOutPortsGlobalIDMap::const_iterator it;
	MultiOutputProjectedEventSender::ProjOutPortsGlobalIDMap::const_iterator end_it;

};



#endif /* PROJECTEDOUTPUTPORTITERATOR_H_ */
