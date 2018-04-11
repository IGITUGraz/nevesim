#include "EvSimEngineTest.h"
#include <cmath>

#include "TesterProjectedEventSender.h"
#include "ProjectedOutputPortIterator.h"

#include <iostream>
using std::cerr;
using std::endl;

#include <vector>
using std::vector;
#include <utility>
using std::pair;
using std::make_pair;
#include "utils.h"

// choose one from the following
SIM_LOCAL_AUTOBUILD_TEST_SUITE( EvSimEngineTest )
// SIM_LOCAL_NIGHTLY_TEST_SUITE( EvSimEngineTest )

void EvSimEngineTest::setUp()
{
}

void EvSimEngineTest::tearDown()
{
}

class EvSimEngineWithAccessors : public EvSimEngine {
public:
	EvSimEngineWithAccessors(EvSimNetwork &net, EvSimObjectPool &obj_pool, RoutingDelayMap &delay_map,
			EventTargetGroupPool &etg_pool, UpdateLinksMap &upd_links_map) :
				EvSimEngine(net, obj_pool, delay_map, etg_pool, upd_links_map)
	{};

	// just for testing
	ProjectedEventsHeapBuffer & getProjEvHeapBuffer() {
		return proj_events_buf;
	};

	// just for testing
	ScheduledEventsQueue & getScheduledEventsQueue() {
		return scheduled_events_queue;
	};

	double & getCurrSimClock() {
		return curr_sim_clock;
	}


};

void EvSimEngineTest::testScheduleEvent() {
	SimParameters sim_params(524332);

	EvSimNetwork net(sim_params);
	EvSimObjectPool obj_pool;
	RoutingDelayMap delay_map;
	EventTargetGroupPool etg_pool;
	UpdateLinksMap upd_links_map;

	EvSimEngineWithAccessors eng(net, obj_pool, delay_map, etg_pool, upd_links_map);

	TestEvSimObject obj_factory;

	TesterDirectEventSender src_obj_fact;

	EvSimObject::ID src_objid = obj_pool.createObject(src_obj_fact);
	EvSimObject *src_obj = obj_pool.getObject(src_objid);
	TesterDirectEventSender *test_src_obj = dynamic_cast<TesterDirectEventSender *>(src_obj);
	test_src_obj->setNumOutputPorts(1);
	delay_map.registerOutPorts(src_obj);

	EvSimObject::ID src_objid2 = obj_pool.createObject(src_obj_fact);
	EvSimObject *src_obj2 = obj_pool.getObject(src_objid2);
	TesterDirectEventSender *test_src_obj2 = dynamic_cast<TesterDirectEventSender *>(src_obj2);
	test_src_obj->setNumOutputPorts(2);
	delay_map.registerOutPorts(src_obj2);


	EvSimObject::ID objid1 = obj_pool.createObject(obj_factory);
	EvSimObject::ID objid2 = obj_pool.createObject(obj_factory);
	EvSimObject::ID objid3 = obj_pool.createObject(obj_factory);
	EvSimObject::ID objid4 = obj_pool.createObject(obj_factory);


	event_target_group_id_t etg1 = etg_pool.createEventTargetGroup(obj_pool.getObject(objid1), 1);

	event_target_group_id_t etg2 = etg_pool.createEventTargetGroup(obj_pool.getObject(objid2), 2);
	etg_pool.addEventTarget(etg2, obj_pool.getObject(objid3), 0);

	event_target_group_id_t etg3 = etg_pool.createEventTargetGroup(obj_pool.getObject(objid4), 0);


	global_out_port_id_t src_port_global_id1 = test_src_obj->getFirstOutPortGlobalID();

	global_out_port_id_t src_port_global_id2 = test_src_obj2->getFirstOutPortGlobalID();


	delay_map.insert(src_port_global_id1, 1, etg1);
	delay_map.insert(src_port_global_id1, 2, etg2);

	delay_map.insert(src_port_global_id1, 0, etg3);

	delay_map.insert(src_port_global_id2, 1.2, etg2);


	eng.init();
	ScheduledEventsQueue &queue = eng.getScheduledEventsQueue();
	queue.reset();
	eng.getCurrSimClock() = 4;
	eng.scheduleEvent(src_port_global_id1, 4);

	vector< pair<port_id_t, Time> > expected_recv_events4;
	expected_recv_events4.push_back(make_pair(0, 4));

	TestEvSimObject *test_obj4 = dynamic_cast<TestEvSimObject *>(obj_pool.getObject(objid4));
	CPPUNIT_ASSERT_VECTOR_EQUAL( expected_recv_events4, test_obj4->getRecvEvents() );


	CPPUNIT_ASSERT_EQUAL( etg1, queue.top()->etg);
	CPPUNIT_ASSERT_EQUAL( Time(5), queue.top()->time);

	queue.pop();

	eng.scheduleEvent(src_port_global_id2, 5);

	eng.getCurrSimClock() = 5.5;
	eng.scheduleEvent(src_port_global_id1, 5.5);

	expected_recv_events4.push_back(make_pair(0, 5.5));
	CPPUNIT_ASSERT_VECTOR_EQUAL( expected_recv_events4, test_obj4->getRecvEvents() );

	CPPUNIT_ASSERT_EQUAL( etg2, queue.top()->etg);
	CPPUNIT_ASSERT_EQUAL( Time(6), queue.top()->time);

	queue.pop();

	CPPUNIT_ASSERT_EQUAL( etg2, queue.top()->etg);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( Time(6.2), queue.top()->time, 1e-4);

	queue.pop();


	CPPUNIT_ASSERT_DOUBLES_EQUAL( Time(6.5), queue.top()->time, 1e-4);
	CPPUNIT_ASSERT_EQUAL( etg1, queue.top()->etg);

	queue.pop();

	CPPUNIT_ASSERT_EQUAL( etg2, queue.top()->etg);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( Time(7.5), queue.top()->time, 1e-4);

}

