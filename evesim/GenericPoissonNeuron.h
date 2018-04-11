#ifndef GENERICPOISSONNEURON_H_
#define GENERICPOISSONNEURON_H_

#include "EvSimObject.h"
#include "ProjectedEventSender.h"
#include "DirectEventSender.h"
#include "SpikeResponse.h"
#include "Synapse.h"
#include "EvSimObjectTypeUtils.h"
#include "RandomDistribution.h"
#include "Neuron.h"

#include <string>
using std::string;


class PoissonNeuronBase : public Neuron, public SingleOutputProjectedEventSender  {
public:
	PoissonNeuronBase();
	virtual ~PoissonNeuronBase();

	virtual void updateState(SimContext &sim_context, Time time, update_id_t upd_id) {};

	virtual unsigned numProjOutputPorts() const
	{
		return 1;
	};

	virtual unsigned numOutputPorts() const
	{
		return 1;
	};


	virtual PortType getOutputPortType(port_id_t) const
	{
		return eventPortProjected;
	};


	virtual unsigned registerInputSynapse(EvSimObject *input_synapse) { return 0; };

	virtual void announceEvEmitted(SimContext &sim_ctxt, Time time, global_out_port_id_t port_id);

protected:
	double Vm;
	Time lastSpike;
	bool in_refractory;
};


template <class Base >
class GenericPoissNeuronRecalcVm : public Base {
public:

	virtual ~GenericPoissNeuronRecalcVm() {};

	virtual unsigned registerInputSynapse(EvSimObject *input_synapse)
	{
		post_syn_potentials.push_back(static_cast<float *>(dynamic_cast<EvSimObject *>(input_synapse)->getVariablePtr("psr")));
		return post_syn_potentials.size() - 1;
	}

protected:
	struct Accum {
		Accum() : s(0) {}

		inline void operator()(float *p) {s+=*p;}

		double s;
	};

	inline void updateVm(Time time)
	{
		Accum accumulate;
		std::for_each(post_syn_potentials.begin(), post_syn_potentials.end(), accumulate);
		Base::Vm = accumulate.s;
	}

	vector<float *> post_syn_potentials;
};


template <class Base >
class GenericPoissNeuronFast : public Base {
public:
	virtual ~GenericPoissNeuronFast() {};

	virtual void updatePSR(double psr, double psr_diff, Time time)
	{
		Base::Vm += psr_diff;
	}

	virtual bool requiresActiveSynapse() {
		return true;
	}

	inline void updateVm(Time time) {};
};


template <class Base >
class GenericExpPoissonNeuron : public Base {
public:

	virtual ~GenericExpPoissonNeuron() {};

	float C;

	float A;

	float bias;

	virtual void reset(SimContext &sim_ctxt)
	{
		Base::reset(sim_ctxt);
		Base::Vm = 0;
	};

protected:

	inline double firingRate()
	{
		return A*exp(C*(Base::Vm+bias));
	}
	
	void recalcNextSpikeTime(SimContext &sim_info, Time time)
	{
		Time next_spike_time;
		double rate = firingRate();
		if (rate == 0) {
			next_spike_time = MAX_TIME;
		}
		else {
			ExponentialDistribution exp_dist(rate);
			next_spike_time = exp_dist( *global_sim_random_eng );
		}
		Base::changeProjectedEventTime(sim_info, (port_id_t)0, time + next_spike_time);
	};
};

// to be implemented
template <class Base>
class GenericGenPoissonNeuron : public Base {
public:

	virtual ~GenericGenPoissonNeuron();

	float C;

	virtual void reset(SimContext &sim_ctxt)
	{
		Base::Vm = 0;
	};

protected:

	inline double firingRate()
	{
		return log(C*Base::Vm);
	};
	
	void recalcNextSpikeTime(SimContext &sim_info, Time time)
	{
		Time next_spike_time;
		double rate = firingRate();
		if (rate == 0) {
			next_spike_time = MAX_TIME;
		}
		else {
			ExponentialDistribution exp_dist(rate);
			next_spike_time = exp_dist( *global_sim_random_eng );
		}
		Base::changeProjectedEventTime(sim_info, (port_id_t)0, time + next_spike_time);
	};
};


template <class Base>
class GenericPoissonNeuronComplete : public Base
{
public:

	virtual ~GenericPoissonNeuronComplete() {};

	virtual void reset(SimContext &sim_ctxt)
	{
		Base::reset(sim_ctxt);
		Base::lastSpike = -Trefract-1;
		Base::in_refractory = false;
		Base::updateVm(0);
		Base::recalcNextSpikeTime(sim_ctxt, 0);
	};

	virtual void updateState(SimContext &sim_ctxt, Time time, update_id_t upd_id)
	{
		Base::updateVm(time);
		if (!Base::in_refractory) {
			Base::recalcNextSpikeTime(sim_ctxt, time);
		}
	}

	virtual void postAddedInNetwork(EvSimNetwork &net, EvSimObject::ID self){
		net.connect(self, 0, self, 0, Trefract);
	}

	virtual void eventHit(SimContext & sim_ctxt, Time time, port_id_t port) {
		Base::in_refractory = false;
		Base::recalcNextSpikeTime(sim_ctxt, time);
	}

	float Trefract;
};

#endif /* GENERICPOISSONNEURON_H_ */
