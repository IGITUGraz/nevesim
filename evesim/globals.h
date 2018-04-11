#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <boost/shared_ptr.hpp>

#include <vector>
using std::vector;

#define EVENT_BUFFER_CHUNK_SIZE 1024

//! The size of one block of the SimObject::BlockAllocator
#define EVSIMOBJECT_ALLOCATOR_BLOCK_SIZE 0x20000

typedef double Time;

typedef boost::shared_ptr<vector<Time> > EventTimesVector;

extern const Time MAX_TIME;
extern const Time INVALID_TIME;

typedef unsigned objectid_t;

typedef unsigned port_id_t;

typedef int update_id_t;

typedef unsigned global_out_port_id_t;

typedef unsigned global_proj_out_port_id_t;

typedef unsigned event_target_group_id_t;

extern const event_target_group_id_t no_event_target_group;

extern const port_id_t INVALID_PORT_ID;
extern const port_id_t PORT_NOT_DEFINED;



#endif /* GLOBALS_H_ */
