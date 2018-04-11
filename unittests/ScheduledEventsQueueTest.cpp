
#include "ScheduledEventsQueueTest.h"
#include "CppUnitMain.h"

#include <cstdio>

#include <vector>
#include <utility>
#include <algorithm>

#include "RandomDistribution.h"

SIM_LOCAL_AUTOBUILD_TEST_SUITE( ScheduledEventsQueueTest )

void ScheduledEventsQueueTest::setUp()
{
    queue = new ScheduledEventsQueue();
    queue->reset();
}

void ScheduledEventsQueueTest::tearDown()
{
    delete queue;
}

void ScheduledEventsQueueTest::testSimple()
{


	queue->scheduleEvent(event_target_group_id_t(3), Time(1));
	queue->scheduleEvent(event_target_group_id_t(5), Time(2));

	CPPUNIT_ASSERT_EQUAL( Time(1), queue->topTime() );

	ScheduledEventsQueue::Node *top_node = queue->top();

	CPPUNIT_ASSERT_EQUAL( event_target_group_id_t(3), top_node->etg );
	CPPUNIT_ASSERT_EQUAL( Time(1), top_node->time );

	queue->pop();

	CPPUNIT_ASSERT_EQUAL( Time(2), queue->topTime() );

	top_node = queue->top();

	CPPUNIT_ASSERT_EQUAL( event_target_group_id_t(5), top_node->etg );
	CPPUNIT_ASSERT_EQUAL( Time(2), top_node->time );

	queue->pop();

	CPPUNIT_ASSERT( queue->empty() );
}

bool compare_pair_second(const std::pair<event_target_group_id_t, Time> &left, const std::pair<event_target_group_id_t, Time> &right) {
	return left.second < right.second;
}

void ScheduledEventsQueueTest::testManyEvents() {
	std::vector< std::pair<event_target_group_id_t, Time > > events;
	const int num_events = 1000;

	for (int i = 0 ; i < num_events ; ++i)
		events.push_back(std::make_pair(event_target_group_id_t(i), Time(i)));

	MersenneTwister19937 rnd_eng;
	rnd_eng.seed(1450298);

	for (unsigned i = 0; i < num_events - 1; ++i) {
		UniformIntegerDistribution rnd_dist(i, num_events-1);
		std::swap(events[i], events[unsigned(rnd_dist.get(rnd_eng))]);
	}

	queue->reset();

	std::vector< std::pair<event_target_group_id_t, Time > >::const_iterator ev_it = events.begin();
    for (; ev_it != events.end(); ++ev_it)
    	queue->scheduleEvent(ev_it->first, ev_it->second);

    sort(events.begin(), events.end(), compare_pair_second);

    ev_it = events.begin();
    for (;ev_it != events.end(); ++ev_it) {
    	CPPUNIT_ASSERT_EQUAL( ev_it->second, queue->topTime());
    	ScheduledEventsQueue::Node *top_node = queue->top();
    	CPPUNIT_ASSERT_EQUAL( ev_it->second, top_node->time);
    	CPPUNIT_ASSERT_EQUAL( ev_it->first, top_node->etg);
    	queue->pop();
    }
    CPPUNIT_ASSERT( queue->empty() );
}
