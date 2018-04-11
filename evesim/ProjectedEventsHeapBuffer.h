#ifndef PROJECTEDEVENTSHEAPBUFFER_H_
#define PROJECTEDEVENTSHEAPBUFFER_H_

#include "globals.h"
#include "EvSimObject.h"

#include <vector>
using std::vector;

#include <utility>

#include "binaryheap.h"


class ProjectedEventsHeapBuffer {
public:

	ProjectedEventsHeapBuffer();
	virtual ~ProjectedEventsHeapBuffer();

	static const int invalid_heap_pos = -1;


	size_t registerProjectedOutPorts(EvSimObject* sender);


	void setGlobalOutputPort(global_proj_out_port_id_t proj_port_id, global_out_port_id_t global_out_port_id);

	void makeHeap();

	void update(global_proj_out_port_id_t port, Time newtime);

	void updateTop(Time newtime);

	std::pair<EvSimObject *, global_out_port_id_t> topSender();

	Time topTime();

	bool empty() const {
		return projected_port_vec.empty();
	}

	size_t size() {
		return projected_port_vec.size();
	}

protected:

	struct Node {
			Node(EvSimObject * sender_obj) :
				sender_obj(sender_obj), time(MAX_TIME), heap_pos(invalid_heap_pos) {};
			EvSimObject* sender_obj;
			global_out_port_id_t global_port_id;
			Time time;
			int heap_pos;
		};


	class NodeCompare {
	public:
		bool operator()(Node const * left, Node const * right) const {
			return left->time > right->time;
		}
	};

	class HeapPosUpdater {
	public:
		void operator()(Node *n, unsigned pos) {
			n->heap_pos = pos;
		}
	};

	vector<Node *> heap_vec;

	vector<Node> projected_port_vec;

};


inline size_t ProjectedEventsHeapBuffer::registerProjectedOutPorts(EvSimObject * sender)
{
	size_t prev_size = projected_port_vec.size();
	size_t num_ports = sender->numProjOutputPorts();
	projected_port_vec.resize(prev_size + num_ports, Node(sender));
	return prev_size;
}

inline void ProjectedEventsHeapBuffer::makeHeap()
{
	vector<Node>::iterator proj_vec_it = projected_port_vec.begin();
	heap_vec.reserve(projected_port_vec.size());
	for ( ; proj_vec_it != projected_port_vec.end() ; ++proj_vec_it) {
		proj_vec_it->heap_pos = heap_vec.size();
		heap_vec.push_back(&(*proj_vec_it));
	}
	std::make_heap(heap_vec.begin(), heap_vec.end(), NodeCompare(), HeapPosUpdater());
}

inline void ProjectedEventsHeapBuffer::update(global_proj_out_port_id_t port_id, Time newtime)
{
	projected_port_vec[port_id].time= newtime;
	std::update_heap_pos(heap_vec.begin(), heap_vec.end(),
			             heap_vec.begin() + projected_port_vec[port_id].heap_pos, NodeCompare(), HeapPosUpdater());
}

inline void ProjectedEventsHeapBuffer::updateTop(Time newtime) {
	global_proj_out_port_id_t port_id = heap_vec[0] - &projected_port_vec[0];
	update(port_id, newtime);
}

inline std::pair<EvSimObject *, global_out_port_id_t> ProjectedEventsHeapBuffer::topSender() {
    return std::make_pair(heap_vec[0]->sender_obj, heap_vec[0]->global_port_id);
}


inline Time ProjectedEventsHeapBuffer::topTime() {
	return heap_vec[0]->time;
}


#endif /* PROJECTEDEVENTSHEAPBUFFER_H_ */
