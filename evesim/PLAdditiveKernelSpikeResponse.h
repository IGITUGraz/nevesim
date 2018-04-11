#ifndef AdditiveKernelSpikeResponseBase_H_
#define AdditiveKernelSpikeResponseBase_H_

#include "EvSimObject.h"
#include "DirectEventSender.h"
#include "SharedArray.h"
#include "SharedArrayTypes.h"
#include "SpikeResponse.h"
#include "EvSimObjectTypeUtils.h"

#include <queue>
#include <vector>

//! Piecewise linear additive spike response
/*! The shape of the spike response is defined by an array of amplitudes
    \f$(a_1,...,a_N)\f$ and an array of time spans \f$(d_1,...,d_N)\f$.
    The PSP amplitude at time \f$t_1 = 0\f$ after a spike was triggered is
    \f$a_1\f$, that at time \f$t_n = d_1 + d_2 + ... + d_{n-1}\f$ is
    \f$a_n\f$. The PSP amplitudes are linearly interpolated between \f$t_{n}\f$
    and \f$t_{n+1}\f$. The PSP amplitudes between \f$t_{N}\f$ and \f$t_{N+1}\f$
    are constant \f$a_{N}\f$. If a spike occurs while the response is active
    the overlapping spike responses are added up.
    \see PLResetKernelSpikeResponse
 */
class PLAdditiveKernelSpikeResponse : public SpikeResponse, public DirectEventSender {

    EV_SIM_OBJECT( PLAdditiveKernelSpikeResponse )

public:
    //! Default Constructor: initializes the object without a response kernel
    /*! Use the setResponseKernel() to set up the response kernel
        \see setResponseKernel()
     */
    PLAdditiveKernelSpikeResponse() : resp_shape_amplitudes(0), resp_shape_timepoints(0) {};

    //! Constructs a spike response with given response kernel
    /*! \param resp_shape_amplitudes A shared array holding the amplitude values
        \param resp_shape_timepoints A shared array holding the time span of the
                                    given amplitude value
     */
    PLAdditiveKernelSpikeResponse(const SharedArray<double> &resp_shape_amplitudes,
                                  const SharedArray<double> &resp_shape_timepoints)
        : resp_shape_amplitudes(resp_shape_amplitudes.get()),
          resp_shape_timepoints(resp_shape_timepoints.get())
    {}

    //! Destructor: deletes all active spike responses
    virtual ~PLAdditiveKernelSpikeResponse();

    //! Sets the response kernel
    /*! \param resp_shape_amplitudes A shared array holding the amplitude values
        \param resp_shape_timepoints A shared array holding the time span of the
                                     given amplitude value
     */
    void setResponseKernel(const SharedArray<double> &resp_shape_amplitudes,
                           const SharedArray<double> &resp_shape_timepoints)
    {                           
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

    virtual void reset(SimContext &sim_ctxt);

    virtual double getSlope() {return slope;};

    virtual void postAddedInNetwork(EvSimNetwork &net, EvSimObject::ID self);
    
    virtual void eventHit(SimContext & sim_info, Time time, port_id_t port);

    virtual void updateState(SimContext &sim_info, Time time, update_id_t upd_id);

protected:
    class ResponseKernel {

    public:
        ResponseKernel(const vector<double> * resp_amplitudes, const vector<double> * resp_shape_timepoints);
        
        ~ResponseKernel();
        
        void trigger(Time time);
        
        void update(Time time);
        
        bool isFinished() const;

        double r;
        double slope;
        Time next_expected_time;
        
    protected:
        static const unsigned RESPONSE_NOT_ACTIVE = ~(0u);
        
        void updateResponse(Time time);
        
        const vector<double> * resp_shape_amplitudes;
        const vector<double> * resp_shape_timepoints;
        unsigned curr_timepoint;
    };

	class CompareResponseKernel {
	public:
		bool operator()(ResponseKernel const * left, ResponseKernel const * right) const {
			return left->next_expected_time > right->next_expected_time;
		}
	};

    std::priority_queue<ResponseKernel*, std::vector<ResponseKernel*>, CompareResponseKernel > response_queue;
    
    const vector<double> * resp_shape_amplitudes;
    const vector<double> * resp_shape_timepoints;

    Time cur_time;
    double slope;
};

#endif /* AdditiveKernelSpikeResponseBase_H_ */

