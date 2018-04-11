#include "EvSimObjectPoolTest.h"
#include "SimException.h"
#include "EvSimObjectTypeUtils.h"
#include "TestEvSimObject.h"

#include <cmath>

#include <iostream>
using std::cerr;
using std::endl;

#include <vector>
using std::vector;

// choose one from the following
SIM_LOCAL_AUTOBUILD_TEST_SUITE( EvSimObjectPoolTest )

void EvSimObjectPoolTest::setUp()
{

}

void EvSimObjectPoolTest::tearDown()
{

}


void EvSimObjectPoolTest::test()
{
	EvSimObjectPool obj_pool;

	EvSimObject * o1 = new TestEvSimObject();

	EvSimObject::ID o1_id = obj_pool.addObject(o1);

	CPPUNIT_ASSERT_EQUAL( o1_id , EvSimObject::ID(0) );

	EvSimObject * o1_retrieved = obj_pool.getObject(o1_id);

	CPPUNIT_ASSERT_EQUAL( o1, o1_retrieved) ;

	TestEvSimObject obj_factory;

	EvSimObject::ID o2_id = obj_pool.createObject(obj_factory);

	EvSimObject *o2 = obj_pool.getObject(o2_id);

	CPPUNIT_ASSERT_EQUAL( obj_pool.numOfObjects() , 2);

	TestEvSimObject * simple_o2 = dynamic_cast<TestEvSimObject *>(o2);

	CPPUNIT_ASSERT( simple_o2 != 0 );

	EvSimObject::ID non_existent_id = 3;

	bool exceptionCaught = false;

	try {
		obj_pool.getObject(non_existent_id);
	}
	catch (evesim::ConstructionException &e) {
		exceptionCaught = true;
	}

	CPPUNIT_ASSERT(exceptionCaught);

	obj_pool.createObject(obj_factory);

	vector<EvSimObject *> sim_obj_vec;

	for (EvSimObjectPool::Iterator it = obj_pool.begin(); it != obj_pool.end() ; ++it) {
		sim_obj_vec.push_back(*it);
	}

	CPPUNIT_ASSERT_EQUAL( sim_obj_vec[0], o1);

	CPPUNIT_ASSERT_EQUAL( sim_obj_vec[1], o2);

	CPPUNIT_ASSERT_EQUAL( sim_obj_vec.size(), size_t(3) );

}
