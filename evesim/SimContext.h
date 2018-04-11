#ifndef SimContext_H_
#define SimContext_H_

#include <cstddef>

#include "globals.h"

class EvSimNetwork;
class EvSimEngine;
class ProjectedEventsHeapBuffer;

class SimContext {
    public:

	   SimContext( Time t = 0.0 ) :
        t( t ), net( 0 ), eng( 0 ), projected_ev_heap_buf( 0 ) {}
        ;

        SimContext( EvSimNetwork *net, EvSimEngine *eng, ProjectedEventsHeapBuffer *heap_buf ) :
        	t( 0 ), net( net ), eng( eng ), projected_ev_heap_buf( heap_buf ) {} ;

        Time t;
        EvSimNetwork *net;
        EvSimEngine *eng;
        ProjectedEventsHeapBuffer *projected_ev_heap_buf;
};

typedef SimContext InitializeContext;


#endif /*SimContext_H_*/
