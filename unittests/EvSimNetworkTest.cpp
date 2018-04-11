#include "EvSimNetworkTest.h"
#include "TestEvSimObject.h"
#include "SimException.h"
#include "ArrayInputNeuron.h"
#include "EventTimesRecorder.h"
#include "RecordVariableTestSimObject.h"
#include "SimParameters.h"
#include "VariableRecorder.h"
#include "VariableRecorderTypes.h"
#include "ProjectedPortsTestSimObject.h"
#include "EvSimRegularClock.h"

#include <cmath>

#include "boost/lambda/lambda.hpp"
using namespace boost::lambda;

#include <iostream>
using std::cerr;
using std::endl;


#include <algorithm>
#include <functional>

// choose one from the following
SIM_LOCAL_AUTOBUILD_TEST_SUITE( EvSimNetworkTest )
// SIM_LOCAL_NIGHTLY_TEST_SUITE( EvSimNetworkTest )


class EvSimNetworkWithAccessors : public EvSimNetwork
{
public:
	EvSimNetworkWithAccessors(SimParameters sim_params = SimParameters()) : EvSimNetwork(sim_params)
	{}

	EvSimObjectPool & getObjPool() {
		return obj_pool;
	}

	EventTargetGroupPool & getETGPool() {
		return etg_pool;
	}

	RoutingDelayMap & getRoutingDelayMap() {
		return delay_map;
	}

};

void EvSimNetworkTest::setUp()
{
}

void EvSimNetworkTest::tearDown()
{
}

