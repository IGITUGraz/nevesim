#
# pyevesimTests.py
#
# Global test loader runs all pyevesim tests
#

import unittest
import sys, os

sys.path.append(os.path.dirname(os.path.realpath(__file__)))
sys.path.append("..")

from testRectangularRefractorySampler import RectangularRefractorySamplerTest
from testPLResetKernelSpikeResponse import PLResetKernelSpikeResponseTest
from testPLAdditiveKernelSpikeResponse import PLAdditiveKernelSpikeResponseTest
from testPLExpPoissonNeuron import PLExpPoissonNeuronTest
from testDblExpSpikeResponse import DblExpSpikeResponseTest

unittest.TestLoader().loadTestsFromTestCase(RectangularRefractorySamplerTest)
unittest.TestLoader().loadTestsFromTestCase(DblExpSpikeResponseTest)
unittest.TestLoader().loadTestsFromTestCase(PLResetKernelSpikeResponseTest)
unittest.TestLoader().loadTestsFromTestCase(PLAdditiveKernelSpikeResponseTest)
unittest.TestLoader().loadTestsFromTestCase(PLExpPoissonNeuronTest)

unittest.main()
