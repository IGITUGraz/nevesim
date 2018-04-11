#
# testPLAdditiveKernelSpikeResponse.py
#
# Test functionality of PLAdditiveKernelSpikeResponse
#
# David Kappel 30.07.2013
#

#import pylab as pl
import numpy as np
import pyevesim as ev
import unittest



class PLAdditiveKernelSpikeResponseTest (unittest.TestCase):
    """
    PLAdditiveKernelSpikeResponseTest

    Runs several tests to verify the functionality of
    PLAdditiveKernelSpikeResponseTest
    
    evesim classes involved:
    PLAdditiveKernelSpikeResponseTest, SharedDblArray,
    ArrayInputNeuron, EvSimRegularClock, NeurSamplingNetwork
    """
    
    def setUp(self):
        self.t_max = 500.0E-3
        self.dt = 1E-4
        self.dt_kernel = 0.001
        self.alpha_rise = 0.002
        self.alpha_fall = 0.020
        self.n_trials = 10        
        self.tolerance = 0.05
        self.plot_results = False

    def test_4_spikes(self):
        self.run_test([2.5E-3, 8.1E-3, 20.5E-3, 300.0E-3])        

    def test_1_random(self):
        for trial in range(0,self.n_trials):
            self.run_test([0.1])
            
    def test_2_random(self):
        for trial in range(0,self.n_trials):
            self.run_test(np.sort(np.random.rand(2)*self.t_max))

    def test_5_random(self):
        for trial in range(0,self.n_trials):
            self.run_test(np.sort(np.random.rand(5)*self.t_max))

    def test_10_random(self):
        for trial in range(0,self.n_trials):
            self.run_test(np.sort(np.random.rand(10)*self.t_max))

    def test_100_random(self):
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
        
        #time_points = np.arange(0,0.2,self.dt_kernel)
        time_points = np.array([0.0000, 0.0010, 0.0020, 0.0030, 0.0040, 0.0045, 0.0050, 0.0055, 0.0060, 0.0070,
                                0.0080, 0.0090, 0.0120, 0.0150, 0.0180, 0.0240, 0.0300, 0.0350, 0.0400, 0.0450,
                                0.0500, 0.0600, 0.0700, 0.0800, 0.0900, 0.1000, 0.1250, 0.1500, 0.1750, 0.2000]);
        time_steps = ev.SharedDblArray(np.diff(time_points))
        amp_values = ev.SharedDblArray(np.exp(-(time_points[:-1])/alpha_fall)-np.exp(-(time_points[:-1])/alpha_rise))
        
        sr_proto = ev.PLAdditiveKernelSpikeResponse()
        sr_proto.setResponseKernel(amp_values, time_steps)
        resp_id = net.create(sr_proto)
        
        net.connect(inp_nrn_id, 0, resp_id, 0)
        net.causalUpdateLink(rec_clock_id, resp_id);
        
        rec_r_id = net.record(resp_id, "r", rec_clock_id)
        rec_rold_id = net.record(resp_id, "r_old", rec_clock_id)        
        net.getObject(inp_nrn_id).setInputEvents(inp_ev_times)
        
        net.advance(t_max)
        
        time = np.arange(0.0,t_max,dt)
        eps_gd = np.zeros(time.shape)
        
        for ct in inp_ev_times:
            eps_gd += (time-ct>=0)*(np.exp(-(time-ct)/alpha_fall)-np.exp(-(time-ct)/alpha_rise))
        
        rec_r_values = net.getObject(rec_r_id).getRecordedValues()
        rec_rold_values = net.getObject(rec_rold_id).getRecordedValues()
        
        #if self.plot_results:
        #    print ((rec_r_values[0:len(eps_gd)] - eps_gd)**2).sum()
        #    pl.plot(time, rec_r_values[0:len(time)], 'b.-', time, eps_gd, 'r-')            
        #    pl.show()
        
        self.assertTrue( ((rec_r_values[0:len(eps_gd)] - eps_gd)**2).sum() < (self.tolerance*len(inp_ev_times)) )


if __name__ == '__main__':
    unittest.main()

