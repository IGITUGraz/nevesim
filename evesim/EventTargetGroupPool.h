#ifndef EventTargetGroupPOOL_H_
#define EventTargetGroupPOOL_H_

#include "EvSimObject.h"

#include <cstring>

#include <list>
using std::list;

#include <vector>
using std::vector;

#include <utility>
using std::pair;


class EventTargetGroupPool
{
public:

    struct EventTarget
    {
        EventTarget( ): receiver(NULL), port(0) {};
        EventTarget(EvSimObject * receiver, port_id_t p): receiver(receiver), port(p) {};

        EvSimObject * receiver;
        port_id_t port;

    };

    typedef list<EventTarget> EventTargetContainer;
    typedef EventTargetContainer::const_iterator const_iterator ;

    EventTargetGroupPool();

    virtual ~EventTargetGroupPool();

    event_target_group_id_t createEventTargetGroup( EvSimObject* receiver, port_id_t port );

    event_target_group_id_t addEventTarget( event_target_group_id_t group, EvSimObject* receiver, port_id_t port );

    inline const_iterator begin(event_target_group_id_t group)
    {
        return pool[group].begin();
    }

    inline const_iterator end(event_target_group_id_t group)
    {
        return pool[group].end();
    }

    inline size_t size(event_target_group_id_t group)
    {
    	return pool[group].size();
    }

    unsigned numETGroups()
    {
    	return pool.size();
    };


protected:
    vector<EventTargetContainer> pool;

};

#endif /* SPIKERECEIVINGGROUPS_H_ */
