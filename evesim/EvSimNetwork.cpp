#include "EvSimNetwork.h"

#include "globals.h"
#include "DirectEventSender.h"
#include "SimException.h"
#include "VariableRecorder.h"
#include "EventTimesRecorder.h"
#include "EvSimRegularClock.h"
#include "SimException.h"
#include "EvSimEngine.h"
#include "EvSimObjectTypeUtils.h"
#include "RecorderSelector.h"

#include <boost/format.hpp>
#include <string>
using std::string;

EvSimNetwork::EvSimNetwork(SimParameters sim_params)
        : sim_params(sim_params), isInitialized(false), isReset(false)
{
        simengine = new EvSimEngine(*this, obj_pool, delay_map, etg_pool, upd_links_map);
        EvSimRegularClock clock(sim_params.default_clock_tick);
        default_sim_clock_id = create(clock);
}

EvSimNetwork::~EvSimNetwork()
{
    delete simengine;
}


EvSimObject::ID EvSimNetwork::add(EvSimObject * obj)
{
    EvSimObject::ID oid = obj_pool.addObject(obj);
    delay_map.registerOutPorts(obj);
    simengine->registerProjectedOutputPorts(obj);
    obj->postAddedInNetwork(*this, oid);
    return oid;
}

EvSimObject::ID EvSimNetwork::create(const EvSimObjectFactory & obj_factory)
{
    EvSimObject::ID oid = obj_pool.createObject(obj_factory);
    EvSimObject *obj= obj_pool.getObject(oid);

#ifdef DEBUG
    obj->setID(oid);
#endif

    delay_map.registerOutPorts(obj);
    simengine->registerProjectedOutputPorts(obj);
    obj->postAddedInNetwork(*this, oid);
    return oid;
}

EvSimObject::IDVector EvSimNetwork::create(const EvSimObjectFactory & obj_factory, unsigned n)
{
    EvSimObject::IDVector obj_ids(new std::vector<EvSimObject::ID>);
    for (unsigned i = 0 ; i < n ; ++i) {
        EvSimObject::ID oid = create(obj_factory);
        obj_ids->push_back(oid);
    }
    return obj_ids;
}

EvSimObject * EvSimNetwork::getObject(EvSimObject::ID oid) const
{
    return obj_pool.getObject(oid);
}

void EvSimNetwork::setParams(SimParameters sim_params)
{
    this->sim_params = sim_params;
    simengine->setRNDSeed(sim_params.rnd_seed);
    EvSimRegularClock *clock = dynamic_cast<EvSimRegularClock *>(getObject(default_sim_clock_id));
    clock->setTickInterval(sim_params.default_clock_tick);
}



EvSimObject::ID EvSimNetwork::record(EvSimObject::ID id, port_id_t port_id) {
    EventTimesRecorder rec_prototype;
    EvSimObject::ID rec_id = create(rec_prototype);
    connect(id, port_id, rec_id, 0);
    return rec_id;
}

EvSimObject::ID EvSimNetwork::record(EvSimObject::ID id, const string &var_name) {
    return record(id, var_name, default_sim_clock_id);
}


EvSimObject::ID EvSimNetwork::record(EvSimObject::ID id, const string &var_name, EvSimObject::ID clock_id) {
    EvSimObject *obj = obj_pool.getObject(id);
    VariableRecorder & rec = recorderSelector.selectAppropriateRecorder(obj->getVariableType(var_name));
    EvSimObject::ID rec_id = create(rec);
    VariableRecorder *rec_obj = dynamic_cast<VariableRecorder *>(obj_pool.getObject(rec_id));
    rec_obj->setRecordedSimObject(obj, var_name);
    connect(clock_id, 0, rec_id, 0);
    return rec_id;
}

EvSimObject::IDVector EvSimNetwork::record(EvSimObject::IDVector id_vec, port_id_t port_id)
{
    EvSimObject::IDVector::element_type::const_iterator id_it = id_vec->begin();
    EvSimObject::IDVector rec_id_vec(new EvSimObject::IDVector::element_type());
    for (;id_it != id_vec->end(); ++id_it) {
        rec_id_vec->push_back(record(*id_it, port_id));
    }
    return rec_id_vec;
}


EvSimObject::IDVector EvSimNetwork::record(EvSimObject::IDVector id_vec, const string &var_name)
{
    return record(id_vec, var_name, default_sim_clock_id);
}

EvSimObject::IDVector EvSimNetwork::record(EvSimObject::IDVector id_vec, const string &var_name, EvSimObject::ID clock_id)
{
    EvSimObject::IDVector::element_type::const_iterator id_it = id_vec->begin();
    EvSimObject::IDVector rec_id_vec(new EvSimObject::IDVector::element_type());
    for (;id_it != id_vec->end(); ++id_it) {
        rec_id_vec->push_back(record(*id_it, var_name, clock_id));
    }
    return rec_id_vec;
}



