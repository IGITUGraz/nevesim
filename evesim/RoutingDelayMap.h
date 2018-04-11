#ifndef ROUTINGDELAYMAP_H_
#define ROUTINGDELAYMAP_H_

#include <vector>
#include <map>
#include "globals.h"
#include "EvSimObject.h"

using std::map;
using std::vector;

typedef double delay_t;

//!  Spike routing table for scheduling of spikes within one engine.
/*!
 *   For each spike sending object identified by id,
 *   the delay map stores many (delay, spike_target_group) pairs.
 * 
 *   For a spiking connection with delay D between object A and object B which 
 *   are in the same engine, an entry is added in the the map [A]->(D, STG)
 *   where the spike target group contains B. If there is already an entry [A]->(D, X)
 *   then B is added to X spike target group.
 * 
 *
 */
class RoutingDelayMap
{


public:
    typedef map<delay_t, event_target_group_id_t> delaymaptype;

    // Iterator type for iterating through (delay, etg) pairs
    typedef delaymaptype::const_iterator const_iterator;

    RoutingDelayMap() : totalNumOutPorts(0) {};

    virtual ~RoutingDelayMap() {};

    //! Inserts new entry in the map [out_port_id]->(delay,eventtargetgroup)
    inline void insert(global_out_port_id_t port_id, delay_t delay, event_target_group_id_t group)
    {
        resize( port_id );
        _map[port_id].insert( delaymaptype::value_type(delay, group) );
    }

    //! Checks if there is an entry for the specified sending object and the delay.
    /*!
     *  \return the ETG id if there is an entry, and <i> no_eventgroup </i> otherwise.
     */
    event_target_group_id_t find(global_out_port_id_t port_id, delay_t delay);

    //! Returns a begin iterator which can serve to iterate through all (delay, stg) pairs.
    inline const_iterator begin(global_out_port_id_t out_port_id)
    {
        resize( out_port_id );
        return _map[out_port_id].begin();
    };

    //! Returns an end iterator which can serve to iterate through all (delay, stg) pairs.
    inline const_iterator end(global_out_port_id_t out_port_id)
    {
        resize( out_port_id );
        return _map[out_port_id].end();
    };

    inline void increaseTotalNumOutPorts(unsigned num_ports)
    {
    	totalNumOutPorts += num_ports;
    }

    inline unsigned getTotalNumOutPorts()
    {
    	return totalNumOutPorts;
    }

    void registerOutPorts(EvSimObject *obj);

protected :

    inline void resize( global_out_port_id_t out_port_id )
    {
        if (out_port_id >= _map.size()) {
            _map.resize(out_port_id+1);
        }
    };

    // data structure used for holding the mapping: [senderid]->(delay, etg)
    vector< delaymaptype > _map;

    unsigned totalNumOutPorts;
};

#endif /*ROUTINGDELAYMAP_H_*/
