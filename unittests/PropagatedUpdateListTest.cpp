#include "PropagatedUpdateListTest.h"
#include "TestEvSimObject.h"
#include "RandomDistribution.h"
#include "SimException.h"

#include <cmath>

#include <iostream>
using std::cerr;
using std::endl;

#include <algorithm>
#include <numeric>

#include <vector>
using std::vector;

#include <list>
using std::list;

#include <set>

// choose one from the following
SIM_LOCAL_AUTOBUILD_TEST_SUITE( PropagatedUpdateListTest )
// SIM_LOCAL_NIGHTLY_TEST_SUITE( PropagatedUpdateListTest )

void PropagatedUpdateListTest::setUp()
{
}

void PropagatedUpdateListTest::tearDown()
{
}

void PropagatedUpdateListTest::test2neurons()
{
	EventTargetGroupPool etg_pool;
	EvSimObjectPool obj_pool;
	UpdateLinksMap update_links_map;

	TestEvSimObject obj_factory;

	EvSimObject::ID nrn1_id = obj_pool.createObject(obj_factory);
	EvSimObject *nrn1 = obj_pool.getObject(nrn1_id);
	EvSimObject::ID nrn2_id = obj_pool.createObject(obj_factory);
	EvSimObject *nrn2 = obj_pool.getObject(nrn2_id);

	EvSimObject::ID psp1_id = obj_pool.createObject(obj_factory);
	EvSimObject *psp1 = obj_pool.getObject(psp1_id);

	EvSimObject::ID psp2_id = obj_pool.createObject(obj_factory);
	EvSimObject *psp2 = obj_pool.getObject(psp2_id);


	EvSimObject::ID syn1_id = obj_pool.createObject(obj_factory);
	EvSimObject *syn1 = obj_pool.getObject(syn1_id);
	EvSimObject::ID syn2_id = obj_pool.createObject(obj_factory);
	EvSimObject *syn2 = obj_pool.getObject(syn2_id);

	event_target_group_id_t etg1_id = etg_pool.createEventTargetGroup(psp1, port_id_t(0));
	event_target_group_id_t etg2_id = etg_pool.createEventTargetGroup(psp2, port_id_t(0));

	update_links_map[psp1].push_back(UpdateNode(syn1, 0));
	update_links_map[psp2].push_back(UpdateNode(syn1, 1));

	update_links_map[syn1].push_back(UpdateNode(nrn1, 0));

	update_links_map[psp2].push_back(UpdateNode(syn2, 0));
	update_links_map[psp1].push_back(UpdateNode(syn2, 1));

	update_links_map[syn2].push_back(UpdateNode(nrn2, 0));

	PropagatedUpdateList update_list(etg_pool, update_links_map, obj_pool);

	update_list.prepare();

	enum {i_syn1 = 0, i_syn2 = 1, i_nrn1 = 2, i_nrn2 = 3 } ;
	EvSimObject *obj_ptrs[] = { syn1, syn2, nrn1, nrn2 };

	int visited[] = {-1, -1, -1, -1};

	event_target_group_id_t etg_id_arr[] = {etg1_id, etg2_id};

	for (event_target_group_id_t * etg_id_it = etg_id_arr;
						etg_id_it != (etg_id_arr + sizeof(etg_id_arr)/sizeof(event_target_group_id_t)); ++etg_id_it) {
		PropagatedUpdateList::const_iterator upd_obj_it = update_list.begin(*etg_id_it);
		unsigned len = 0;
		for (; upd_obj_it != update_list.end(*etg_id_it); ++upd_obj_it) {
			EvSimObject * *found_obj = std::find(obj_ptrs, obj_ptrs+4, upd_obj_it->object);
			visited[found_obj - obj_ptrs] = len;
			CPPUNIT_ASSERT( upd_obj_it->is_single_update_id );
			update_id_t expected_update_id = 0;
			if ( ( *found_obj == obj_ptrs[i_syn2] && *etg_id_it == etg1_id ) ||
				       ( *found_obj == obj_ptrs[i_syn1] && *etg_id_it == etg2_id ) )
				expected_update_id = 1;
			CPPUNIT_ASSERT_EQUAL( expected_update_id, upd_obj_it->update_id );
			len++;
		}
		CPPUNIT_ASSERT_EQUAL(4u, len);
		CPPUNIT_ASSERT( visited[i_syn1] < visited[i_nrn1] );
		CPPUNIT_ASSERT( visited[i_syn2] < visited[i_nrn2] );
		int * found_not_visited = std::find(visited, visited + 4, -1);
		CPPUNIT_ASSERT_EQUAL( visited + 4, found_not_visited);
	}
}


