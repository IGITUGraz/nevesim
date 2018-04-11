#ifndef EvSimNetwork_H_
#define EvSimNetwork_H_

#include "EvSimObject.h"
#include "RoutingDelayMap.h"
#include "EventTargetGroupPool.h"
#include "UpdateLinksMap.h"
#include "EvSimObjectPool.h"
#include "SimParameters.h"


#include <string>
using std::string;

class EvSimEngine;

/*! \mainpage Getting Started with NEVESIM
    - Creating and connecting objects is handled by the \ref EvSimNetwork class.
      This class provides the basic functionality for running a simulation.
      
    - All objects in NEVESIM are derived from the \ref EvSimObject class which
      provides the interface to event management.

    - For a concrete neural network implementation see the \ref Neuron class.
      
    - \ref EvSimEngine implements the core functionality of NEVESIM for managing
      the events triggered throughout the simulation.
      
    - See also the <a href="http://wiki.tugraz.at/igi/sim/" target=about:blank>
      EVESIM wiki page</a> for example usage.
 */
 
 
//! Basic network class that handles creating and connecting NEVESIM objects.
/*! Creates object, like neurons and synapses, places them in the network and
    connects them.
 */
class EvSimNetwork {

public:
	EvSimNetwork(SimParameters sim_params = SimParameters());

	virtual ~EvSimNetwork();

    //! Creates a new object and places it in the network
    /*! The \ref EvSimObjectFactory::create() function is used
        to create the object.
        \returns The id of the generated object.
     */
	EvSimObject::ID create(const EvSimObjectFactory & obj_factory);

    //! Adds the object in to the network
	EvSimObject::ID add(EvSimObject * obj);

    //! Creates multiple objects and places them in the network
	EvSimObject::IDVector create(const EvSimObjectFactory & obj_factory, unsigned n);

    /*! Connects am output port of object \a src to an input port of object \a dest
        \param src The object ID of the source object
        \param src_port The output port of the source object
        \param dest The object ID of the destination object
        \param dest_port The input port of the destination object
        \param delay 
     */
	void connect(EvSimObject::ID src, port_id_t src_port, EvSimObject::ID dest, port_id_t dest_port, Time delay = 0);

    //! Creates a causal update link between object \a src and object \a dest
	void causalUpdateLink(EvSimObject::ID src, EvSimObject::ID dest, update_id_t update_id = 0);

	void mount(EvSimObject::ID obj_id, EvSimObject::ID mountpoint);

	EvSimObject::ID mount(const EvSimObjectFactory &obj_factory, EvSimObject::ID mountpoint);

	EvSimObject::ID record(EvSimObject::ID id, const string &var_name);
	EvSimObject::ID record(EvSimObject::ID id, const string &var_name, EvSimObject::ID clock_id);

	EvSimObject::ID record(EvSimObject::ID id, port_id_t port_id = 0);

	EvSimObject::IDVector record(EvSimObject::IDVector id_vec, const string &var_name);
	EvSimObject::IDVector record(EvSimObject::IDVector id_vec, const string &var_name, EvSimObject::ID clock_id);

	EvSimObject::IDVector record(EvSimObject::IDVector id_vec, port_id_t port_id = 0);

	void setParams(SimParameters sim_params);

	EvSimObject * getObject(EvSimObject::ID oid) const;

	Time getCurrentSimTime();

	void advance(Time sim_time);

	void simulate(Time sim_time);

	void init();

	void reset();


	void setVariables(EvSimObject::IDVector id_vec, const string &var_name, const std::vector<double> &values );

	std::vector<double> getVariables(EvSimObject::IDVector id_vec, const string &var_name ) const;

	void connect(EvSimObject::IDVector src, port_id_t src_port, EvSimObject::IDVector dest, port_id_t dest_port, Time delay = 0);

protected:
	friend class EvSimEngine;

	void registerOutPorts(EvSimObject *obj);

	EvSimEngine *simengine;

	UpdateLinksMap upd_links_map;

	EvSimObjectPool obj_pool;

	RoutingDelayMap delay_map;

	EventTargetGroupPool etg_pool;

	EvSimObject::ID default_sim_clock_id;

	SimParameters sim_params;

	bool isInitialized;
	bool isReset;
};


#endif /*EvSimNetwork_H_*/
