"""
Aim: Signal simulation from sea-floor
Note: 
    The previous script produces scatterers that represent the sea-floor. Here, we simulate the signal received from these scatterers. 
"""

# packages/libraries
import numpy as np
import os
import pdb
import matplotlib.pyplot as plt

# clearing python terminal
os.system('cls' if os.name == 'nt' else 'clear')

# Signal parameters
c                       = 1500
chirp_start_frequency   = 100
recording_time          = 2e-1       # Duration of Recording
sampling_frequency      = 16e3# Sampling Frequency of Hydrophones
transmissionDuration    = 1e-2#Duration of transmission
timeArray               = 0:(1/sampling_frequency):transmissionDuration;  % sampling intervals
transmissionSignal      = chirp(timeArray, ...
                                100, ...
                                timeArray(end), ...
                                sampling_frequency/2);
max_range               = transmissionDuration*c/2;

