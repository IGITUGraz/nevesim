#include "ProjectedEventsHeapBufferTest.h"
#include "TestEvSimObject.h"
#include "RandomDistribution.h"

#include <cmath>

#include <iostream>
using std::cerr;
using std::endl;

#include <utility>
using std::pair;


SIM_LOCAL_AUTOBUILD_TEST_SUITE( ProjectedEventsHeapBufferTest )

void ProjectedEventsHeapBufferTest::setUp()
{
}

void ProjectedEventsHeapBufferTest::tearDown()
{
}

void ProjectedEventsHeapBufferTest::testOneObject()
{
	ProjectedEventsHeapBuffer proj_events_buf;




	TestEvSimObject test_obj;
	EvSimObject * test_obj_ptr = static_cast<EvSimObject *>(&test_obj);
	test_obj.setNumProjOutputPorts(3);
	proj_events_buf.registerProjectedOutPorts(&test_obj);

	for (int i = 0 ; i < 3 ; ++i) {
		proj_events_buf.setGlobalOutputPort(i, i+1);
	}

	proj_events_buf.makeHeap();

	proj_events_buf.update(global_proj_out_port_id_t(0), Time(20));
	proj_events_buf.update(global_proj_out_port_id_t(1), Time(10));
	proj_events_buf.update(global_proj_out_port_id_t(2), Time(30));


	CPPUNIT_ASSERT_EQUAL( Time(10), proj_events_buf.topTime());
	pair<EvSimObject *, global_out_port_id_t> top_info = proj_events_buf.topSender();

	CPPUNIT_ASSERT_EQUAL( test_obj_ptr, top_info.first);
	CPPUNIT_ASSERT_EQUAL( global_out_port_id_t(2), top_info.second);

	proj_events_buf.update(global_proj_out_port_id_t(2), 5);

	CPPUNIT_ASSERT_EQUAL( Time(5), proj_events_buf.topTime());
	top_info = proj_events_buf.topSender();

	CPPUNIT_ASSERT_EQUAL( test_obj_ptr, top_info.first);
	CPPUNIT_ASSERT_EQUAL( global_out_port_id_t(3), top_info.second);

}

struct ProjPortInfo {
	ProjPortInfo(EvSimObject *obj, Time t, global_out_port_id_t port) :
		obj(obj), t(t), port(port) {}

	EvSimObject *obj;
	Time t;
	global_out_port_id_t port;
};

bool portInfoCompare(ProjPortInfo *left, ProjPortInfo *right) {
	return left->t < right->t;
}



void ProjectedEventsHeapBufferTest::testRandom()
{
	ProjectedEventsHeapBuffer proj_events_buf;

	vector<TestEvSimObject> test_obj_vec;
	vector<TestEvSimObject *> test_obj_ptr_vec;

	MersenneTwister19937 rnd_eng;
	rnd_eng.seed(123092830);

	UniformIntegerDistribution uni_int_dist(1,5);

	test_obj_vec.resize(20);
	test_obj_ptr_vec.resize(test_obj_vec.size());

	for (unsigned i = 0 ; i < test_obj_vec.size(); ++i) {
		test_obj_ptr_vec[i] = &test_obj_vec[i];
		test_obj_vec[i].setNumProjOutputPorts(int(uni_int_dist.get(rnd_eng)));
	}


	for (vector<TestEvSimObject*>::iterator obj_it = test_obj_ptr_vec.begin();
			                        obj_it != test_obj_ptr_vec.end(); ++obj_it) {
		proj_events_buf.registerProjectedOutPorts(*obj_it);
	}


	vector<ProjPortInfo> proj_ports_vec;
	vector<ProjPortInfo *> sorted_proj_ports_vec;


	unsigned port_counter = 0;
	for (unsigned i = 0 ; i < test_obj_vec.size() ; ++i) {
		for (unsigned port_i = 0 ; port_i < test_obj_vec[i].numProjOutputPorts() ; ++port_i) {
			proj_events_buf.setGlobalOutputPort(port_counter, port_counter+3);
			proj_ports_vec.push_back(ProjPortInfo(&test_obj_vec[i], MAX_TIME, port_counter+3));
			port_counter ++;
		}
	}

	for (unsigned i = 0 ; i < proj_ports_vec.size() ; ++i) {
		sorted_proj_ports_vec.push_back(&proj_ports_vec[i]);
	}

	proj_events_buf.makeHeap();

	UniformDistribution time_dist(0, 1e6);
	UniformIntegerDistribution port_dist(0, proj_ports_vec.size()-1);

	for (unsigned test_i = 0 ; test_i < 100; ++test_i) {
		for (unsigned change_i = 0; change_i < 30; ++change_i) {
			global_proj_out_port_id_t chosen_port = global_proj_out_port_id_t(port_dist.get(rnd_eng));
			Time new_time = time_dist.get(rnd_eng);
			proj_events_buf.update(chosen_port, new_time);
			proj_ports_vec[chosen_port].t = new_time;
		}

		sort(sorted_proj_ports_vec.begin(), sorted_proj_ports_vec.end(), portInfoCompare);

		CPPUNIT_ASSERT_EQUAL( sorted_proj_ports_vec[0]->t, proj_events_buf.topTime() ) ;
		pair<EvSimObject *, global_out_port_id_t> top_info = proj_events_buf.topSender();
		CPPUNIT_ASSERT_EQUAL( sorted_proj_ports_vec[0]->obj , top_info.first ) ;
		CPPUNIT_ASSERT_EQUAL( sorted_proj_ports_vec[0]->port , top_info.second ) ;
	}

}
