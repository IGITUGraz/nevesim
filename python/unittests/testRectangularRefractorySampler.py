#
# RectangularRefractorySamplerTest.py
#
# Rectangular refractory kernel sampler. 'Golden Device'
# implementation by Johannes Bill tested against evesim.
#
# David Kappel 25.07.2012
#

import numpy as np
import pyevesim as ev
import unittest
from time import time

class BuesingSamplerAbsRefractorySigma(object):
    """
    Neural Dynamics Sampler (developed by of Lars Buesing) augmented with interaction.

    J : interaction matrix incl. inhibition and pi_k
    u : array of membrane potential variables
    z : state vector of cause neurons
    N : num timesteps for refractory period / inhibition
    burnIn : num Updates per neuron to draw valid sample

    Comments on interaction matrix J:
        Diagonal elements J_kk correspond to pi_k!!!
        Normally: u_k = 1/2 J_kk + ...
        Here we use: u_k = (J*z)_k for faster calculation. Thus pi_k == J_kk/2 |-> pi_k == J_kk!
    """
    def __init__ (self, J, u, z, N, burnIn=1):
        self.shape = z.shape
        assert u.shape == z.shape, "ERROR: Shapes of u and z don't match!"
        self.z = z.reshape(z.size)
        self.u = u.reshape(u.size) # does NOT include pi_k, since added via J_kk
        self.r = np.zeros(u.size)
        self.dummy = np.zeros(u.size)
        assert J.shape == u.shape + u.shape, "ERROR: Shapes of J and u don't match!"
        self.J = J.reshape((u.size, u.size))
        self.N = N
        self.logN = np.log(N)
        self.burnIn = burnIn

        self.sigma = lambda x: 1./(1. + np.exp(-x))
        self.spike_in_01 = lambda u: np.random.rand() < self.sigma(u - self.logN)
        self._Jz = lambda idx: np.dot(self.J[idx], np.greater(self.r,0.,self.dummy))

    def draw_sample(self, clearState = False, randomOrder=False):
        if clearState:
            self.r.fill(0.)
        indices = np.arange(self.u.size)
        for iteration in xrange(self.burnIn):
            if randomOrder: np.random.shuffle(indices)
            for idx in indices:
                u = self.u[idx]
                r = self.r[idx]
                #print idx, u, r
                if r > 1:               # easy case: neuron is refractory
                    self.r[idx] -= 1
                    continue
                # we know r in {0,1}; make sure J_kk is added to u
                self.r[idx] += 1
                u += self._Jz(idx)      # u (=V*y) + interaction + pi_k
                self.r[idx] -= 1
                self.r[idx] = self.N * self.spike_in_01(u)

        self.z[:] = self.r > 0
        return self.z


