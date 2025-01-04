"""
Aim: Signal simulation from sea-floor
Note: 
    The previous script produces scatterers that represent the sea-floor. Here, we simulate the signal received from these scatterers. 
"""

# packages/libraries
import numpy as np; import os; import pdb; import matplotlib.pyplot as plt
import scipy; import CustomFunctions as cf; import math; import pickle
from CustomClasses import ULA, AUV, Projector, Scatterer; import sys;

# clearing python terminal
os.system('cls' if os.name == 'nt' else 'clear')

# Reading scatterers
with open("/Users/vrsreeganesh/Documents/GitHub/AUV/Code/Assets/floor_scatterers.pkl", "rb") as f:
    floor_scatterers = pickle.load(f)

# AUV Setup
auv_initial_location                = np.zeros([3,1])       # auv's initial location
auv_initial_location[1,0]           = 2.0
auv_initial_location[2,0]           = 2.0
auv_initial_velocity                = np.zeros([3,1])       # auv's initial velocity
auv_initial_velocity[0,0]           = 1.0
auv_acceleration                    = np.zeros([3,1])           # auv's acceleration
auv_acceleration[0,0]               = 0.2
auv_initial_pointing_direction      = np.zeros([3,1])    # initial pointing direction
auv_initial_pointing_direction[0,0] = 1

# initializing AUV
auv = AUV(location              = auv_initial_location,
          velocity              = auv_initial_velocity,
          acceleration          = auv_acceleration,
          pointing_direction    = auv_initial_pointing_direction)

# Setting up ULAs
num_sensors               = 32                # number of sensors
intersensor_distance      = 1e-3              # space between sensors
auv.ula_portside    = ULA(num_sensors            = num_sensors,
                          intersensor_distance   = intersensor_distance,
                          first_sensor_location  = None,
                          ula_sensor_direction   = None)
auv.ula_starboard   = ULA(num_sensors            = num_sensors,
                          intersensor_distance   = intersensor_distance,
                          first_sensor_location  = None,
                          ula_sensor_direction   = None)

# Setting up projectors
auv.projector_fbls          = Projector(location            = None,
                                        azimuthal_angle     = 0,
                                        elevation_angle     = cf.fCart2Rad(-60),
                                        azimuth_beamwidth   = cf.fCart2Rad(120),
                                        vertical_beamwidth  = cf.fCart2Rad(60))
auv.projector_portside      = Projector(location            = None,
                                        azimuthal_angle     = cf.fCart2Rad(90),
                                        elevation_angle     = cf.fCart2Rad(-60),
                                        azimuth_beamwidth   = cf.fCart2Rad(60),
                                        vertical_beamwidth  = cf.fCart2Rad(30))
auv.projector_starboard     = Projector(location            = None,
                                        azimuthal_angle     = cf.fCart2Rad(-90),
                                        elevation_angle     = cf.fCart2Rad(-60),
                                        azimuth_beamwidth   = cf.fCart2Rad(60),
                                        vertical_beamwidth  = cf.fCart2Rad(30))


sys.exit()

# ULA parameters
num_sensors               = 32                # number of sensors
intersensor_distance      = 1e-3              # space between sensors
ULA_origin                = np.zeros((2,1))   # location of first sensor
ULA_sensor_direction      = np.zeros((2,1))   # direction from origin to rest
ULA_sensor_direction[0,0] = 1                 # setting direction
ULA_sensor_direction      = cf.fColumnNormalize(ULA_sensor_direction)
ULA_sensorlocations       = np.arange(0, num_sensors, intersensor_distance)
ULA_axis                  = - ULA_sensor_direction



sys.exit()

# Setting up ULA
ula = ULA(num_sensors            = 4,
          intersensor_distance   = 1e-3,
          first_sensor_location  = np.zeros([3,1]),
          ula_sensor_direction   = np.array([[1],[0],[0]]))

print("ula.num_sensors = ", ula.num_sensors)
print("ula.intersensor_distance = ", ula.intersensor_distance)
print("ula.first_sensor_location = \n", ula.first_sensor_location)
print("ula.ula_sensor_direction = \n", ula.ula_sensor_direction)
print("ula.ULA_sensorlocations = \n", ula.ULA_sensorlocations)

sys.exit()

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