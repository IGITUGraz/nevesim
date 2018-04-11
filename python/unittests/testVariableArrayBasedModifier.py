#
# testVariableArrayBasedModifier.py
#
# Test functionality of VariableArrayBasedModifier
# by using it to set the learn rate of NSHebbianSynapse
#
# David Kappel 25.07.2012
#

import pylab as pl
import numpy as np
import pyevesim as ev
import unittest



class VariableArrayBasedModifierTest (unittest.TestCase):
    """
    VariableArrayBasedModifierTest

    Runs several tests to verify the functionality of
    VariableArrayBasedModifier by using it to set the
    learn rate of NSHebbianSynapse
    
    evesim classes involved:
    VariableArrayBasedModifier, NSHebbianSynapse,
    NeurSamplingNetwork, 
    """
    
    def setUp(self):
        self.t_refrac = 20e-3
        self.min_speedup = 50
        self.max_std = 1.5
        self.verbose = False
        self.plot_results = False
        self.dt = 0.001                                     # interpretation of a Markov chain step
        self.W_mu, self.W_std =  0.,  0.5                   # params for random connectivity matrix
        self.b_mu, self.b_std = -2., 0.25

        
    def test_cyclic(self):
        val_list = np.array([0.2,-0.2,0,5],dtype=np.float)
        test_list = self.run_test(val_list,True,10)
        self.assertAlmostEqual((test_list - [0,0.2,-0.2,0,5,0.2,-0.2,0,5,0.2]).sum(), 0)

    def test_acyclic(self):
        val_list = np.array([0.2,-0.2,0,5],dtype=np.float)
        test_list = self.run_test(val_list,False,10)
        self.assertAlmostEqual((test_list - [0,0.2,-0.2,0,5,5,5,5,5,5]).sum(), 0)
        
    def run_test(self, val_list, cyclic, num_runs):
        
        num_neurons = 2
                
        net = ev.NeurSamplingNetwork()
        nrn_ids = net.create(ev.ExpPoissonNeuron(1.0/self.t_refrac, 0, self.t_refrac), num_neurons)
            
        syn_ids = net.connectAll2All(nrn_ids, nrn_ids,
                                     ev.CompositeSynapse(ev.NSHebbianActiveSynapse(1,0),
                                                         ev.ResetRectSpikeResponse(self.t_refrac),
                                                         ev.ResetRectSpikeResponse(self.t_refrac)))
        
        sh_vals = ev.SharedFloatArray(val_list)
        
        modifier = ev.FloatVariableModifier()
        modifier.setCyclic(cyclic)
        modifier.setValuesArray(sh_vals)
        
        m_ids = net.create(modifier,syn_ids.shape[0])
        c_id = net.create(ev.EvSimRegularClock(0.1,True,0.05))
        
        for i in xrange(syn_ids.shape[0]):            
            net.getObject(m_ids[i]).setModifiedVariable(net.getObject(syn_ids[i]),"eta")
            net.connect(c_id,0,m_ids[i],0)
            
        result = np.zeros(num_runs,dtype=np.float)
        
        for i in xrange(num_runs):
            result[i] = net.getObject(syn_ids[0]).eta
            net.simulate(0.100)
            
        return result


if __name__ == '__main__':
    unittest.main()
