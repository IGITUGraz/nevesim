#
# Simple example demonstrating the usage of NEVESIM
#
# Creates a exponential Poisson neuron and modulates
# its membrane potential with a sine wave. The
# resulting spike train is plotted.
#
# http://sim.igi.tugraz.at/
#
#

import numpy as np
import pyevesim as ev
import pylab as pl

# create the network object
net = ev.EvSimNetwork()

# create a exponential Poisson neuron and add it to the network
neuron_id = net.create( ev.ExpPoissonNeuron() )

# create a clock object for updating the membrane potential
clock_id = net.create( ev.EvSimRegularClock(0.001) )

# create a shared array that holds the amplitude values
vm_amplitude = 2*np.sin(2*np.arange(0,8,.001))-1
sh_amplitude = ev.SharedDblArray( vm_amplitude )

# create a modifier object that is used to update the membrane potential (Vm)
modifier_id = net.create( ev.DoubleVariableModifier() )
modifier = net.getObject( modifier_id )
modifier.setValuesArray( sh_amplitude )
modifier.setModifiedVariable(net.getObject(neuron_id),"Vm")
net.causalUpdateLink(modifier_id,neuron_id)
net.connect(clock_id,0,modifier_id,0)

# create a recorder to record the output spike times
recorder_id = net.record( neuron_id )

# simulate the network
net.simulate( 8.0 )

# get the recorded spike times
spike_times = net.getObject( recorder_id ).getRecordedValues()


pl.subplot(2,1,1)
pl.plot( np.arange(0,8.0,0.001), vm_amplitude )
pl.xlabel('time [s]')
pl.ylabel('membrane potential')

pl.subplot(2,1,2)
pl.plot( spike_times, np.zeros(spike_times.shape[0]), '|', markersize=20 )
pl.xlabel('time [s]')
pl.ylabel('output spikes')
pl.yticks([])

pl.show()

