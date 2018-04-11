#ifndef ScheduledEventsQueue_H_
#define ScheduledEventsQueue_H_

#include "globals.h"

#include <iostream>
using std::cout;
using std::endl;

#include <list>
using std::list;

#include <vector>
using std::vector;

#include <queue>
using std::priority_queue;

#include <assert.h>


//! A storage of scheduled Events to be delivered at a later point in the simulation
class ScheduledEventsQueue
{

public:

    //! Constructor of a queue of scheduled events
    /* \param chunkSize Constant used for internal memory management.
     *                   Should increase with increasing
     *                   expected number of spikes.
     */
    ScheduledEventsQueue(int chunkSize = EVENT_BUFFER_CHUNK_SIZE );

    //! Destructor: Deletes all scheduled spikes
    virtual ~ScheduledEventsQueue();

    //! Schedule a group of event targets to get an event at time t
    /*! \param etg  The identifier of the event target group to be used as
     *              identifier in EventTargetGroupPool::beginEventTarget( Etg )
     *
     *   \param t   The time stamp of the event
     */
    void scheduleEvent(event_target_group_id_t etg, Time t);

    //! Structure which stores the index of the spike target group and a pointer to the next element in various lists
	struct Node
	{
		event_target_group_id_t etg;
		Time time;
		Node *next;
	};

	class NodeCompare {
	public:
		bool operator()(Node const * left, Node const * right) const {
			return left->time > right->time;
		}
	};

    inline Node * top();

    Time topTime();

    inline void pop();


    //! Must be called at the begin (reset) of a simulation
    void reset();


    //! returns true if the queue is empty.
    bool empty() const {
    	return event_queue.empty();
    }

protected:

    //! Set up internal memory management
    void init();

    priority_queue<Node*, vector<Node*>, NodeCompare > event_queue;

    //! Get a new Node from the internal memory management
    Node *getFreeNode(void);

    //! Buffer with pointers to chunks of memory allocated
    vector< Node* > chunkBuffer;

    //! Pointer to array of Nodes to take from (from position nextFreeNodeIdx)
    Node *currentFreeChunk;

    //! Index into array currentFreeChunk which specifies the next Node to use
    int  nextFreeNodeIdx;

    //! Index into chunkBuffer which specifies the next free chunk of memory to use
    size_t nextFreeChunkIdx;

    //! Head of list of Nodes which can be reused
    Node* recycledNodes;

    //! Number of Nodes per chunk of memory to allocate
    int chunkSize;

    double fillitup[32];
};

inline ScheduledEventsQueue::Node * ScheduledEventsQueue::top()
{
	return event_queue.top();
}

inline Time ScheduledEventsQueue::topTime()
{
	return event_queue.top()->time;
}


inline void ScheduledEventsQueue::pop()
{
	Node *n = event_queue.top();
	n->next = recycledNodes;
	recycledNodes = n;
	event_queue.pop();
}

inline void ScheduledEventsQueue::scheduleEvent(event_target_group_id_t etg, Time t)
{
	Node *n = getFreeNode();
	n->etg = etg;
	n->time = t;
	event_queue.push(n);
}



#endif /*ScheduledEventsQueue_H_*/
