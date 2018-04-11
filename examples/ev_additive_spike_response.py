#
# Example demonstrating the usage of additive spike
# responses within NEVESIM
#
# Creates a exponential Poisson neuron and modulates
# its membrane potential with a sine wave. The
# resulting spike train is plotted.
#
# http://sim.igi.tugraz.at/
#
#

import pylab as pl
import numpy as np
import pyevesim as ev
import matplotlib.gridspec as gridspec
import time

# create the network
net = ev.NeurSamplingNetwork()

# create 20 input neurons with 1Hz Poisson rate
num_in = 20
in_nrn_ids = net.create(ev.PLExpPoissonNeuron(1, 1.0, 0.0), num_in)

# create output neuron with 50Hz Poisson rate and a (soft) threshold at 3
vm_thresh = 3.0
outp_nrn_id = net.create(ev.PLExpPoissonNeuron(50, -vm_thresh, 0.0), 1)

# create a double exponential spike response with 15 piecewise linear
# interpolation points
alpha_rise = 0.002
alpha_fall = 0.020
time_points = np.array([.0000, .0020, .0040, .0050, .0060,
                        .0080, .0120, .0180, .0300, .0400,
                        .0500, .0700, .0900, .1250, .1750]);
amp_values = ev.SharedDblArray(np.exp(-(time_points[:-1])/alpha_fall)-np.exp(-(time_points[:-1])/alpha_rise))
time_steps = ev.SharedDblArray(np.diff(time_points))
sr_proto = ev.PLAdditiveKernelSpikeResponse(amp_values, time_steps)

# create synapses between input and output neurons
syn_proto = ev.CompositeSynapse(ev.PLBasicActiveSynapse(2),sr_proto)
syn_id = net.connectAll2All(in_nrn_ids, outp_nrn_id, syn_proto)

# create recorder for the input spike times     
rec_spikes_in = net.record(in_nrn_ids)

# create recorder for output neuron membrane potential
rec_clock_id = net.create(ev.EvSimRegularClock(.0001, True, .0001))
rec_vm_id = net.record(outp_nrn_id, "Vm", rec_clock_id)
net.causalUpdateLink(rec_clock_id, outp_nrn_id)

# create recorder for the output spike times     
rec_spikes_out = net.record(outp_nrn_id)

# simulate the network for 1 second
sim_start_time = time.time()
net.simulate(1)
sim_end_time = time.time()

print "simulation took %f seconds" % (sim_end_time - sim_start_time)

# get recorded values
rec_vm_values = net.getObject(rec_vm_id).getRecordedValues()
rec_spike_times = net.getObject(rec_spikes_out).getRecordedValues()

gs = gridspec.GridSpec(3, 1, height_ratios=[8,8,1])

pl.subplot(gs[0])
for n in range(0,len(rec_spikes_in)):
    rec_in_spike_times = net.getObject(rec_spikes_in[n]).getRecordedValues()
    pl.hold(True)
    pl.plot(rec_in_spike_times, np.ones(len(rec_in_spike_times))*n, 'b|')
    
pl.xlim([0,1])
pl.ylim([-1,num_in])
pl.yticks([])
pl.ylabel('input')

pl.subplot(gs[1])
pl.plot(np.arange(0,1,.0001),rec_vm_values,'b-')
pl.hold(True)
pl.plot([0,1],[vm_thresh,vm_thresh],'r-')
pl.xlim([0,1])
pl.ylabel('membrane potential')

pl.subplot(gs[2])
pl.plot(rec_spike_times,np.ones(len(rec_spike_times)),'b|')
pl.yticks([])
pl.xlim([0,1])
pl.xlabel('time')
pl.ylabel('output')

pl.show()


