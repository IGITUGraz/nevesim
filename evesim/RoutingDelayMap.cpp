#include "RoutingDelayMap.h"
#include "DirectEventSender.h"

event_target_group_id_t RoutingDelayMap::find(global_out_port_id_t senderid, delay_t delay)
{
    if( senderid >= _map.size() ) return no_event_target_group;

    delaymaptype::const_iterator find_iter ;
    if (  (find_iter = _map[senderid].find(delay)) != _map[senderid].end() ) {
        return find_iter->second ;
    }

    return no_event_target_group;
}


void RoutingDelayMap::registerOutPorts(EvSimObject *obj) {
	DirectEventSender *sender = dynamic_cast<DirectEventSender *>(obj);
	if (sender != 0) {
		sender->setFirstOutPortGlobalID(getTotalNumOutPorts());
		increaseTotalNumOutPorts(obj->numOutputPorts());
	}
 }
