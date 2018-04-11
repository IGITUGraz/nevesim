#include "PropagatedUpdateList.h"

#include  "evesim_hash.h"
#include <utility>
using std::make_pair;
#include <boost/shared_ptr.hpp>
#include "SimException.h"

#include <iostream>
using std::cerr;
using std::endl;

struct DegreeAndUpdateIDsPair {
	DegreeAndUpdateIDsPair(int degree = 0, update_id_t update_id  = 0, bool propagates_update = true)
	              : degree(degree),
					 propagates_update(propagates_update),
					 is_single_update_id(true), update_id(update_id)
	{}

	int degree;
	bool propagates_update;
	bool is_single_update_id;
	union {
		UpdateIDVector *update_id_vec;
		update_id_t update_id;
	};
};

PropagatedUpdateList::~PropagatedUpdateList()
{
	vector< vector< UpdateTarget > >::iterator upd_list_it = update_list.begin();

	for (; upd_list_it != update_list.end(); ++upd_list_it ) {
		vector< UpdateTarget >::iterator upd_target_it = upd_list_it->begin();
		for (; upd_target_it != upd_list_it->end(); ++upd_target_it) {
			if (!upd_target_it->is_single_update_id) {
				delete upd_target_it->update_id_vec;
			}
		}
	}
}


void PropagatedUpdateList::createUpdateListForETG(event_target_group_id_t etg_id)
{
	typedef unordered_map<EvSimObject *, DegreeAndUpdateIDsPair, evesim::hash<EvSimObject *> > DegreeMapType;
	typedef pair<DegreeMapType::iterator, bool> DegreeMapResult;
	DegreeMapType degree_map;
	EventTargetGroupPool::const_iterator oid_it;

	list<EvSimObject*> vert_list;

	for (oid_it = etgpool.begin(etg_id); oid_it != etgpool.end(etg_id); ++oid_it) {
		vert_list.push_back(oid_it->receiver);
		degree_map.insert(make_pair(oid_it->receiver,DegreeAndUpdateIDsPair(-1)));
	}

	list<EvSimObject*>::iterator vert_it = vert_list.begin();
	while (vert_it != vert_list.end()) {
		AdjacentUpdateNodesVector::const_iterator adj_vert_it;
		UpdateLinksMap::const_iterator vert_map_it;
		if ((vert_map_it = upd_links_map.find(*vert_it)) != upd_links_map.end())
			for (adj_vert_it = vert_map_it->second.begin();
					adj_vert_it != vert_map_it->second.end() ; ++adj_vert_it) {
				bool propagate_new_update = adj_vert_it->object->propagatesUpdate(adj_vert_it->updateid);
				DegreeMapResult res = degree_map.insert(make_pair(adj_vert_it->object,
						            DegreeAndUpdateIDsPair(0, adj_vert_it->updateid, propagate_new_update)));
				DegreeAndUpdateIDsPair &v = res.first->second;

				bool changed_upd_propagation = false;
				if (!res.second && propagate_new_update && !v.propagates_update) {
					v.propagates_update = true;
					v.update_id = adj_vert_it->updateid;
					changed_upd_propagation = true;
				}

				if (v.degree >= 0) {
					v.degree++;
					/*
					if (!res.second) {
						if (v.is_single_update_id) {
							update_id_t first_update_id = v.update_id;
							v.is_single_update_id = false;
							v.update_id_vec = new UpdateIDVector();
							v.update_id_vec->push_back(first_update_id);
						}
						v.update_id_vec->push_back(adj_vert_it->updateid);
					}*/
				}
				if ( (res.second && propagate_new_update) || changed_upd_propagation ) {
					vert_list.push_back(adj_vert_it->object);
				}
			}
		++vert_it;
		vert_list.pop_front();
	}

	// topological sorting
	vert_list.clear();
	for (oid_it = etgpool.begin(etg_id); oid_it != etgpool.end(etg_id); ++oid_it) {
		vert_list.push_back(oid_it->receiver);
	}

	update_list[etg_id].reserve(degree_map.size() - etgpool.size(etg_id));
	vector<UpdateTarget> &update_list_specific = update_list[etg_id];

	vert_it = vert_list.begin();
	while (vert_it != vert_list.end()) {
		AdjacentUpdateNodesVector::const_iterator adj_vert_it;
		UpdateLinksMap::const_iterator vert_map_it;
		if ((vert_map_it = upd_links_map.find(*vert_it)) != upd_links_map.end())
			for (adj_vert_it = vert_map_it->second.begin();
					adj_vert_it != vert_map_it->second.end(); ++adj_vert_it) {
				if (degree_map.count(adj_vert_it->object)) {
					DegreeMapType::mapped_type & v = degree_map[adj_vert_it->object];
					if (v.degree > 0)
						v.degree --;
					if (v.degree == 0) {
						update_list_specific.push_back(UpdateTarget(adj_vert_it->object,
																		v.is_single_update_id,
																		v.update_id,
																		v.update_id_vec));
						vert_list.push_back(adj_vert_it->object);
					}
				}
			}
		++vert_it;
		vert_list.pop_front();
	}

	if (update_list_specific.size() < (degree_map.size() - etgpool.size(etg_id))) {
		throw evesim::ConstructionException("PropagatedUpdateList::prepare",
				                            "Detected a directed circle in the graph of causal update connections.");
	}
}

void PropagatedUpdateList::prepare()
{
    //	printUpdateLinksMap();

	event_target_group_id_t etg_id;
	unsigned numETGs = etgpool.numETGroups();
	update_list.resize(numETGs);

	for (etg_id = 0 ; etg_id < numETGs ; ++ etg_id) {
		createUpdateListForETG(etg_id);
	}

	// printUpdateList();
}

void PropagatedUpdateList::printUpdateLinksMap()
{
	UpdateLinksMap::const_iterator it;
	 for ( it=upd_links_map.begin() ; it != upd_links_map.end(); it++ ) {
	    cerr << (*it).first->getTypeName() << ":" <<(*it).first << ":";
	    AdjacentUpdateNodesVector::const_iterator vec_it;
	    for ( vec_it=it->second.begin() ; vec_it != it->second.end(); vec_it++ ) {
	    	cerr << " (" << vec_it->object->getTypeName() << ":" << vec_it->object << " , " << vec_it->updateid << " ) , " << endl;
	    }
	 }
}

void PropagatedUpdateList::printUpdateList()
{
	for (unsigned i = 0;  i < update_list.size(); ++i) {
		cerr << "********* ETG:" << i << ": ";
		EventTargetGroupPool::const_iterator oid_it;
		for (oid_it = etgpool.begin(i); oid_it != etgpool.end(i); ++oid_it) {
			cerr << oid_it->receiver->getTypeName() << ":"<< oid_it->receiver << ", ";
		}
		cerr << endl;
		for (unsigned j = 0; j < update_list[i].size() ; ++j ) {
			cerr << "obj : " << update_list[i][j].object->getTypeName() << " " << update_list[i][j].object << ":";
			if (update_list[i][j].is_single_update_id) {
				cerr << update_list[i][j].update_id;
			}
			else {
				for (unsigned k = 0; k < update_list[i][j].update_id_vec->size(); ++k ) {
					cerr << update_list[i][j].update_id_vec->at(k) << ", ";
				}
			}
			cerr << endl;
		}
	}
}
