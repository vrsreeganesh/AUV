"""
Aim: Custom Functions
"""

# packages/libraries
import numpy as np
import os
import pdb
import matplotlib.pyplot as plt
import scipy

# function to calculate speed from velocity
def fCalculateSpeed(AUV_velocity):
    speed = np.linalg.norm(AUV_velocity,
                           ord=2)
    return speed

# column normalize
def fColumnNormalize(inputtensor):
    column_normalized_tensor = inputtensor/np.linalg.norm(inputtensor, 
                                                          ord       = 2,
                                                          axis      = 0,
                                                          keepdims  = True)
    return column_normalized_tensor