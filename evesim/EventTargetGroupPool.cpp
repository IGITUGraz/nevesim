#include "EventTargetGroupPool.h"

EventTargetGroupPool::EventTargetGroupPool()
{}

EventTargetGroupPool::~EventTargetGroupPool()
{}

event_target_group_id_t EventTargetGroupPool::addEventTarget( event_target_group_id_t group, EvSimObject* receiver, port_id_t port )
{
    if ( group >= (event_target_group_id_t)pool.size() ) {
        pool.resize( group + 1 );
    }
    pool[ group ].push_back( EventTarget(receiver,port) );
    return group;
}

event_target_group_id_t EventTargetGroupPool::createEventTargetGroup( EvSimObject* receiver, port_id_t port )
{
    event_target_group_id_t group = pool.size();
    pool.resize( group + 1 );
    pool[ group ].push_back( EventTarget(receiver,port) );
    return group;
}