void PropagatedUpdateListTest::testManySynapsesToOneNeuron()
{
	EventTargetGroupPool etg_pool;
	EvSimObjectPool obj_pool;
	UpdateLinksMap update_links_map;

	TestEvSimObject obj_factory;

	const unsigned num_syn = 10;

	EvSimObject::ID syn_ids[num_syn];
	EvSimObject* syns[num_syn];

	for (unsigned i = 0 ; i < num_syn; ++i) {
		syn_ids[i] = obj_pool.createObject(obj_factory);
		syns[i] = obj_pool.getObject(syn_ids[i]);
	}

	EvSimObject::ID src_nrn_id = obj_pool.createObject(obj_factory);
	EvSimObject *src_nrn = obj_pool.getObject(src_nrn_id);

	EvSimObject::ID dest_nrn_id = obj_pool.createObject(obj_factory);
	EvSimObject *dest_nrn = obj_pool.getObject(dest_nrn_id);

	event_target_group_id_t etg1_id = etg_pool.createEventTargetGroup(src_nrn, port_id_t(0));

	for (unsigned i = 0 ; i < num_syn; ++i) {
		update_links_map[src_nrn].push_back(UpdateNode(syns[i], i));
		update_links_map[syns[i]].push_back(UpdateNode(dest_nrn, i));
	}
	PropagatedUpdateList update_list(etg_pool, update_links_map, obj_pool);

	update_list.prepare();

	const unsigned i_dest_nrn = num_syn + 1;

	EvSimObject *obj_ptrs[num_syn+2];

	obj_ptrs[0] = obj_pool.getObject(src_nrn_id);
	obj_ptrs[num_syn+1] = obj_pool.getObject(dest_nrn_id);

	for (unsigned i = 0 ;i < num_syn ; ++i)
		obj_ptrs[i+1] = obj_pool.getObject(syn_ids[i]);


	vector<update_id_t> expected_update_id_vec;
	expected_update_id_vec.resize(num_syn);
	for (unsigned i = 0 ; i < num_syn ; ++i)
		expected_update_id_vec[i] = i;

	unsigned idx = 0;
	PropagatedUpdateList::const_iterator upd_obj_it = update_list.begin(etg1_id);
	for (; upd_obj_it != update_list.end(etg1_id); ++upd_obj_it) {
		EvSimObject * *found_obj = std::find(obj_ptrs, obj_ptrs+num_syn+2, upd_obj_it->object);
		if (*found_obj == obj_ptrs[i_dest_nrn]) {
			CPPUNIT_ASSERT_EQUAL( num_syn, idx);
			CPPUNIT_ASSERT( upd_obj_it->is_single_update_id );
			CPPUNIT_ASSERT_EQUAL( 0u, unsigned(upd_obj_it->update_id) );
		}
		else {
			CPPUNIT_ASSERT( upd_obj_it->is_single_update_id );
			CPPUNIT_ASSERT_EQUAL( update_id_t(found_obj - obj_ptrs - 1), upd_obj_it->update_id );
		}
		idx++;
	}
	CPPUNIT_ASSERT_EQUAL(num_syn + 1, idx);
}