void EvSimEngineTest::testDeliverScheduledEvent() {

	SimParameters sim_params(12340294);

	EvSimNetwork net(sim_params);
	EvSimObjectPool obj_pool;
	RoutingDelayMap delay_map;
	EventTargetGroupPool etg_pool;
	UpdateLinksMap upd_links_map;

	EvSimEngineWithAccessors eng(net, obj_pool, delay_map, etg_pool, upd_links_map);

	TestEvSimObject obj_factory;

	EvSimObject::ID objid1 = obj_pool.createObject(obj_factory);
	EvSimObject * obj1 = obj_pool.getObject(objid1);
	EvSimObject::ID objid2 = obj_pool.createObject(obj_factory);
	EvSimObject * obj2 = obj_pool.getObject(objid2);
	EvSimObject::ID objid3 = obj_pool.createObject(obj_factory);
	EvSimObject * obj3 = obj_pool.getObject(objid3);

	event_target_group_id_t etg = etg_pool.createEventTargetGroup(obj1, 0);

	etg_pool.addEventTarget(etg, obj2, 1);
	etg_pool.addEventTarget(etg, obj3, 2);

	EvSimObject::ID objid4 = obj_pool.createObject(obj_factory);
	EvSimObject * obj4 = obj_pool.getObject(objid4);

	EvSimObject::ID objid5 = obj_pool.createObject(obj_factory);
	EvSimObject * obj5 = obj_pool.getObject(objid5);

	upd_links_map[obj1].push_back(UpdateNode(obj4, 2));
	upd_links_map[obj2].push_back(UpdateNode(obj5, 3));
	upd_links_map[obj3].push_back(UpdateNode(obj4, 4));

	eng.init();
	eng.deliverScheduledEvent(etg, 2);

	TestEvSimObject *test_obj1 = dynamic_cast<TestEvSimObject *>(obj1);

	vector< pair<port_id_t, Time> > expected_recv_events1;
	expected_recv_events1.push_back(make_pair(0, 2));
	CPPUNIT_ASSERT_VECTOR_EQUAL(expected_recv_events1, test_obj1->getRecvEvents());
	CPPUNIT_ASSERT( test_obj1->getRecvUpdates().empty() );

	TestEvSimObject *test_obj2 = dynamic_cast<TestEvSimObject *>(obj2);
	vector< pair<port_id_t, Time> > expected_recv_events2;

	expected_recv_events2.push_back(make_pair(1, 2));
	CPPUNIT_ASSERT_VECTOR_EQUAL(expected_recv_events2, test_obj2->getRecvEvents());
	CPPUNIT_ASSERT( test_obj2->getRecvUpdates().empty() );

	TestEvSimObject *test_obj3 = dynamic_cast<TestEvSimObject *>(obj3);
	vector< pair<port_id_t, Time> > expected_recv_events3;

	expected_recv_events3.push_back(make_pair(2, 2));
	CPPUNIT_ASSERT_VECTOR_EQUAL(expected_recv_events3, test_obj3->getRecvEvents());
	CPPUNIT_ASSERT( test_obj3->getRecvUpdates().empty() );

	TestEvSimObject * test_obj4 = dynamic_cast<TestEvSimObject *>(obj4);

	vector< pair<update_id_t, Time> > expected_recv_updates4;
	expected_recv_updates4.push_back(make_pair(2, 2));
	CPPUNIT_ASSERT_VECTOR_EQUAL( expected_recv_updates4, test_obj4->getRecvUpdates());

	TestEvSimObject * test_obj5 = dynamic_cast<TestEvSimObject *>(obj5);

	vector< pair<update_id_t, Time> > expected_recv_updates5;
	expected_recv_updates5.push_back(make_pair(3, 2));
	CPPUNIT_ASSERT_VECTOR_EQUAL( expected_recv_updates5, test_obj5->getRecvUpdates());

	eng.deliverScheduledEvent(etg, 5);

	expected_recv_events1.push_back(make_pair(0, 5));
	CPPUNIT_ASSERT_VECTOR_EQUAL(expected_recv_events1, test_obj1->getRecvEvents());

	expected_recv_events2.push_back(make_pair(1, 5));
	CPPUNIT_ASSERT_VECTOR_EQUAL(expected_recv_events2, test_obj2->getRecvEvents());

	expected_recv_events3.push_back(make_pair(2, 5));
	CPPUNIT_ASSERT_VECTOR_EQUAL(expected_recv_events3, test_obj3->getRecvEvents());
	CPPUNIT_ASSERT( test_obj3->getRecvUpdates().empty() );


	expected_recv_updates4.push_back(make_pair(2, 5));
	CPPUNIT_ASSERT_VECTOR_EQUAL( expected_recv_updates4, test_obj4->getRecvUpdates());


	expected_recv_updates5.push_back(make_pair(3, 5));
	CPPUNIT_ASSERT_VECTOR_EQUAL( expected_recv_updates5, test_obj5->getRecvUpdates());

}


