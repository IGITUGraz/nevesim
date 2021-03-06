#
# testPLResetKernelSpikeResponse.py
#
# Test functionality of PLResetKernelSpikeResponse
#
# David Kappel 11.09.2013
#

#import pylab as pl
import numpy as np
import pyevesim as ev
import unittest



class PLResetKernelSpikeResponseTest (unittest.TestCase):
    """
    AlphaSpikeResponseTest

    Runs several tests to verify the functionality of
    AlphaKernelSpikeResponseTest
    
    evesim classes involved:
    AlphaSpikeResponse, EvSimRegularClock,
    ArrayInputNeuron, NeurSamplingNetwork
    """
    
    def setUp(self):
        self.t_max = 500.0E-3
        self.dt = 5E-4
        self.dt_kernel = 0.001
        self.alpha_rise = 0.002
        self.alpha_fall = 0.020
        self.n_trials = 10        
        self.tolerance = 0.01
        self.plot_results = False

    def test_4spikes(self):
        self.run_test([2.5E-3, 8.1E-3, 20.5E-3, 300.0E-3])        

    def test_1random(self):
        for trial in range(0,self.n_trials):
            self.run_test(np.random.rand(1)*self.t_max)
            
    def test_2random(self):
        for trial in range(0,self.n_trials):
            self.run_test(np.sort(np.random.rand(2)*self.t_max))

    def test_5random(self):
        for trial in range(0,self.n_trials):
            self.run_test(np.sort(np.random.rand(5)*self.t_max))

    def test_10random(self):
        for trial in range(0,self.n_trials):
            self.run_test(np.sort(np.random.rand(10)*self.t_max))

    def test_100random(self):
        for trial in range(0,self.n_trials):
            self.run_test(np.sort(np.random.rand(100)*self.t_max))
        
    def run_test(self, inp_ev_times):

        t_max = self.t_max
        dt = self.dt
        alpha_rise = self.alpha_rise
        alpha_fall = self.alpha_fall
        
        net = ev.NeurSamplingNetwork()
        inp_nrn_id = net.create(ev.ArrayInputNeuron(), 1)
        rec_clock_id = net.create(ev.EvSimRegularClock(dt, True, 0))
        
        time_points = np.arange(0,0.2,self.dt_kernel)
        time_steps = ev.SharedDblArray(self.dt_kernel*np.ones(len(time_points)))
        amp_values = ev.SharedDblArray(np.exp(-(time_points)/alpha_fall)-np.exp(-(time_points)/alpha_rise))
        sr_proto = ev.PLResetKernelSpikeResponse()
        sr_proto.setResponseKernel(amp_values, time_steps)
        resp_id = net.create(sr_proto)
        net.connect(inp_nrn_id, 0, resp_id, 0)
        net.connect(rec_clock_id, 0, resp_id, 1)
        
        rec_r_id = net.record(resp_id, "r", rec_clock_id)
        rec_rold_id = net.record(resp_id, "r_old", rec_clock_id)        
        net.getObject(inp_nrn_id).setInputEvents(inp_ev_times)
        
        net.advance(t_max)
        
        time = np.arange(0.0,t_max,dt)
        eps_gd = np.zeros(time.shape)
        
        for ct in inp_ev_times:
            eps_gd += (time-ct>=0)*( (np.exp(-(time-ct)/alpha_fall)-np.exp(-(time-ct)/alpha_rise)) - eps_gd )
        
        rec_r_values = net.getObject(rec_r_id).getRecordedValues()
        rec_rold_values = net.getObject(rec_rold_id).getRecordedValues()
        
        #if self.plot_results:
        #    print ((rec_r_values - eps_gd)**2).sum()
        #    pl.plot(time, rec_r_values, 'b-', time, eps_gd, 'r-')            
        #    pl.show()
        
        self.assertTrue( ((rec_r_values - eps_gd)**2).sum() < (self.tolerance*len(inp_ev_times)) )


if __name__ == '__main__':
    unittest.main()

