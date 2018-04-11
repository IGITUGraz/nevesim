#ifndef PROPAGATEDUPDATELIST_H_
#define PROPAGATEDUPDATELIST_H_

#include "globals.h"

#include "EvSimObjectPool.h"
#include "EventTargetGroupPool.h"
#include "UpdateLinksMap.h"
#include "EvSimObject.h"

typedef vector<update_id_t> UpdateIDVector;

struct UpdateTarget
{
	UpdateTarget(EvSimObject *object,  UpdateIDVector * update_id_vec)
	    : object(object),  update_id_vec(update_id_vec) {};

	UpdateTarget(EvSimObject *object, bool is_single_update_id, update_id_t update_id, UpdateIDVector *update_id_vec)
		: object(object), is_single_update_id(is_single_update_id)
	{
		if (is_single_update_id)
			this->update_id = update_id;
		else
			this->update_id_vec = update_id_vec;
	}

	~UpdateTarget() {}

	EvSimObject * object;
	bool is_single_update_id;
	union {
		UpdateIDVector* update_id_vec;
		update_id_t update_id;
	};
};


class PropagatedUpdateList {
public:
	typedef vector<UpdateTarget>::const_iterator const_iterator;

	PropagatedUpdateList(EventTargetGroupPool & etgpool, UpdateLinksMap & upd_links_map, EvSimObjectPool & obj_pool)
	: etgpool(etgpool), upd_links_map(upd_links_map), obj_pool(obj_pool)
	{}


	~PropagatedUpdateList();

	void prepare();

	const_iterator begin(event_target_group_id_t etg_id) {
		return update_list[etg_id].begin();
	}

	const_iterator end(event_target_group_id_t etg_id) {
		return update_list[etg_id].end();
	}


protected:

	void createUpdateListForETG(event_target_group_id_t etg_id);

	void printUpdateLinksMap();
	void printUpdateList();


	EventTargetGroupPool & etgpool;
	UpdateLinksMap & upd_links_map;
	EvSimObjectPool & obj_pool;

	vector< vector< UpdateTarget > > update_list;
};

#endif /* PROPAGATEDUPDATELIST_H_ */
