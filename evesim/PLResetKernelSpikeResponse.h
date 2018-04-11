#ifndef PLResetKernelSpikeResponse_H_
#define PLResetKernelSpikeResponse_H_

#include "EvSimObject.h"
#include "DirectEventSender.h"
#include "SharedArray.h"
#include "SharedArrayTypes.h"
#include "KernelSpikeResponseBase.h"
#include "EvSimObjectTypeUtils.h"

//! Piecewise linear resetting spike response
/*! The shape of the spike response is defined by an array of amplitudes
    \f$(a_1,...,a_N)\f$ and an array of time spans \f$(d_1,...,d_N)\f$.
    The PSP amplitude at time \f$t_1 = 0\f$ after a spike was triggered is
    \f$a_1\f$, that at time \f$t_n = d_1 + d_2 + ... + d_{n-1}\f$ is
    \f$a_n\f$. The PSP amplitudes are linearly interpolated between \f$t_{n}\f$
    and \f$t_{n+1}\f$. The PSP amplitudes between \f$t_{N}\f$ and \f$t_{N+1}\f$
    are constant \f$a_{N}\f$. If a spike occurs while the response is active
    the spike response is reset and triggered at the new spike event.
    \see PLAdditiveKernelSpikeResponse
    \see ResetKernelSpikeResponse
 */
class PLResetKernelSpikeResponse : public KernelSpikeResponseBase {

    EV_SIM_OBJECT( PLResetKernelSpikeResponse )

public:
    //! Default Constructor: initializes the object without a response kernel
    /*! Use the setResponseKernel() to set up the response kernel
        \see setResponseKernel()
     */
    PLResetKernelSpikeResponse() {};

    //! Constructs a spike response with given response kernel
    /*! /param resp_shape_amplitudes A shared array holding the amplitude values
        /param resp_shape_timepoints A shared array holding the time span of the
                                     given amplitude value
     */
    PLResetKernelSpikeResponse(const SharedArray<double> &resp_shape_amplitudes,
                               const SharedArray<double> &resp_shape_timepoints)
        : KernelSpikeResponseBase(resp_shape_amplitudes, resp_shape_timepoints)
    {}

    virtual ~PLResetKernelSpikeResponse() {};

    virtual void eventHit(SimContext & sim_info, Time time, port_id_t port);

    virtual void updateState(SimContext &sim_info, Time time, update_id_t upd_id);

    virtual double getSlope() { return slope; };
    
    virtual void reset(SimContext &sim_ctxt)
    {
        KernelSpikeResponseBase::reset( sim_ctxt );
        slope = 0;
        cur_time = 0;
    };

protected:
    double slope;
    Time cur_time;
};

#endif /* PLResetKernelSpikeResponse_H_ */

