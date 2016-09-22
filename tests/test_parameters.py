"""Unit tests for parameters

realtimcornwell@gmail.com
"""

import unittest
import os

from arl.parameters import *


class TestParameters(unittest.TestCase):
    def setUp(self):
        chome = os.environ['CROCODILE']
        
        self.paramsfile = "%s/data/parameters/TestParameters.txt" % chome

        self.parameters = {'npixel': 256, 'cellsize':0.1, 'predict.cellsize':0.2, 'invert.spectral_mode':'mfs'}

    def test_exportimport(self):
        export_parameters(self.parameters, self.paramsfile)
        d = import_parameters(self.paramsfile)
        assert d == self.parameters


def test_getparameter(self):
    
    assert get_parameter(self.parameters, 'cellsize') == 0.1
    assert get_parameter(self.parameters, 'predict.cellsize') == 0.2
    assert get_parameter(self.parameters, 'spectral_mode', 'channels') == 'channels'
    assert get_parameter(self.parameters, 'invert.spectral_mode') == 'mfs'
    assert get_parameter(self.parameters, 'predict.cellsize') == 0.2

if __name__ == '__main__':
    unittest.main()