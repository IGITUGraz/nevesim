#ifndef KernelSpikeResponseBase_H_
#define KernelSpikeResponseBase_H_

#include "EvSimObject.h"
#include "DirectEventSender.h"
#include "SharedArray.h"
#include "SharedArrayTypes.h"
#include "SpikeResponse.h"


class KernelSpikeResponseBase : public SpikeResponse, public DirectEventSender {

public:

    KernelSpikeResponseBase() : resp_shape_amplitudes(0), resp_shape_timepoints(0) {};

    KernelSpikeResponseBase(const SharedArray<double> &resp_shape_amplitudes,
                            const SharedArray<double> &resp_shape_timepoints)
        : resp_shape_amplitudes(resp_shape_amplitudes.get()),
          resp_shape_timepoints(resp_shape_timepoints.get())
    {}

    virtual ~KernelSpikeResponseBase() {};


    void setResponseKernel(const SharedArray<double> &resp_shape_amplitudes,
                           const SharedArray<double> &resp_shape_timepoints) {
        this->resp_shape_amplitudes = resp_shape_amplitudes.get();
        this->resp_shape_timepoints = resp_shape_timepoints.get();
    }

    virtual unsigned numInputPorts() const
    {
        return 2;
    };

    virtual unsigned numOutputPorts() const
    {
        return 1;
    };


    virtual PortType getInputPortType(port_id_t p) const
    {
        if (p < 2)
            return eventPortDirect;
        return portUndefined;
    };

    virtual PortType getOutputPortType(port_id_t p) const
    {
        if (p == 0)
            return eventPortDirect;
        return portUndefined;
    };

    virtual void reset(SimContext &sim_ctxt)
    {
        curr_timepoint = RESPONSE_NOT_ACTIVE;
        next_expected_time = MAX_TIME;
        r = r_old = 0;
    };

    virtual void postAddedInNetwork(EvSimNetwork &net, EvSimObject::ID self);

protected:
    static const unsigned RESPONSE_NOT_ACTIVE = ~(0u);

    const vector<double> * resp_shape_amplitudes;
    const vector<double> * resp_shape_timepoints;

    unsigned curr_timepoint;
    double next_expected_time;
};

#endif /* KernelSpikeResponseBase_H_ */

