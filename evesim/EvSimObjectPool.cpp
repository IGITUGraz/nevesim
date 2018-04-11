#include "EvSimObjectPool.h"
#include <iostream>
using std::cerr;
using std::endl;

#include <boost/format.hpp>

#include "SimException.h"

#include <string>
using std::string;

EvSimObjectPool::EvSimObjectPool() :
        pool(0)
{
}

/*!
 * Adds object in the pool.
 * the object is assigned id.
 */
EvSimObject::ID EvSimObjectPool::addObject(EvSimObject *o)
{
    EvSimObject::ID new_oid = pool.size() ;
    pool.push_back(o);
    return new_oid ;
}


/*!
 * Returns a pointer to the object identified by its id.
 */
EvSimObject *EvSimObjectPool::getObject(EvSimObject::ID oid) const
{
    if (oid >= pool.size())
    	throw( evesim::ConstructionException( "EvSimObjectPool::getObject(EvSimObject::ID oid)", str( boost::format( "Trying to access nonexistent object id=%1%") % oid) ) );
    return pool[oid];
}


/*!
 * Destructor. All sim objects that the pool contains
 * are also deleted from the heap.
 */
EvSimObjectPool::~EvSimObjectPool()
{
	Iterator oit;
    for (oit = begin() ; oit != end() ; ++oit) {
       	if ((*oit)->owned_by_net())
              delete *oit;
    }
}


/*!
 * Calls init() of every evsimobject in the pool.
 */
void EvSimObjectPool::initObjects(EvSimNetwork &net)
{
	Iterator oit;
	for (oit = begin() ; oit != end() ; ++oit) {
		(*oit)->init(net, oit.getID());
	}
}

/*!
 * Calls init() of every evsimobject in the pool.
 */
void EvSimObjectPool::resetObjects(InitializeContext &sim_ctxt)
{
	Iterator oit;
	for (oit = begin() ; oit != end() ; ++oit) {
		(*oit)->reset(sim_ctxt);
	}
}