void EvSimNetwork::setVariables(EvSimObject::IDVector id_vec, const string &var_name, const std::vector<double> &values )
{
    EvSimObject::IDVector::element_type::const_iterator id_it = id_vec->begin();
        std::vector<double>::const_iterator val_it = values.begin();
    for (;id_it != id_vec->end(); ++id_it)
    {
        obj_pool.getObject(*id_it)->setVariable(var_name,*val_it);
        val_it++;
    }
}


std::vector<double> EvSimNetwork::getVariables(EvSimObject::IDVector id_vec, const string &var_name) const
{
    EvSimObject::IDVector::element_type::const_iterator id_it = id_vec->begin();
    std::vector<double> values;
    for (;id_it != id_vec->end(); ++id_it)
    {
        values.push_back( obj_pool.getObject(*id_it)->getVariable(var_name) );
    }
    return values;
}


void EvSimNetwork::connect(EvSimObject::IDVector src, port_id_t src_port, EvSimObject::IDVector dst, port_id_t dst_port, Time delay)
{
    EvSimObject::IDVector::element_type::const_iterator src_it = src->begin();
    EvSimObject::IDVector::element_type::const_iterator dst_it = dst->begin();
    for (;src_it != src->end(); ++src_it) {
        connect(*src_it,src_port,*dst_it,dst_port,delay);
        ++dst_it;
    }
}


void EvSimNetwork::init()
{
    if (!isInitialized) {

        // Initialize the structures in the simengine
        simengine->init();

        // initialize all objects
        obj_pool.initObjects(*this);

        isInitialized = true;
    }
}


void EvSimNetwork::reset()
{
    if (!isReset) {
        init();
        simengine->reset();
        isReset = true;
    }
}


void EvSimNetwork::simulate(Time sim_time) {
    reset();
    simengine->simulate(sim_time);
}

void EvSimNetwork::advance(Time sim_time) {
    reset();
    simengine->advance(sim_time);
}


void EvSimNetwork::connect(EvSimObject::ID src, port_id_t src_port,
                              EvSimObject::ID dest, port_id_t dest_port, Time delay)
{
    EvSimObject *src_obj = obj_pool.getObject(src);
    DirectEventSender *sender = dynamic_cast<DirectEventSender *>(src_obj);
    if (sender == 0) {
        throw evesim::ConstructionException("EvSimNetwork::connect",
                         boost::str( boost::format("Cannot connect source sim object (%d,%s) to another object via port %d because it is not of type DirectEventSender." ) % src % typeid(*src_obj).name() % src_port) );
    }
    global_out_port_id_t global_src_port_id = sender->getOutPortGlobalID(src_port);
    if (global_src_port_id == PORT_NOT_DEFINED) {
        throw evesim::ConstructionException("EvSimNetwork::connect",
                                 boost::str( boost::format("Output port %d not defined in sim object (%d,%s)." ) % src_port % src % typeid(*src_obj).name() ) );
    }
    event_target_group_id_t etg = delay_map.find(global_src_port_id, delay);
    EvSimObject *dest_obj = obj_pool.getObject(dest);
    if (etg == no_event_target_group) {
        etg = etg_pool.createEventTargetGroup(dest_obj, dest_port);
        delay_map.insert(global_src_port_id, delay, etg);
    }
    else {
        etg = etg_pool.addEventTarget(etg, dest_obj, dest_port);
    }
    src_obj->postOutPortConnect(*this, src_port, dest, dest_port);
    dest_obj->postInputPortConnect(*this, dest_port, src, src_port);
}

void EvSimNetwork::causalUpdateLink(EvSimObject::ID src, EvSimObject::ID dest, update_id_t update_id)
{
    EvSimObject *src_obj = obj_pool.getObject(src);
    EvSimObject *dest_obj = obj_pool.getObject(dest);
    upd_links_map[src_obj].push_back(UpdateNode(dest_obj, update_id));
    src_obj->postSrcCausalLink(*this, dest, update_id);
    dest_obj->postTargetCausalLink(*this, src, update_id);
}

void EvSimNetwork::mount(EvSimObject::ID obj_id, EvSimObject::ID mountpoint)
{
    EvSimObject *obj = obj_pool.getObject(obj_id);
    EvSimObject *mountpoint_obj = obj_pool.getObject(obj_id);
    obj->postMountAsMountedObj(*this, obj_id, mountpoint);
    mountpoint_obj->postMountAsMountPoint(*this, mountpoint, obj_id);
}

EvSimObject::ID EvSimNetwork::mount(const EvSimObjectFactory &obj_factory, EvSimObject::ID mountpoint)
{
    EvSimObject::ID obj_id = create(obj_factory);
    mount(obj_id, mountpoint);
    return obj_id;
}

Time EvSimNetwork::getCurrentSimTime()
{
    return simengine->curr_sim_clock;
}
