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
import CustomFunctions as cf
import math

# clearing python terminal
os.system('cls' if os.name == 'nt' else 'clear')

# ULA parameters
num_sensors               = 32                # number of sensors
intersensor_distance      = 1e-3              # space between sensors
ULA_origin                = np.zeros((2,1))   # location of first sensor
ULA_sensor_direction      = np.zeros((2,1))   # direction from origin to rest
ULA_sensor_direction[0,0] = 1                 # setting direction
ULA_sensor_direction      = cf.fColumnNormalize(ULA_sensor_direction)
ULA_sensorlocations       = np.arange(0, num_sensors, intersensor_distance)
ULA_axis                  = - ULA_sensor_direction

# AUV parameters
AUV_speed                   = 2                 # current AUV speed
AUV_currentlocation         = np.zeros((2,1))   # setting up AUV location
AUV_currentlocation[0,0]    = 0                 # in meters
AUV_currentlocation[1,0]    = 5                 # in meters

AUV_velocity        = np.zeros((2,1))           # setting up AUV speed
AUV_velocity[0,0]   = 1
AUV_velocity        = AUV_speed * AUV_velocity

# setting up signal projectors
projector_starboard_location   = ULA_sensorlocations[0]
projector_port_location        = ULA_sensorlocations[0]
projector_fbls_location        = ULA_sensorlocations[0]

projector_starboard_wrtaxis       = np.zeros([3,1])
projector_starboard_wrtaxis[0,0]  = 0
projector_starboard_wrtaxis[1,0]  = -1
projector_starboard_wrtaxis[2,0]  = -1
projector_starboard_wrtaxis       = cf.fColumnNormalize(projector_starboard_wrtaxis)

projector_port_wrtaxis        = np.zeros([3,1])
projector_port_wrtaxis[0,0]   = 0
projector_port_wrtaxis[1,0]   = 1
projector_port_wrtaxis[2,0]   = -1
projector_port_wrtaxis        = cf.fColumnNormalize(projector_port_wrtaxis)

projector_fbls_wrtaxis        = np.zeros([3,1])
projector_fbls_wrtaxis[0,0]   = 1
projector_fbls_wrtaxis[1,0]   = 0
projector_fbls_wrtaxis[2,0]   = -1
projector_fbls_wrtaxis        = cf.fColumnNormalize(projector_fbls_wrtaxis)

# Signal parameters
c                       = 1500      # speed of sound
chirp_start_frequency   = 100       # starting frequency of chirp signal
recording_time          = 2e-1      # duration of recording
sampling_frequency      = 16e3      # sampling frequency of hydrophones
transmissionDuration    = 1e-2      # duration of transmission

max_range               = transmissionDuration*c/2; # maximum range for which we're processing
timeArray               = np.arange(0, transmissionDuration + 1/sampling_frequency, 
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

# Building impulse signal simulation
numsamples = math.floor(recording_time * sampling_frequency)
signalMatrix = np.zeros([numsamples, num_sensors])

# loading scatters
floor_coordinates_3D    = np.load("/Users/vrsreeganesh/Documents/GitHub/AUV/Code/Assets/floor_coordinates_3D.npy")
floor_scatterers        = np.load("/Users/vrsreeganesh/Documents/GitHub/AUV/Code/Assets/floor_scatterers.npy")

# changing relative origin

# plotting signal matrix
plt.figure(2)
plt.imshow(signalMatrix[0:32,:], cmap='hot', interpolation='nearest')
plt.colorbar()
plt.draw(); plt.pause(1);