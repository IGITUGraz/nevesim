#ifndef UPDATELINKSMAP_H_
#define UPDATELINKSMAP_H_

#include "EvSimObject.h"

#include <vector>
using std::vector;

#include <map>
#include <utility>

struct UpdateNode
{
	UpdateNode(EvSimObject* object, update_id_t updateid) : object(object), updateid(updateid) {};
	EvSimObject *object;
	update_id_t updateid;
};

typedef vector<UpdateNode> AdjacentUpdateNodesVector;

typedef std::map<EvSimObject *, AdjacentUpdateNodesVector > UpdateLinksMap;



#endif /* UPDATELINKSMAP_H_ */