void PropagatedUpdateListTest::initUpdateBinaryMap(vector<vector<bool> > &upd_bin_map, unsigned nRows, unsigned nCols)
{
	upd_bin_map.resize(nRows);
	vector<vector<bool> >::iterator it = upd_bin_map.begin();
	for (;it != upd_bin_map.end(); ++it) {
		it->resize(nCols);
	}
}


void PropagatedUpdateListTest::testRandom()
{
	EventTargetGroupPool etg_pool;
	EvSimObjectPool obj_pool;
	UpdateLinksMap update_links_map;

	MersenneTwister19937 rnd_eng;
	rnd_eng.seed(65304934);

	const unsigned num_obj = 20;

	TestEvSimObject obj_factory;

	EvSimObject *obj_ptrs[num_obj];
	EvSimObject::ID obj_ids[num_obj];

	for (unsigned i = 0 ; i < num_obj; ++i) {
		obj_ids[i] = obj_pool.createObject(obj_factory);
		obj_ptrs[i] = obj_pool.getObject(obj_ids[i]);
	}

	const unsigned num_etgs = 5;

	vector<vector<EvSimObject::ID> > etg_list;
	etg_list.resize(num_etgs);
	vector<event_target_group_id_t> etg_ids;

	UniformIntegerDistribution rnd_obj_dist(0, num_obj-1);
	UniformIntegerDistribution rnd_etg_size_dist(1, 5);
	for (unsigned etg_i = 0; etg_i < num_etgs; ++etg_i) {
		unsigned etg_size = unsigned(rnd_etg_size_dist.get(rnd_eng));
		unsigned obj_i = unsigned(rnd_obj_dist.get(rnd_eng));
		etg_ids.push_back(etg_pool.createEventTargetGroup(obj_ptrs[obj_i], port_id_t(0)));
		for (unsigned j = 1 ; j < etg_size ; ++j ) {
			unsigned obj_i = unsigned(rnd_obj_dist.get(rnd_eng));
			etg_pool.addEventTarget(etg_ids.back(), obj_ptrs[obj_i], port_id_t(0));
		}
	}

	unsigned num_connections = num_obj * num_obj / 5;

	const unsigned upd_id_range = 5;

	UniformIntegerDistribution src_obj_dist(0, num_obj-2);
	UniformIntegerDistribution rnd_upd_id_dist(0, upd_id_range);

	for (unsigned i = 0 ; i < num_connections; ++i) {
		unsigned src_i = unsigned(src_obj_dist.get(rnd_eng));
		UniformIntegerDistribution dest_obj_dist(src_i + 1, num_obj-1);
		unsigned dest_i = unsigned(dest_obj_dist.get(rnd_eng));
		update_id_t rnd_upd_id = update_id_t(rnd_upd_id_dist.get(rnd_eng));
		update_links_map[obj_ptrs[src_i]].push_back(UpdateNode(obj_ptrs[dest_i], rnd_upd_id));
	}

	PropagatedUpdateList update_list(etg_pool, update_links_map, obj_pool);

	update_list.prepare();

	typedef vector<vector<bool> > update_bin_map_type;

	vector<event_target_group_id_t>::const_iterator etg_it = etg_ids.begin();
	for (; etg_it != etg_ids.end(); ++etg_it) {

		update_bin_map_type expected_upd_bin_map;
		update_bin_map_type actual_upd_bin_map;

		initUpdateBinaryMap(expected_upd_bin_map, num_obj, upd_id_range+1);
		initUpdateBinaryMap(actual_upd_bin_map, num_obj, upd_id_range+1);

		list<EvSimObject*> visited_obj_list;

		std::set<EvSimObject*> etg_objects;

		EventTargetGroupPool::const_iterator etg_obj_it = etg_pool.begin(*etg_it);
		for (; etg_obj_it != etg_pool.end(*etg_it); ++etg_obj_it) {
			visited_obj_list.push_back(etg_obj_it->receiver);
			etg_objects.insert(etg_obj_it->receiver);
		}
		vector<bool> has_an_update_vec(num_obj, false);
		list<EvSimObject *>::iterator visited_it = visited_obj_list.begin();
		while (visited_it != visited_obj_list.end()) {
			AdjacentUpdateNodesVector::iterator update_nodes_it = update_links_map[*visited_it].begin();
			for (;update_nodes_it != update_links_map[*visited_it].end(); ++update_nodes_it) {
				if (!etg_objects.count(update_nodes_it->object)) {
					visited_obj_list.push_back(update_nodes_it->object);
					EvSimObject **found_it = std::find(obj_ptrs, obj_ptrs+sizeof(obj_ptrs)/sizeof(EvSimObject*), update_nodes_it->object);
					unsigned obj_idx = found_it - obj_ptrs;
					if (!has_an_update_vec[obj_idx]) {
						expected_upd_bin_map[obj_idx][update_nodes_it->updateid] = true;
						has_an_update_vec[obj_idx] = true;
					}
				}
			}
			++visited_it;
			visited_obj_list.pop_front();
		}

		PropagatedUpdateList::const_iterator upd_list_it = update_list.begin(*etg_it);
		for (;upd_list_it != update_list.end(*etg_it); ++upd_list_it) {
			EvSimObject **found_it = std::find(obj_ptrs, obj_ptrs+sizeof(obj_ptrs)/sizeof(EvSimObject *), upd_list_it->object);
			unsigned obj_idx = found_it - obj_ptrs;
			if (upd_list_it->is_single_update_id) {
				actual_upd_bin_map[obj_idx][upd_list_it->update_id] = true;
			}
			else {
				UpdateIDVector::const_iterator upd_id_it = upd_list_it->update_id_vec->begin();
				for (;upd_id_it != upd_list_it->update_id_vec->end(); ++upd_id_it) {
					actual_upd_bin_map[obj_idx][*upd_id_it] = true;
				}
			}
		}

		for (unsigned i = 0 ; i < num_obj; ++i) {
			for (unsigned j = 0 ; j <= upd_id_range; ++j) {
				CPPUNIT_ASSERT_EQUAL( expected_upd_bin_map[i][j], actual_upd_bin_map[i][j] );
			}
		}
	}

}

