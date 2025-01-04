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

# Class representing ULA
class ULA:
    # init function
    def __init__(self,
                 num_sensors,           # number of sensors
                 intersensor_distance,  # distance between sensors
                 first_sensor_location, # location of first sensor
                 ula_sensor_direction): # vector from origin to rest
        """
        Initializing params related to ULA
        """

        # independent variables
        self.num_sensors            = num_sensors            # number of sensors
        self.intersensor_distance   = intersensor_distance   # space between sensors
        self.first_sensor_location  = first_sensor_location  # location of 1st sensor
        self.ula_sensor_direction   = ula_sensor_direction   # vector from first-to-rest

        # dependent variables
        self._ULA_sensorlocations   = None
    
    # calculate sensor locations
    def __getattr__(self, name):
        if name == "ULA_sensorlocations":
            # length-normalizing the signal
            self.ula_sensor_direction = self.ula_sensor_direction/np.linalg.norm(self.ula_sensor_direction, 
                                                                                 ord = 2, 
                                                                                 axis = 0, 
                                                                                 keepdims=True)
            self._ULA_sensorlocations = np.tile(self.ula_sensor_direction * self.intersensor_distance, 
                                                [1, self.num_sensors])
            self._ULA_sensorlocations = self.first_sensor_location + \
                                        self._ULA_sensorlocations * np.arange(0, self.num_sensors, 1)

            # returning
            return self._ULA_sensorlocations


# class representing scatters
class Scatterer:
    # init function
    def __init__(self,
                 coordinates    = None,
                 reflectivity   = None):
        self.coordinates    = coordinates
        self.reflectivity   = reflectivity

    # function: produce top view

    # function: produce side-view

    # function: project to angle

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
        



# class representing AUV
class AUV:
    # init function
    def __init__(self,
                 location,              # current location of AUV [tensor]
                 velocity,              # velocity of AUV [tensor]
                 acceleration,          # acceleration of AUV [tensor]
                 pointing_direction):   # direction in which AUV is pointed [tensor]
        """
        Initializing parameters related to AUV
        """
        # fundamental attributes
        self.location               = location              # current location of AUV
        self.velocity               = velocity              # velocity of AUV
        self.acceleration           = acceleration          # acceleration of AUV
        self.pointing_direction     = pointing_direction    # direction in which AUV is pointed

        # add-on attributes
        self.projector_starboard    = None   # projector to the right
        self.projector_portside     = None   # projector to the left
        self.projector_fbls         = None   # projector to the front
        
        self.ula_portside           = None   # ULA mounted on the left
        self.ula_starboard          = None   # ULA mounted on the right

    def summarize(self):
        print(">location              = \n", self.location)
        print(">velocity              = \n", self.velocity)
        print(">acceleration          = \n", self.acceleration)
        print(">pointing_direction    = \n", self.pointing_direction)
        # print("\t>")
        # print("\t>")
        # print("\t>")
        # print("\t>")
        

    def update_timestep(self):
        """
        Updating the after each time step
        """

    def simulate_signal(self):
        """
        Aim: Simulate signals
        Note:
            > Project signals from projectors
            > Return signals from scatters
            > simulate signals received by ULAs
        """
        pass