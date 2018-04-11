#ifndef EvSimObject_H_
#define EvSimObject_H_

#include <cstring>
#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include "globals.h"
#include "EvSimObjectFactory.h"

#include <string>
using std::string;

#ifndef SWIG

#include <typeinfo>
using std::type_info;

#endif

class EvSimNetwork;
class SimContext;

class EvSimObjectTypeUtilsBase;

//! A dummy class to represent "not found" return value in the getVariableType method
class NoVariableFound {
};

//! Base class to all NEVESIM objects
/*! Provides the interface to event handling and object updating.
    All NEVESIM objects are factories, clones of which can be created by
    the \ref EvSimObjectFactory::create() method.
 */
class EvSimObject : public virtual EvSimObjectFactory {

public:
	typedef enum { eventPortDirect, eventPortProjected, portUndefined } PortType;


	typedef unsigned ID;

	typedef shared_ptr< std::vector< ID > > IDVector;

	EvSimObject();
	virtual ~EvSimObject();

	virtual void announceEvEmitted(SimContext & sim_ctxt, Time time, global_proj_out_port_id_t port_id) {};

	virtual void eventHit(SimContext & sim_ctxt, Time time, port_id_t port) {};

	virtual void updateState(SimContext &sim_ctxt, Time time, update_id_t upd_id) {};

	virtual bool owned_by_net()
	{
		return true;
	};

	virtual unsigned numProjOutputPorts() const
	{
		return 0;
	};

	virtual unsigned numOutputPorts() const
	{
		return 0;
	};


	virtual unsigned numInputPorts() const
	{
		return 0;
	};

	virtual PortType getOutputPortType(port_id_t) const
	{
		return portUndefined;
	};

	virtual PortType getInputPortType(port_id_t) const
	{
		return portUndefined;
	};

	virtual void init(EvSimNetwork & net, ID id)
	{};

	virtual void reset(SimContext &sim_ctxt)
	{};

	virtual const EvSimObjectTypeUtilsBase * getSimObjTypeUtils() const {return 0;} ;

	virtual EvSimObject *copy() const;

	EvSimObject *create(void) const;
	
	//! Sets the specified variable to the given value.
    /*! If the object has no variable named \a var_name an instance of
        \a invalid_argument is raised.
      \note This function does not perform clipping on the provided
            values. If the values don't match the numerical type of the
            variables the result is undefined.
      \param var_name the name of the variable
      \param new_value the new value of the variable
	 */
	void setVariable(const string &var_name, double new_value);
	
	//! Returns the value of the specified variable.
    /*! If the object has no variable named \a var_name an instance of
        \a invalid_argument is raised.
      \param var_name the name of the variable
	 */
	double getVariable(const string &var_name) const;

	const string & getTypeName() const;

	string getTypeNameWithPtr() const {
		return getTypeName() + " *";
	}

	virtual void postAddedInNetwork(EvSimNetwork &net, EvSimObject::ID self) {};

	virtual void postOutPortConnect(EvSimNetwork &net, port_id_t p, EvSimObject::ID dest, port_id_t dest_port) {};

	virtual void postInputPortConnect(EvSimNetwork &net, port_id_t p, EvSimObject::ID src, port_id_t src_port) {};

	virtual void postSrcCausalLink(EvSimNetwork &net, EvSimObject::ID target, update_id_t upd_id) {};

	virtual void postTargetCausalLink(EvSimNetwork &net, EvSimObject::ID src, update_id_t upd_id) {};

	virtual void postMountAsMountedObj(EvSimNetwork &net, EvSimObject::ID self, EvSimObject::ID mountpoint) {};

	virtual void postMountAsMountPoint(EvSimNetwork &net, EvSimObject::ID self, EvSimObject::ID mounted_obj) {};

	virtual bool propagatesUpdate(update_id_t upd_id) const {
		return true;
	}


#ifndef SWIG

	void * const getVariablePtr(const string &var_name) const;

	const type_info & getVariableType(const string &var_name) const;

public:
    //! Class for memory management of SimObjects
    /** The ratinale behind this memory management is, that all SimObjects belonging
    *  to one simulation thread should be in a continous block of memory to
    *  avoid false sharing of cache lines. This class approximates this requirement.
    */
    class BlockAllocator
    {
    public:
        BlockAllocator( size_t blockSize = EVSIMOBJECT_ALLOCATOR_BLOCK_SIZE );

        ~BlockAllocator();
        void *allocate( size_t size );
    private:
        size_t blk_sz;
        std::vector< void* > mem_block;
        char *free_pos;
        size_t num_free_bytes;
    };

    void static setAllocator( BlockAllocator & ba );

    void *operator new(size_t sz);
    void operator delete(void *, size_t);

    void *operator new[](size_t sz);
    void operator delete[](void *, size_t);

#ifdef DEBUG

	void setID(ID id) {
		my_id = id;
	}

	ID my_id;

#endif

private:

    static BlockAllocator defaultAllocator;
    static BlockAllocator *currentAllocator;
    //@}



#endif

};

extern const EvSimObject::ID INVALID_EVSIMOBJECT_ID;

#endif /* EvSimObject_H_ */