void EvSimNetworkTest::testCreateAddConnect()
{
	EvSimNetworkWithAccessors net;

	//***********************
	//  test create and add
	//***********************


	TesterDirectEventSender obj_fact;
	obj_fact.setNumOutputPorts(1);

	EvSimObject::ID src_objid1 = net.create(obj_fact);
	EvSimObject * src_obj1 = net.getObject(src_objid1);

	TesterDirectEventSender * test_src_obj1 = dynamic_cast<TesterDirectEventSender *>(src_obj1);

	CPPUNIT_ASSERT( test_src_obj1 != 0 );

	EvSimObject::IDVector idvec = net.create(obj_fact, 5);

	vector <TestEvSimObject *> obj_vec;
	for (unsigned i = 0 ; i < idvec->size() ; ++i) {
		obj_vec.push_back(dynamic_cast<TestEvSimObject *>(net.getObject((*idvec)[i])));
		CPPUNIT_ASSERT( obj_vec.back() != 0 );
	}

	TestEvSimObject *ext_obj = new TestEvSimObject();
	EvSimObject::ID ext_obj_id = net.add(ext_obj);

	TestEvSimObject *retrieved_obj = dynamic_cast<TestEvSimObject *>(net.getObject(ext_obj_id));

	CPPUNIT_ASSERT_EQUAL( ext_obj, retrieved_obj );


	//*********************
	//  test connect
	//*********************

	net.connect(src_objid1, 0, (*idvec)[0], 0);
	net.connect(src_objid1, 0, (*idvec)[2], 2);


	EventTargetGroupPool & etg_pool = net.getETGPool();
	RoutingDelayMap & delay_map = net.getRoutingDelayMap();

	CPPUNIT_ASSERT_EQUAL( 2u, etg_pool.numETGroups() );

	CPPUNIT_ASSERT_EQUAL( 8u, delay_map.getTotalNumOutPorts() );

	DirectEventSender * sender1 = dynamic_cast<DirectEventSender *>(test_src_obj1);


	RoutingDelayMap::const_iterator delay_map_it = delay_map.begin(sender1->getFirstOutPortGlobalID());

	unsigned n_etgs = 0;
	event_target_group_id_t etg_id = 0;
	for (;delay_map_it != delay_map.end(sender1->getFirstOutPortGlobalID()); ++delay_map_it, ++n_etgs) {
		CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0,  delay_map_it->first, 1e-6);
		etg_id = delay_map_it->second;
	}

	CPPUNIT_ASSERT_EQUAL( 1u, n_etgs );

	EventTargetGroupPool::const_iterator etg_it = etg_pool.begin(etg_id);

	vector<EvSimObject *> target_objs;
	vector<port_id_t> target_ports;
	for (; etg_it != etg_pool.end(etg_id); ++etg_it) {
		target_objs.push_back(etg_it->receiver);
		target_ports.push_back(etg_it->port);
	}

	CPPUNIT_ASSERT_EQUAL( dynamic_cast<EvSimObject *>(obj_vec[0]), target_objs[0]);
	CPPUNIT_ASSERT_EQUAL( dynamic_cast<EvSimObject *>(obj_vec[2]), target_objs[1]);

	CPPUNIT_ASSERT_EQUAL( port_id_t(0), target_ports[0]);
	CPPUNIT_ASSERT_EQUAL( port_id_t(2), target_ports[1]);

	//**********
	net.connect(src_objid1, 0, (*idvec)[1], 1, 1);
	net.connect((*idvec)[1], port_id_t(0), (*idvec)[3], port_id_t(2));

	CPPUNIT_ASSERT_EQUAL( 4u, etg_pool.numETGroups() );

	delay_map_it = delay_map.begin(sender1->getFirstOutPortGlobalID());
	n_etgs = 0;
	for (;delay_map_it != delay_map.end(sender1->getFirstOutPortGlobalID()); ++delay_map_it, ++n_etgs) {
		if (n_etgs == 1)
			CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0,  delay_map_it->first, 1e-6);
		etg_id = delay_map_it->second;
	}

	CPPUNIT_ASSERT_EQUAL( 2u, n_etgs);

	etg_it = etg_pool.begin(etg_id);


	CPPUNIT_ASSERT_EQUAL( dynamic_cast<EvSimObject *>(obj_vec[1]), etg_it->receiver);
	CPPUNIT_ASSERT_EQUAL(port_id_t(1), etg_it->port);
	++etg_it;
	CPPUNIT_ASSERT( etg_pool.end(etg_id) == etg_it);

	DirectEventSender *sender2 = dynamic_cast<DirectEventSender *>(obj_vec[1]);

	delay_map_it = delay_map.begin(sender2->getFirstOutPortGlobalID());
	CPPUNIT_ASSERT_EQUAL( 0.0,  delay_map_it->first);
	etg_id = delay_map_it->second;
	++delay_map_it;
	CPPUNIT_ASSERT( delay_map.end(sender2->getFirstOutPortGlobalID()) == delay_map_it);

	etg_it = etg_pool.begin(etg_id);

	CPPUNIT_ASSERT_EQUAL( dynamic_cast<EvSimObject *>(obj_vec[3]), etg_it->receiver);
	CPPUNIT_ASSERT_EQUAL(port_id_t(2), etg_it->port);
	++etg_it;
	CPPUNIT_ASSERT( etg_pool.end(etg_id) == etg_it);

	//***************************
	//  test connect exceptions
	//***************************

	bool exceptionCaught = false;

	try {
		net.connect(ext_obj_id, 0, (*idvec)[0], 0);
	}
	catch(evesim::ConstructionException &e) {
		exceptionCaught = true;
	}
	CPPUNIT_ASSERT( exceptionCaught );

	exceptionCaught = false;
	try {
		net.connect(src_objid1, 1, (*idvec)[0], 0);
	}
	catch(evesim::ConstructionException &e) {
		exceptionCaught = true;
	}
	CPPUNIT_ASSERT( exceptionCaught );

}