class RectangularRefractorySamplerTest (unittest.TestCase):
    """
    RectangularRefractorySamplerTest

    Runs several tests sampling from a evesim network of
    different sizes. Sets up the same network using the
    'golden device' implementation and compares output
    statistics and empirical speedup.
    
    evesim classes involved:
    NeurSamplingNetwork, BasicActiveSynapse,
    ResetRectSpikeResponse, ExpPoissonNeuron
    """
    
    def setUp(self):        
        self.verbose = False
        self.plot_results = False
        self.dt = 0.001                                     # interpretation of a Markov chain step
        self.W_mu, self.W_std =  0.,  0.5                   # params for random connectivity matrix
        self.b_mu, self.b_std = -2., 0.25
        self.t_refrac = 20e-3
        self.min_speedup = 20
        self.max_std = 1.5
 
    def test_10_neur_10s(self):
        self.run_test(10,10.0)
        
    def run_test(self, num_neurons, t_max):
        np.random.seed(1243098)        
        N = int(round(self.t_refrac/self.dt))               # num refractory states
        
        if self.verbose:
            print "\n\n  > Simulating network of", num_neurons, "neurons."
        
        # build symmetric connectivity matrix
        J = np.random.normal(self.W_mu, self.W_std, num_neurons**2).reshape((num_neurons,num_neurons))
        for i in xrange(num_neurons):
            for j in xrange(i):
                J[i,j] = J[j,i]
        J += np.diag(np.random.normal(self.b_mu, self.b_std, num_neurons) - J.diagonal())
        
        z = np.zeros(num_neurons)                           # vector holding z-states
        u = np.zeros(num_neurons)                           # additional biases b_k -> b_k + u_k
        
        # init sampler
        sampler = BuesingSamplerAbsRefractorySigma(J=J, z=z, u=u, N=N)
        
        # record states to the following arrays
        num_samples = len(np.arange(0, t_max, self.dt))
        state = np.zeros((num_samples, num_neurons))
        spike = np.zeros((num_samples, num_neurons))
        
        
        # init evesim network:        
        if self.verbose:
            print '  > Using EvESim sampler'
        
        net = ev.NeurSamplingNetwork(ev.SimParameters(15309384))
        nrn_ids = np.zeros(num_neurons, dtype=np.int32 )
        for i in xrange(num_neurons):
            nrn_ids[i] = net.create(ev.ExpPoissonNeuron(1.0/self.t_refrac, J[i,i], self.t_refrac), 1)
            
        for i in xrange(num_neurons):
            for j in xrange(num_neurons):
                if i!=j:
                    net.connect(nrn_ids[i], nrn_ids[j],
                                ev.CompositeSynapse(ev.BasicActiveSynapse(J[i,j]),
                                                    ev.ResetRectSpikeResponse(self.t_refrac)))
        rec_ids = net.record(nrn_ids)
        
        # run simulation
        time_of_start = time()
        net.simulate(t_max)
        ev_time_elapsed = time() - time_of_start
        
        spike_times = []
        for i in xrange(num_neurons):
            tmp_spike_times = net.getObject(int(rec_ids[i])).getRecordedValues()
            spike_times.append(tmp_spike_times)
        
        if self.verbose:
            print "  > Simulation of %.3fs took %.3fs." % (t_max, ev_time_elapsed)
            print '  > Used sampler is', BuesingSamplerAbsRefractorySigma.__name__

        # run the 'golden device' sampler:
        time_of_start = time()
        
        for tn,t in enumerate(np.arange(0, t_max, self.dt)):
            state[tn] = sampler.draw_sample()
            spike[tn] = sampler.r == sampler.N
        
        gd_time_elapsed = time() - time_of_start
        speedup = gd_time_elapsed/ev_time_elapsed
        nu = spike.sum(0) / t_max
        nu_ev = np.zeros(num_neurons)
        
        for i in xrange(num_neurons):
            nu_ev[i] = spike_times[i].shape[0] / t_max        
        
        std_rate = np.std( nu - nu_ev )
        
        if self.verbose:
            print "  > Simulation of", num_samples, "time steps took %.3fs." % gd_time_elapsed
            print "  > speedup is: %.2f" % speedup
            print "  > standard error is: %.2f" % std_rate
       
        # plot results
        #if self.plot_results:
        #    
        #    fig_spike = pl.figure(figsize=(12,5))
        #    t,k = spike.nonzero()
        #    pl.plot(t, k, 'k|')
        #    pl.ylim(-0.5, num_neurons-0.5)
        #    pl.xlim(0, num_samples)
        #    pl.title('spike train (GD)')
        #    pl.xlabel('time [dt]')
        #    pl.ylabel('neuron index')
        #     pl.subplots_adjust(0.05,0.1,0.98,0.9)
        #     
        #    # average firing rates
        #    fig_rate = pl.figure(figsize=(4,3))
        #    pl.bar(range(num_neurons), nu, align='center', fc='orangered')
        #    pl.title('average firing rates (GD)')
        #    pl.xlabel('neuron index k')
        #    pl.ylabel('rate [Hz]')
        #    pl.subplots_adjust(0.18,0.15,0.98,0.9)
        #    pl.xlim(-0.5, num_neurons-0.5)
        #    pl.ylim(0., 1.1*nu.max())
        #    
        #    fig_ev_spike = pl.figure(figsize=(12,5))
        #    for i in xrange(num_neurons):
        #        pl.plot(spike_times[i], i*np.ones(spike_times[i].shape), 'k|')
        #    pl.ylim(-0.5, num_neurons-0.5)
        #    pl.xlim(0, t_max)
        #    pl.title('spike train (EvESim)')
        #    pl.xlabel('time [s]')
        #    pl.ylabel('neuron index')
        #    pl.subplots_adjust(0.05,0.1,0.98,0.9)
        #    
        #    fig_ev_rate = pl.figure(figsize=(4,3))
        #    pl.bar(range(num_neurons), nu_ev, align='center', fc='orangered')
        #    pl.title('average firing rates (EvESim)')
        #    pl.xlabel('neuron index k')
        #    pl.ylabel('rate [Hz]')
        #    pl.subplots_adjust(0.18,0.15,0.98,0.9)
        #    pl.xlim(-0.5, num_neurons-0.5)
        #    pl.ylim(0., 1.1*nu.max())
        #    
        #    pl.draw()
        #    pl.show()
        
        self.assertTrue( std_rate < self.max_std, "standard error of rate exceeds threshold" )
        self.assertTrue( speedup > self.min_speedup, "speedup below threshold" )


if __name__ == '__main__':
    unittest.main()