void EvSimEngineTest::testRegisterProjOutPorts()
{

	SimParameters sim_params(12340294);

	EvSimNetwork net(sim_params);
	EvSimObjectPool obj_pool;
	RoutingDelayMap delay_map;
	EventTargetGroupPool etg_pool;
	UpdateLinksMap upd_links_map;


	EvSimEngineWithAccessors eng(net, obj_pool, delay_map, etg_pool, upd_links_map);

	TesterSingleOutProjectedEventSender single_out_sender_factory;
	TesterMultiOutProjectedEventSender multi_out_sender_factory;

	EvSimObject::ID oid = obj_pool.createObject(single_out_sender_factory);
	TesterSingleOutProjectedEventSender *sender1 = dynamic_cast<TesterSingleOutProjectedEventSender *>(obj_pool.getObject(oid));
	CPPUNIT_ASSERT( sender1 != 0 );
	sender1->setNumProjOutputPorts(1);
	sender1->setNumOutputPorts(1);


	delay_map.registerOutPorts(sender1);
	eng.registerProjectedOutputPorts(sender1);

	oid = obj_pool.createObject(multi_out_sender_factory);
	TesterMultiOutProjectedEventSender *sender2 = dynamic_cast<TesterMultiOutProjectedEventSender *>(obj_pool.getObject(oid));
	sender2->setNumProjOutputPorts(2);
	sender2->setNumOutputPorts(4);

	EvSimObject::PortType port_types[] = { EvSimObject::eventPortProjected, EvSimObject::eventPortDirect,
											EvSimObject::eventPortProjected, EvSimObject::eventPortDirect };
	std::vector<EvSimObject::PortType> port_types_vec(port_types, port_types+4);
	sender2->setOutPortTypes(port_types_vec);

	delay_map.registerOutPorts(sender2);
	eng.registerProjectedOutputPorts(sender2);

	TestEvSimObject obj_factory;

	oid = obj_pool.createObject(obj_factory);
	TestEvSimObject * obj = dynamic_cast<TestEvSimObject *>(obj_pool.getObject(oid));
	obj->setNumProjOutputPorts(0);
	obj->setNumOutputPorts(1);

	delay_map.registerOutPorts(obj);
	eng.registerProjectedOutputPorts(obj);


	oid = obj_pool.createObject(single_out_sender_factory);
	TesterSingleOutProjectedEventSender * sender3 = dynamic_cast<TesterSingleOutProjectedEventSender *>(obj_pool.getObject(oid));
	sender3->setNumProjOutputPorts(1);
	sender3->setNumOutputPorts(1);

	delay_map.registerOutPorts(sender3);
	eng.registerProjectedOutputPorts(sender3);

	oid = obj_pool.createObject(multi_out_sender_factory);
	TesterMultiOutProjectedEventSender *sender4 = dynamic_cast<TesterMultiOutProjectedEventSender *>(obj_pool.getObject(oid));
	sender4->setNumProjOutputPorts(1);
	sender4->setNumOutputPorts(3);


	EvSimObject::PortType port_types2[] = { EvSimObject::eventPortProjected, EvSimObject::eventPortDirect, EvSimObject::eventPortDirect };
	std::vector<EvSimObject::PortType> port_types_vec2(port_types2, port_types2+4);
	sender4->setOutPortTypes(port_types_vec2);

	delay_map.registerOutPorts(sender4);
	eng.registerProjectedOutputPorts(sender4);

	// sender 1
	CPPUNIT_ASSERT_EQUAL( global_out_port_id_t(0), sender1->getFirstOutPortGlobalID() );

	std::pair<port_id_t, global_proj_out_port_id_t> port_info = sender1->getFirstProjectedPortInfo();
	CPPUNIT_ASSERT_EQUAL( port_id_t(0), port_info.first);
    CPPUNIT_ASSERT_EQUAL( global_proj_out_port_id_t(0), port_info.second);


	// sender 2
	CPPUNIT_ASSERT_EQUAL( global_out_port_id_t(1), sender2->getFirstOutPortGlobalID() );

	ProjectedOutputPortIterator proj_port_it = sender2->getProjectedPortIterator();
	CPPUNIT_ASSERT_EQUAL( EvSimObject::eventPortProjected, sender2->getOutputPortType(0));
	port_info = proj_port_it.nextPort();
	CPPUNIT_ASSERT_EQUAL( port_id_t(0), port_info.first);
	CPPUNIT_ASSERT_EQUAL( global_proj_out_port_id_t(1), port_info.second);
	port_info = proj_port_it.nextPort();
	CPPUNIT_ASSERT_EQUAL( port_id_t(2), port_info.first);
	CPPUNIT_ASSERT_EQUAL( global_proj_out_port_id_t(2), port_info.second);
	CPPUNIT_ASSERT_EQUAL( false, proj_port_it.hasNextPort());

	// sender 3
	CPPUNIT_ASSERT_EQUAL( global_out_port_id_t(5), sender3->getFirstOutPortGlobalID() );

	port_info = sender3->getFirstProjectedPortInfo();
	CPPUNIT_ASSERT_EQUAL( port_id_t(0), port_info.first);
	CPPUNIT_ASSERT_EQUAL( global_proj_out_port_id_t(3), port_info.second);

	// sender 4
	CPPUNIT_ASSERT_EQUAL( global_out_port_id_t(6), sender4->getFirstOutPortGlobalID() );

	port_info = sender4->getFirstProjectedPortInfo();
	CPPUNIT_ASSERT_EQUAL( port_id_t(0), port_info.first);
	CPPUNIT_ASSERT_EQUAL( global_proj_out_port_id_t(4), port_info.second);

	proj_port_it = sender4->getProjectedPortIterator();
	port_info = proj_port_it.nextPort();
	CPPUNIT_ASSERT_EQUAL( port_id_t(0), port_info.first);
	CPPUNIT_ASSERT_EQUAL( global_proj_out_port_id_t(4), port_info.second);
	CPPUNIT_ASSERT_EQUAL( false, proj_port_it.hasNextPort());

	eng.init();
	ProjectedEventsHeapBuffer &heap_buf = eng.getProjEvHeapBuffer();

	heap_buf.update(4, 4);
	CPPUNIT_ASSERT_EQUAL( dynamic_cast<EvSimObject *>(sender4), heap_buf.topSender().first);
	CPPUNIT_ASSERT_EQUAL( global_out_port_id_t(6), heap_buf.topSender().second);

	heap_buf.update(3, 3);
	CPPUNIT_ASSERT_EQUAL( dynamic_cast<EvSimObject *>(sender3), heap_buf.topSender().first);
	CPPUNIT_ASSERT_EQUAL( global_out_port_id_t(5), heap_buf.topSender().second);

	heap_buf.update(2, 2);
	CPPUNIT_ASSERT_EQUAL( dynamic_cast<EvSimObject *>(sender2), heap_buf.topSender().first);
	CPPUNIT_ASSERT_EQUAL( global_out_port_id_t(3), heap_buf.topSender().second);

	heap_buf.update(1, 1);
	CPPUNIT_ASSERT_EQUAL( dynamic_cast<EvSimObject *>(sender2), heap_buf.topSender().first);
	CPPUNIT_ASSERT_EQUAL( global_out_port_id_t(1), heap_buf.topSender().second);

	heap_buf.update(0, 0);
	CPPUNIT_ASSERT_EQUAL( dynamic_cast<EvSimObject *>(sender1), heap_buf.topSender().first);
	CPPUNIT_ASSERT_EQUAL( global_out_port_id_t(0), heap_buf.topSender().second);

}