void EvSimNetworkTest::testAdvanceAndRecordEvents() {
	EvSimNetwork net;

	// test advance single object record
	EvSimObject::ID inp_nrn_id = net.create(ArrayInputNeuron());
	EvSimObject::ID rec_id = net.record(inp_nrn_id, 0);

	EvSimObject::ID inp_nrn_id2 = net.create(ArrayInputNeuron());
	EvSimObject::ID rec_id2 = net.create(EventTimesRecorder());

	double delay = 1.2e-3;
	net.connect(inp_nrn_id2, 0, rec_id2, 0, delay);

	ArrayInputNeuron *inp_obj = dynamic_cast<ArrayInputNeuron *>(net.getObject(inp_nrn_id));
	const double inp_events_arr[] = {1.2, 3.4, 3.4, 5.6};
	vector<Time> inp_events(inp_events_arr, inp_events_arr + sizeof(inp_events_arr)/sizeof(double));
	inp_obj->setInputEvents(inp_events);

	ArrayInputNeuron *inp_obj2 = dynamic_cast<ArrayInputNeuron *>(net.getObject(inp_nrn_id2));
	vector<Time> inp_events2(inp_events.size());
	std::transform(inp_events.begin(), inp_events.end(), inp_events2.begin(), std::bind1st(std::plus<Time>(), 0.5));
	inp_obj2->setInputEvents(inp_events2);

	net.advance(10.0);

	EventTimesRecorder *rec_obj = dynamic_cast<EventTimesRecorder *>(net.getObject(rec_id));
	const std::vector<Time> & rec_events = rec_obj->getRecordedValues();



	CPPUNIT_ASSERT_VECTOR_DOUBLES_EQUAL(inp_events, rec_events, 1e-6);


	EventTimesRecorder *rec_obj2 = dynamic_cast<EventTimesRecorder *>(net.getObject(rec_id2));
	const std::vector<Time> & rec_events2 = rec_obj2->getRecordedValues();

	vector<Time> expected_events2(inp_events2.size());
	std::transform(inp_events2.begin(), inp_events2.end(), expected_events2.begin(), std::bind1st(std::plus<Time>(), delay));
	CPPUNIT_ASSERT_VECTOR_DOUBLES_EQUAL(expected_events2, rec_events2, 1e-6);


	// test advance exceptions
	bool exceptionCaught = false;
	try {
		net.advance(MAX_TIME);
	} catch(evesim::Exception & e) {
		exceptionCaught = true;
	}
	CPPUNIT_ASSERT( exceptionCaught );

	exceptionCaught = false;
	try {
		net.simulate(MAX_TIME - 5);
	}
	catch(evesim::Exception &e){
		exceptionCaught = true;
	}

	// ******************************************
	// test record and advance with many objects
	// ******************************************

	EvSimNetwork net2;
    EvSimObject::IDVector inp_id_vec = net2.create(ArrayInputNeuron(), 5);
	EvSimObject::IDVector rec_id_vec = net2.record(inp_id_vec, port_id_t(0));

	Time event_times_arr[] = { 2.2, 3.4, 4.5, 7.6 , 10 };

	vector< vector<Time> > input_event_times;

	EvSimObject::IDVector::element_type::const_iterator inp_id_it = inp_id_vec->begin();
	int i = 0;
	for (;inp_id_it != inp_id_vec->end(); ++inp_id_it, ++i) {
		vector<Time> event_times(event_times_arr, event_times_arr + 5);
		std::transform(event_times.begin(), event_times.end(), event_times.begin(), std::bind1st(std::plus<Time>(), i));
		dynamic_cast<ArrayInputNeuron *>(net2.getObject(*inp_id_it))->setInputEvents(event_times);
		input_event_times.push_back(event_times);
	}

	net2.advance(9);

	EvSimObject::IDVector::element_type::const_iterator rec_id_it = rec_id_vec->begin();
	unsigned idx = 0;
	for (;rec_id_it != rec_id_vec->end(); ++rec_id_it, ++idx) {
		const std::vector<Time> & recorded_events = dynamic_cast<EventTimesRecorder *>(net2.getObject(*rec_id_it))->getRecordedValues();
		vector<Time>::const_iterator ev_it = input_event_times[idx].begin();
		vector<Time> expected_event_times;
		for (; ev_it != input_event_times[idx].end(); ++ev_it) {
			if (*ev_it <= 9)
				expected_event_times.push_back(*ev_it);
		}

		CPPUNIT_ASSERT_VECTOR_DOUBLES_EQUAL(expected_event_times, recorded_events, 1e-8);
	}
}

