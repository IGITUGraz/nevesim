#ifndef EVSIMOBJECTPOOL_H_
#define EVSIMOBJECTPOOL_H_

#include "EvSimObject.h"
#include "SimContext.h"

#include <vector>
using std::vector;

//! Container for all elements in the simulation
class EvSimObjectPool
{
public:

    EvSimObjectPool();

    virtual ~EvSimObjectPool();

    //! Add a new instance of the specified object by using a factory
    EvSimObject::ID createObject( EvSimObjectFactory const& objFactory  );

    //! Add an already created sim object in the pool
    EvSimObject::ID addObject(EvSimObject *o);


    //! Get a pointer to the specified objects
    EvSimObject *getObject(EvSimObject::ID oid) const;

    //! A call to this function makes sure that all objects contained in the pool are initialized properly and ready for simulation
    void initObjects(EvSimNetwork & net);

    void resetObjects(InitializeContext &sim_ctxt);

    //! Returns the number of objects
    int numOfObjects();

	//! Return the highest id value
    inline EvSimObject::ID getLastId();


//***************************
    class Iterator
    {
    public:
        Iterator( vector<EvSimObject*> *vec_=NULL, size_t idx_=0):
                vec(vec_), idx(idx_)
        { /*NOOP*/ };

        EvSimObject** operator->()
        {
            return &((*vec)[idx]);
        }

        EvSimObject* operator*()
        {
            return ((*vec)[idx]);
        }

        Iterator & operator=(const Iterator & other) {
        	vec = other.vec;
        	idx = other.idx;
        	return *this;
        }

        bool operator!=(const Iterator& other)
        {
            return ( idx != other.idx || vec!=other.vec );
        }

        Iterator& operator++()
        {
            idx++;
            return *this;
        }

        inline EvSimObject::ID getID(void)
        {
            return idx;
        };

    private:
        vector<EvSimObject*> *vec;
        size_t idx;
    };


    //! Returns an iterator for looping over all objects
    /*!
     */
    inline Iterator begin()
    {
        return Iterator( &pool, 0 );
    }

    //! End iterator corresponding to begin()
    /* \param type Integer ID specifying the type of objects
     */
    inline Iterator end()
    {
        return Iterator( &(pool), pool.size() );
    }

protected:

    EvSimObject::ID addObject( EvSimObjectFactory const& objFactory, RandomEngine *rng );


    vector< EvSimObject*> pool;

private:

    EvSimObject::BlockAllocator _blockAllocator;

};

inline EvSimObject::ID EvSimObjectPool::addObject( EvSimObjectFactory const& objFactory, RandomEngine *rng )
{
    EvSimObject::setAllocator( _blockAllocator );
    return  addObject( objFactory.create( rng ) );
}

inline EvSimObject::ID EvSimObjectPool::createObject( EvSimObjectFactory const& objFactory  )
{
    EvSimObject::setAllocator( _blockAllocator );
    return addObject( objFactory.create() );
}

inline int EvSimObjectPool::numOfObjects()
{
        return pool.size();
}

inline EvSimObject::ID EvSimObjectPool::getLastId()
{
    return pool.size()-1;
}

#endif /* EVSIMOBJECTPOOL_H_ */
