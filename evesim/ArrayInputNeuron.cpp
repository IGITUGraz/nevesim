#include "ArrayInputNeuron.h"
#include <numeric>
#include <functional>
#include "SimException.h"

#include <boost/lambda/lambda.hpp>
using namespace boost::lambda;

EV_SIM_OBJECT_POST( ArrayInputNeuron )

void ArrayInputNeuron::reset(SimContext &sim_ctxt)
{
	curr_event_idx = 0;
	if (input_events != 0 && input_events->size())
		outputEvent(sim_ctxt, port_id_t(0), (*input_events)[0]);
}

void ArrayInputNeuron::eventHit(SimContext & sim_ctxt, Time time, port_id_t port) {
	if (++curr_event_idx < input_events->size())
		outputEvent(sim_ctxt, port_id_t(0), (*input_events)[curr_event_idx]);
}


void ArrayInputNeuron::postAddedInNetwork(EvSimNetwork &net, EvSimObject::ID self) {
	net.connect(self, 0, self, 0);
}

void ArrayInputNeuron::checkValidInputEvents(const vector<Time> &inp_events)
{
	vector<double> diff(inp_events.size());
	std::adjacent_difference(inp_events.begin(), inp_events.end(), diff.begin());
	vector<double>::iterator diff_it = std::find_if(diff.begin(), diff.end(), std::bind2nd(std::less<double>(), 0));
	if (diff_it != diff.end()) {
		throw evesim::Exception("ArrayInputNeuron::checkValidInputEvents",
				                "The array of input events used to initialize an ArrayInputNeuron is not sorted.");
	}
}