int dbl2int(const double &d) {
	return int(d);
}

void EvSimNetworkTest::testRecordVariables()
{
	SimParameters sim_params(123987, 0.5);
	EvSimNetwork net(sim_params);

	EvSimObject::ID rec_clock_id = net.create(EvSimRegularClock(0.5, true, 0.5));


	double change_times_arr[] = { 0.6, 1.2, 2.2, 4.5, 5.0 };
	vector<Time> change_times(change_times_arr, change_times_arr + 5);
	EvSimObject::ID inp_nrn_id = net.create(ArrayInputNeuron(change_times));

	double levels_arr[] = { 1, 2.1, 4, 5, 6, 8.2 };
	vector<double >levels(levels_arr, levels_arr + 6);

	EvSimObject::ID test_obj_id = net.create(RecordVariableTestSimObject(levels));

	net.connect(inp_nrn_id, 0, test_obj_id, 0);

	EvSimObject::ID rec_int_id = net.record(test_obj_id, "v_int", rec_clock_id);
	EvSimObject::ID rec_dbl_id = net.record(test_obj_id, "v_dbl", rec_clock_id);
	EvSimObject::ID rec_flt_id = net.record(test_obj_id, "v_float", rec_clock_id);
	EvSimObject::ID rec_uns_id = net.record(test_obj_id, "v_unsigned", rec_clock_id);

	net.advance(5.2);

	const vector<double> & dbl_rec_values = dynamic_cast<DoubleVariableRecorder *>(net.getObject(rec_dbl_id))->getRecordedValues();
	const vector<float> float_rec_values = dynamic_cast<FloatVariableRecorder *>(net.getObject(rec_flt_id))->getRecordedValues();
	const vector<int> & int_rec_values = dynamic_cast<IntVariableRecorder *>(net.getObject(rec_int_id))->getRecordedValues();
	const vector<unsigned> & uns_rec_values = dynamic_cast<UnsignedVariableRecorder *>(net.getObject(rec_uns_id))->getRecordedValues();

	double expected_dbl_values_arr[] = { 1, 2.1, 4, 4, 5, 5, 5, 5};
	vector<double> expected_dbl_values(expected_dbl_values_arr, expected_dbl_values_arr + sizeof(expected_dbl_values_arr)/sizeof(double));

	vector<float> expected_float_values(expected_dbl_values.begin(), expected_dbl_values.end());

	vector<int> expected_int_values(expected_dbl_values.size());
	vector<unsigned> expected_unsigned_values(expected_dbl_values.size());

	std::transform(expected_dbl_values.begin(), expected_dbl_values.end(), expected_int_values.begin(), dbl2int);
	std::transform(expected_dbl_values.begin(), expected_dbl_values.end(), expected_unsigned_values.begin(), dbl2int);

	vector<double> dbl_rec_values_no_last2(dbl_rec_values.begin(), dbl_rec_values.end()-2);
	vector<float> float_rec_values_no_last2(float_rec_values.begin(), float_rec_values.end()-2);
	vector<int> int_rec_values_no_last2(int_rec_values.begin(), int_rec_values.end()-2);
	vector<unsigned> unsigned_rec_values_no_last2(uns_rec_values.begin(), uns_rec_values.end()-2);

	CPPUNIT_ASSERT_VECTOR_DOUBLES_EQUAL( expected_dbl_values, dbl_rec_values_no_last2, 1e-6);
	CPPUNIT_ASSERT_VECTOR_DOUBLES_EQUAL( expected_float_values, float_rec_values_no_last2, 1e-6);
	CPPUNIT_ASSERT_VECTOR_EQUAL( expected_int_values, int_rec_values_no_last2);
	CPPUNIT_ASSERT_VECTOR_EQUAL( expected_unsigned_values, unsigned_rec_values_no_last2);

	CPPUNIT_ASSERT( fabs(dbl_rec_values[dbl_rec_values.size()-2] - 5) < 1e-6 ||
			        fabs(dbl_rec_values[dbl_rec_values.size()-2] - 6) < 1e-6  );
	CPPUNIT_ASSERT( fabs(dbl_rec_values.back() - 6) < 1e-6 ||
				        fabs(dbl_rec_values.back() - 8.2) < 1e-6 );

	CPPUNIT_ASSERT( fabs(float_rec_values[float_rec_values.size()-2] - 5) < 1e-6 ||
				        fabs(float_rec_values[float_rec_values.size()-2] - 6) < 1e-6  );
	CPPUNIT_ASSERT( fabs(float_rec_values.back() - 6) < 1e-6 ||
					       fabs(float_rec_values.back() - 8.2) < 1e-6 );

	CPPUNIT_ASSERT( int_rec_values[float_rec_values.size()-2] == 5 ||
					        int_rec_values[float_rec_values.size()-2] == 6 );
	CPPUNIT_ASSERT( int_rec_values.back() == 6 ||
						       int_rec_values.back() == 8 );

	CPPUNIT_ASSERT( uns_rec_values[uns_rec_values.size()-2] == 5 ||
						        uns_rec_values[uns_rec_values.size()-2] == 6 );
	CPPUNIT_ASSERT( uns_rec_values.back() == 6 ||
							       uns_rec_values.back() == 8 );

}


