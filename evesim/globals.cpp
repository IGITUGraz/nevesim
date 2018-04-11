#include "globals.h"

#include <limits>

const event_target_group_id_t no_event_target_group = ~((event_target_group_id_t)0);

// Value indicating an invalid (not assignd) spike port id
const port_id_t INVALID_PORT_ID = ~((port_id_t)0);
const port_id_t PORT_NOT_DEFINED = ~((port_id_t)0);

const Time MAX_TIME = std::numeric_limits<Time>::max();
const Time INVALID_TIME = std::numeric_limits<Time>::max();
