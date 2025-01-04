"""
Aim: Classes
Note:
    ULA: Class for Uniform Linear Arrays
    AUV: Class for Autonomous Underwater Vehicle
    Projector: Class for Projector
"""
# packages/libraries
import numpy as np
import os
import pdb
import matplotlib.pyplot as plt
import scipy
from Classes.Class_Scatterer import Scatterer

# class representing project
class Projector:
    # init function
    def __init__(self,
                 location           = None,     # location of projector
                 azimuthal_angle    = None,     # pointing direction of projector
                 elevation_angle    = None,     # pointing direction of projector
                 azimuth_beamwidth  = None,     # sound projection between axis and max
                 vertical_beamwidth = None):    # sound projection between axis and max
        """
        Init function
            - location:     Location of projector
            - direction:    direction of projector
        """
        self.location               = location
        self.azimuthal_angle        = azimuthal_angle
        self.elevation_angle        = elevation_angle
        self.azimuthal_beamwidth    = azimuth_beamwidth
        self.vertical_beamwith      = vertical_beamwidth

    # subset signals
    def subset_scatters(self,
                        scatterers: Scatterer):
        """
        Aim: Take a set of scatters and subset them
        Note:
            > Takes in a tensor representing the points
            > subset the scatterers that are within FOV of current projector
            > returns the subset
        """