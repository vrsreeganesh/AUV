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
import scipy

# clearing python terminal
os.system('cls' if os.name == 'nt' else 'clear')

# Signal parameters
c                       = 1500      # speed of sound
chirp_start_frequency   = 100       # starting frequency of chirp signal
recording_time          = 2e-1      # duration of recording
sampling_frequency      = 16e3      # sampling frequency of hydrophones
transmissionDuration    = 1e-2      # duration of transmission

max_range               = transmissionDuration*c/2; # maximum range for which we're processing
timeArray               = np.arange(0, 
                                    transmissionDuration + 1/sampling_frequency, 
                                    1/sampling_frequency)   # sampling intervals
transmissionSignal      = scipy.signal.chirp(timeArray,
                                             chirp_start_frequency,
                                             timeArray[-1],
                                             sampling_frequency/2)


# plotting transmitted signal
plt.figure(1)
plt.plot(timeArray, transmissionSignal);
plt.xlabel("time (in seconds)", fontsize=16)
plt.ylabel("Amplitude", fontsize=16)
plt.title("Transmitted Signal", fontsize=16)
plt.draw(); plt.pause(1);
plt.savefig("/Users/vrsreeganesh/Documents/GitHub/AUV/Code/Figures/transmitted_signal.png")

