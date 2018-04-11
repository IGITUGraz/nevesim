#ifndef GENERICPLPOISSONNEURON_H_
#define GENERICPLPOISSONNEURON_H_

#include "EvSimObject.h"
#include "ProjectedEventSender.h"
#include "DirectEventSender.h"
#include "SpikeResponse.h"
#include "Synapse.h"
#include "EvSimObjectTypeUtils.h"
#include "RandomDistribution.h"
#include "GenericPoissonNeuron.h"
#include "Neuron.h"

#include <limits>


template <class Base >
class GenericPLPoissNeuronRecalcVm : public Base {
public:

    virtual ~GenericPLPoissNeuronRecalcVm() {};

    virtual unsigned registerInputSynapse(EvSimObject *input_synapse)
    {
        post_syn_potentials.push_back(static_cast<double *>(dynamic_cast<EvSimObject *>(input_synapse)->getVariablePtr("psr")));
        double * slope_ptr = static_cast<double *>(dynamic_cast<EvSimObject *>(input_synapse)->getVariablePtr("slope"));
        if ( slope_ptr )
            post_syn_slopes.push_back(slope_ptr);
        return post_syn_potentials.size() - 1;
    }
    
    virtual void reset(SimContext &sim_ctxt)
    {
        Base::reset(sim_ctxt);
        Base::Vm = 0;
        slope = 0;
        cur_time = 0;
    };

protected:
    struct Accum {
        Accum() : s(0) {}

        inline void operator()(double *p) {s+=*p;}

        double s;
    };

    inline void updateVm(Time time)
    {
        if (time > cur_time) {
            Accum accumulate_vm;
            Accum accumulate_slope;
            std::for_each(post_syn_potentials.begin(), post_syn_potentials.end(), accumulate_vm);
            std::for_each(post_syn_slopes.begin(), post_syn_slopes.end(), accumulate_slope);
            Base::Vm = accumulate_vm.s;
            slope = accumulate_slope.s;
            cur_time = time;
        }
    }

    vector<double *> post_syn_potentials;
    vector<double *> post_syn_slopes;
    double slope;
    Time cur_time;
};


template <class Base >
class GenericPLPoissNeuronFast : public Base {
public:
    virtual ~GenericPLPoissNeuronFast() {};

    virtual void updatePSR(double psr, double psr_diff, Time time)
    {
        updateVm(time);
        Base::Vm += psr_diff;
    }
    
    virtual void updatePSRSlope(double psr_slope, double psr_slope_diff, Time time)
    {
        updateVm(time);
        slope += psr_slope_diff;
    }

    virtual bool requiresActiveSynapse() {
        return true;
    }

    inline void updateVm(Time time) {
        if (time > cur_time) {
            Base::Vm += (time-cur_time)*slope;
            cur_time = time;
        }
    };
    
    virtual void reset(SimContext &sim_ctxt)
    {
        Base::reset(sim_ctxt);
        Base::Vm = 0;
        slope = 0;
        cur_time = 0;
    };
    
protected:
    double slope;
    Time cur_time;
};


template <class Base >
class GenericPLExpPoissonNeuron : public Base {
public:

    virtual ~GenericPLExpPoissonNeuron() {};

    float C;

    float A;

    float bias;

protected:
    inline double firingRate()
    {
        return A*exp(C*(Base::Vm+bias));
    };
    
	void recalcNextSpikeTime(SimContext &sim_info, Time time)
	{
        Time next_spike_time = MAX_TIME;
        Base::updateVm(time);
        double rate = firingRate();
        if (rate > 0) {
            ExponentialDistribution exp_dist(1.0);
            double exp_dist_sample = exp_dist( *global_sim_random_eng );
            double next_time_projected = Base::slope/rate*exp_dist_sample;
            if (next_time_projected > (-1 + std::numeric_limits<double>::epsilon()) ) {
                if (fabs(next_time_projected) <= std::numeric_limits<double>::epsilon())
                    next_spike_time = Base::cur_time + exp_dist_sample/rate;
                else
                    next_spike_time = Base::cur_time + (1/Base::slope)*log( next_time_projected + 1 );
            }
        }        
        Base::changeProjectedEventTime(sim_info, (port_id_t)0, next_spike_time);
    };
};

#endif /* GENERICPLPOISSONNEURON_H_ */

