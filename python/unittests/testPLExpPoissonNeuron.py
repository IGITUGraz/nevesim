#
# testPLExpPoissonNeuron.py
#
# Test functionality of PLExpPoissonNeuron
#
# David Kappel 11.09.2013
#

#import pylab as pl
import numpy as np
import pyevesim as ev
import unittest



class PLExpPoissonNeuronTest (unittest.TestCase):
    """
    PLExpPoissonNeuron

    Runs several tests to verify the functionality of
    PLExpPoissonNeuron
    
    evesim classes involved:
    PLExpPoissonNeuron, EvSimRegularClock,
    PLResetKernelSpikeResponse, PLBasicActiveSynapse,
    NeurSamplingNetwork
    """
    
    def setUp(self):
        self.t_max = 2
        self.dt = 5E-4
        self.n_trials = 10
        self.vm_tolerance = 1E-2;
        self.rate_tolerance = 1E-4;
        self.plot_results = False
        self.start_time = 0.1


    def test_fixed_mv_trace(self):
        self.run_test([0.1, 0.3, 0.4, 0.7, 1.0], [-5,  -100,  0.4,   -2,  0.1], 0.1, True, 100000, 100)

    def test_random_mv_trace(self):
        self.run_test(np.cumsum( np.random.exponential(1./20,10) ), np.random.randn(10), True, 1, 10000000)
        
    def run_test(self, time_steps, mv_trace, start_time, assert_vm, base_rate=1, n_trials=1):

        t_max = self.t_max
        dt = self.dt

        j = 0;
        time = np.arange(0.0,t_max+dt,dt)
        Vm_true = np.zeros(len(time))
        s_true = np.zeros(len(time))
        s_cur = 0
        Vm_cur = 0
        rel_time_steps = np.append( start_time, np.array(time_steps) + start_time )

        for n in range(0,len(time)):
            if (j >= len(mv_trace)) and ( time[n] > rel_time_steps[-1] ):
                Vm_cur = 0
            elif (j+1 < len(rel_time_steps)) and ( time[n] >= rel_time_steps[j] ):
                if (j+1 < len(mv_trace)):
                    s_cur = (mv_trace[j+1]-mv_trace[j])/(rel_time_steps[j+1]-rel_time_steps[j])
                else:
                    s_cur = 0
                Vm_cur = mv_trace[j]
                j += 1
            Vm_true[n] = Vm_cur
            Vm_cur += dt*s_cur            
            s_true[n] = s_cur
            
        Vm_true = 0.5*(Vm_true[1:] + Vm_true[:-1])
        rate_true = (base_rate*dt)*np.exp(Vm_true)
            
        hist = np.zeros(len(time)-1)

        net = ev.NeurSamplingNetwork(ev.SimParameters(np.random.randint(0,1000000)))

        sh_time_steps = ev.SharedDblArray(np.diff(np.append(0,time_steps)))
        sh_amp_values = ev.SharedDblArray(mv_trace)
        sr_proto = ev.PLAdditiveKernelSpikeResponse()
        sr_proto.setResponseKernel(sh_amp_values, sh_time_steps)
        
        imp_nrn_id = net.create(ev.ArrayInputNeuron([start_time]), 1)
        outp_nrn_id = net.create(ev.PLExpPoissonNeuron(base_rate, 0.0, 0.0), 1)
        rec_clock_id = net.create(ev.EvSimRegularClock(dt, True, 0))
        
        syn_id = net.connect(imp_nrn_id, outp_nrn_id,
                             ev.CompositeSynapse(ev.PLBasicActiveSynapse(1),sr_proto))

        
        if assert_vm:
            rec_vm_id = net.record(outp_nrn_id, "Vm", rec_clock_id)
            rec_slope_id = net.record(outp_nrn_id, "slope", rec_clock_id)
            net.causalUpdateLink(rec_clock_id, outp_nrn_id)
            
        rec_spikes = net.record(outp_nrn_id)

            
        for trial in range(0,n_trials):
                
            net.reset()
            net.advance(t_max)
            
            if assert_vm:
                rec_vm_values = net.getObject(rec_vm_id).getRecordedValues()
                rec_slope_values = net.getObject(rec_slope_id).getRecordedValues()
                
            rec_spike_times = net.getObject(rec_spikes).getRecordedValues()
            
            cur_hist, bin_edges = np.histogram(rec_spike_times, time)            
            hist = hist + cur_hist
            
            #if self.plot_results:
            #    pl.hold(False)
            #    pl.plot( time[:-1], hist/(trial+1), 'b-', time[:-1], rate_true, 'r-' )
            #    pl.hold(True)
            #    if assert_vm:
            #        pl.plot(time[0:len(rec_vm_values)], rec_vm_values, 'b-', time[0:len(rec_slope_values)], rec_slope_values, 'y-', time[:-1], Vm_true, 'r-' )
            #    pl.show()
            
            if assert_vm:
                self.assertTrue( ((rec_vm_values - Vm_true)**2).mean() < self.vm_tolerance )
            
        self.assertTrue( ((hist/n_trials/base_rate - rate_true/base_rate)**2).sum() < self.rate_tolerance )

if __name__ == '__main__':
    unittest.main()

