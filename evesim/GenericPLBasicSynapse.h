#ifndef GenericPLBasicSynapse_H_
#define GenericPLBasicSynapse_H_

#include "EvSimObject.h"
#include "SpikeResponse.h"
#include "EvSimObjectTypeUtils.h"
#include "Synapse.h"
#include "SimException.h"
#include "Neuron.h"

#include <iostream>


#ifndef SWIG

#include <boost/format.hpp>
using boost::str;

#endif

class PLSynapse : public Synapse {
public:

    double getSlope() {return slope;};

    virtual ~PLSynapse();

protected:
    void updatePSRSlope(double new_slope, Time time) {
        slope = new_slope;
    }

    void resetPSRSlope() {
        slope = 0;
    }

    double slope;
};


class PLActiveSynapse : public ActiveSynapse {
public:

    virtual ~PLActiveSynapse();

protected:
    inline void updatePSR(double new_psr, Time time)
    {
        psr_old = psr;
        if (time > cur_time) {
            psr_old += (time-cur_time)*slope;
            cur_time = time;
        }
        psr = new_psr;
        target_nrn->updatePSR(psr, psr - psr_old, time);
    }

    void updatePSRSlope(double new_slope, Time time)
    {
        slope_old = slope;
        slope = new_slope;
        target_nrn->updatePSRSlope(slope, slope - slope_old, time);
    }

    void resetPSRSlope()
    {
        slope = slope_old = 0;
        cur_time = 0;
    }

    double slope;
    double slope_old;
    Time cur_time;
};


template<class BaseSyn>
class GenericPLBasicSynapse : public BaseSyn {
public:

    GenericPLBasicSynapse(float W = 1)
    {
        this->W = W;
        this->syn_resp = 0;
    };

    virtual ~GenericPLBasicSynapse() {};

    double W;

    virtual void reset(SimContext &sim_ctxt)
    {
        BaseSyn::resetPSRs();
        BaseSyn::resetPSRSlope();        
    };

    virtual void setPostSynResponse(SpikeResponse *response) {
        syn_resp = response;
    }

    virtual void updateState(SimContext &sim_ctxt, Time time, update_id_t upd_id)
    {
        BaseSyn::updatePSR(W*syn_resp->get(), time);
        BaseSyn::updatePSRSlope(W*syn_resp->getSlope(), time);
    }

protected:
    SpikeResponse *syn_resp;    
};

#endif /* GenericPLBasicSynapse_H_ */