void EvSimNetworkTest::testAdvanceWithProjectedPorts()
{
	EvSimNetwork net(SimParameters(1230983,9.9));

	EvSimObject::IDVector inp_nrn_ids = net.create(ArrayInputNeuron(), 4);

	Time *update_times_arr[4];
    size_t update_times_arr_sizes[] = {5,5,5,4};
	Time update_times_arr_0[] = {1, 2, 3, 4, 5};
	update_times_arr[0] = update_times_arr_0;

	Time update_times_arr_1[] = {1.2, 2.3, 2.4, 4.6, 7.8};
	update_times_arr[1] = update_times_arr_1;

	Time update_times_arr_2[] = {0.1, 1.3, 1.4, 2.6, 4.8};
	update_times_arr[2] = update_times_arr_2;

	Time update_times_arr_3[] = {1, 2, 3, 4};
	update_times_arr[3] = update_times_arr_3;

	vector< vector<Time> > update_times;
	for (int i = 0; i < 4; ++i) {
		update_times.push_back(vector<Time>(update_times_arr[i], update_times_arr[i] + update_times_arr_sizes[i]));
	}

	EvSimObject::IDVector::element_type::const_iterator inp_nrn_it = inp_nrn_ids->begin();

    for (int i = 0;inp_nrn_it != inp_nrn_ids->end(); ++inp_nrn_it, ++i) {
    	dynamic_cast<ArrayInputNeuron*>(net.getObject(*inp_nrn_it))->setInputEvents(update_times[i]);
    }

    EvSimObject::IDVector test_obj_ids = net.create(ProjectedPortsTestSimObject(), 4);

    Time *proj_event_times_arr[4];
    size_t proj_event_times_arr_sizes[] = {6,6,6,5};
	Time proj_event_times_arr_0[] = {1.5, 1.2, 3.1, 10, 4.2, 5.5};
	proj_event_times_arr[0] = proj_event_times_arr_0;
							// {1.2, 4.2, 5.5}

	Time proj_event_times_arr_1[] = {1.21, 2.31, 2.41, 4.61, 7.81, 9};
	proj_event_times_arr[1] = proj_event_times_arr_1;
							// { 9}

	Time proj_event_times_arr_2[] = {0.05, 1.25, 1.35, 2, 4, 12.5};
	proj_event_times_arr[2] = proj_event_times_arr_2;
							// {0.05, 1.25, 1.35, 2, 4}

	Time proj_event_times_arr_3[] = {5, 4, 3, 3, 4.1};
	proj_event_times_arr[3] = proj_event_times_arr_3;
							// {3, 3, 4.1}

	vector< vector<Time> > proj_event_times;
	for (int i = 0 ; i < 4 ; ++i) {
		proj_event_times.push_back(vector<Time>(proj_event_times_arr[i],
										proj_event_times_arr[i] + proj_event_times_arr_sizes[i]));
	}

	EvSimObject::IDVector::element_type::const_iterator test_obj_it = test_obj_ids->begin();
	int i = 0;
	for (;test_obj_it != test_obj_ids->end(); ++test_obj_it, ++i) {
		dynamic_cast<ProjectedPortsTestSimObject*>(net.getObject(*test_obj_it))->setProjectedEventTimes(proj_event_times[i]);
	}

	EvSimObject::IDVector rec_ids = net.record(test_obj_ids, 0);
	EvSimObject::ID all_rec_id = net.create(EventTimesRecorder());

	test_obj_it = test_obj_ids->begin();
	for (;test_obj_it != test_obj_ids->end(); ++test_obj_it) {
		net.connect(*test_obj_it, 0, all_rec_id, 0);
	}

	for (unsigned i = 0 ; i < 4 ; ++i) {
		net.connect((*inp_nrn_ids)[i], 0, (*test_obj_ids)[i], 0);
	}

	net.advance(10);

	vector< const std::vector<Time> * > rec_ev_times;
	EvSimObject::IDVector::element_type::const_iterator rec_it = rec_ids->begin();
	for (;rec_it != rec_ids->end(); ++rec_it) {
		rec_ev_times.push_back(&dynamic_cast<EventTimesRecorder *>(net.getObject(*rec_it))->getRecordedValues());
	}

	Time *expected_rec_ev_times_arr[4];
	size_t expected_rec_ev_times_arr_sizes[] = {3,1,5,2};
	Time expected_rec_ev_times_arr_0[] = {1.2, 4.2, 5.5};
	expected_rec_ev_times_arr[0] = expected_rec_ev_times_arr_0;

	Time expected_rec_ev_times_arr_1[] = {9};
	expected_rec_ev_times_arr[1] = expected_rec_ev_times_arr_1;

	Time expected_rec_ev_times_arr_2[] = {0.05, 1.25, 1.35, 2, 4};
	expected_rec_ev_times_arr[2] = expected_rec_ev_times_arr_2;

	Time expected_rec_ev_times_arr_3[] = {3, 4.1};
	expected_rec_ev_times_arr[3] = expected_rec_ev_times_arr_3;

	vector< vector<Time> > expected_rec_ev_times;
	for (int i = 0 ; i < 4 ; ++i) {
		expected_rec_ev_times.push_back(vector<Time>(expected_rec_ev_times_arr[i],
										expected_rec_ev_times_arr[i] + expected_rec_ev_times_arr_sizes[i]));
	}

	const std::vector<Time> & alltogether_rec_ev_times = dynamic_cast<EventTimesRecorder *>(net.getObject(all_rec_id))->getRecordedValues();

	vector<Time> expected_all_rec_ev_times;
	for (unsigned i = 0 ; i < expected_rec_ev_times.size() ; ++i) {
			expected_all_rec_ev_times.insert( expected_all_rec_ev_times.end(),
											  expected_rec_ev_times[i].begin(),
					                          expected_rec_ev_times[i].end());
	}
	sort(expected_all_rec_ev_times.begin(), expected_all_rec_ev_times.end());

	for (unsigned i = 0 ; i < 4 ; ++i) {
		CPPUNIT_ASSERT_VECTOR_DOUBLES_EQUAL( expected_rec_ev_times[0], *rec_ev_times[0], 1e-8);
	}

	CPPUNIT_ASSERT_VECTOR_DOUBLES_EQUAL( expected_all_rec_ev_times, alltogether_rec_ev_times, 1e-8);

}

void EvSimNetworkTest::testAdvanceRandomized() {

}