void PropagatedUpdateListTest::testCircle()
{
	EventTargetGroupPool etg_pool;
	EvSimObjectPool obj_pool;
	UpdateLinksMap update_links_map;

	MersenneTwister19937 rnd_eng;
	rnd_eng.seed(65304934);

	const unsigned num_obj = 5;

	TestEvSimObject obj_factory;

	EvSimObject *obj_ptrs[num_obj];
	EvSimObject::ID obj_ids[num_obj];

	for (unsigned i = 0 ; i < num_obj; ++i) {
		obj_ids[i] = obj_pool.createObject(obj_factory);
		obj_ptrs[i] = obj_pool.getObject(obj_ids[i]);
	}


	update_links_map[obj_ptrs[0]].push_back(UpdateNode(obj_ptrs[1], 0));
	update_links_map[obj_ptrs[1]].push_back(UpdateNode(obj_ptrs[2], 0));
	update_links_map[obj_ptrs[2]].push_back(UpdateNode(obj_ptrs[3], 0));
	update_links_map[obj_ptrs[3]].push_back(UpdateNode(obj_ptrs[4], 0));
	update_links_map[obj_ptrs[4]].push_back(UpdateNode(obj_ptrs[2], 0));

	etg_pool.createEventTargetGroup(obj_ptrs[0], port_id_t(0));

	PropagatedUpdateList update_list(etg_pool, update_links_map, obj_pool);

	bool caughtException = false;
	try {
		update_list.prepare();
	}
	catch(evesim::ConstructionException &e) {
		caughtException = true;
	}

	CPPUNIT_ASSERT( caughtException );
}
