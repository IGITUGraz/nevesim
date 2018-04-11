#include "PLAdditiveKernelSpikeResponse.h"

EV_SIM_OBJECT_POST(PLAdditiveKernelSpikeResponse)

FIELD_REGISTRATOR(PLAdditiveKernelSpikeResponse)
{
	REGISTER_FIELD(PLAdditiveKernelSpikeResponse, r)
	REGISTER_FIELD(PLAdditiveKernelSpikeResponse, r_old)
	REGISTER_FIELD(PLAdditiveKernelSpikeResponse, slope)
}


PLAdditiveKernelSpikeResponse::~PLAdditiveKernelSpikeResponse()
{
    while ( !response_queue.empty() )
    {
        delete response_queue.top();
        response_queue.pop();
    }
};

void PLAdditiveKernelSpikeResponse::eventHit(SimContext & sim_ctxt, Time time, port_id_t port)
{
    updateState(sim_ctxt, time, port);

    if ( port == 0 )
    {
        ResponseKernel *kernel = new ResponseKernel(resp_shape_amplitudes, resp_shape_timepoints);
        kernel->trigger(time);
        r += kernel->r;
        slope += kernel->slope;
        outputEvent(sim_ctxt, port_id_t(0), kernel->next_expected_time);
        response_queue.push(kernel);
    }
    else
    {
        while ( !response_queue.empty() && ( response_queue.top()->next_expected_time == time ) )
        {
            ResponseKernel *kernel = response_queue.top();
            response_queue.pop();
            double slope_old = kernel->slope;
            kernel->update(time);            
            slope += kernel->slope - slope_old;
            
            if ( kernel->isFinished() )
            {
                r -= kernel->r;
                delete kernel;
            }
            else
            {
                response_queue.push(kernel);
                outputEvent(sim_ctxt, port_id_t(0), kernel->next_expected_time);                
            }
        }
        
        if (response_queue.empty())
        {
            r = r_old = slope = 0;
        }
    }
}

void PLAdditiveKernelSpikeResponse::reset(SimContext &sim_ctxt)
{
    while ( !response_queue.empty() )
    {
        delete response_queue.top();
        response_queue.pop();
    }
    r = r_old = slope = 0;
    cur_time = 0;
};

void PLAdditiveKernelSpikeResponse::updateState(SimContext &sim_info, Time time, update_id_t upd_id)
{
    if (time > cur_time) {
        r_old = r;
        r += (time-cur_time)*slope;
        cur_time = time;
    }
}

void PLAdditiveKernelSpikeResponse::postAddedInNetwork(EvSimNetwork &net, EvSimObject::ID self)
{
    net.connect(self, 0, self, 1);
}

PLAdditiveKernelSpikeResponse::ResponseKernel::ResponseKernel(const vector<double> * resp_amplitudes, const vector<double> * resp_timepoints)
    :r(0), slope(0),
     next_expected_time(0),
     resp_shape_amplitudes(resp_amplitudes),
     resp_shape_timepoints(resp_timepoints),
     curr_timepoint(0)
{}

PLAdditiveKernelSpikeResponse::ResponseKernel::~ResponseKernel()
{}

void PLAdditiveKernelSpikeResponse::ResponseKernel::trigger(Time time)
{
    curr_timepoint = 0;
    updateResponse(time);
}

void PLAdditiveKernelSpikeResponse::ResponseKernel::update(Time time)
{
    if ( time == next_expected_time )
    {
        curr_timepoint++;
        updateResponse(time);
    }
}

void PLAdditiveKernelSpikeResponse::ResponseKernel::updateResponse(Time time)
{
    if ((resp_shape_timepoints == 0) || curr_timepoint >= resp_shape_timepoints->size()) {
        curr_timepoint = RESPONSE_NOT_ACTIVE;
        next_expected_time = MAX_TIME;
        slope = 0;        
        return;
    }
    
    r = (*resp_shape_amplitudes)[curr_timepoint];
    
    double t_offs = (*resp_shape_timepoints)[curr_timepoint];

    if ((resp_shape_amplitudes == 0) || (curr_timepoint+1) >= resp_shape_amplitudes->size()) {
        slope = 0;
    }
    else {
        if (t_offs > 0.0)
            slope = ((*resp_shape_amplitudes)[curr_timepoint+1] - r)/t_offs;
        else
            slope = 0;
    }
    next_expected_time = time + t_offs;    
}

bool PLAdditiveKernelSpikeResponse::ResponseKernel::isFinished() const
{
    return (curr_timepoint == RESPONSE_NOT_ACTIVE);
}

