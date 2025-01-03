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
                 num_sensors,
                 intersensor_distance,
                 first_sensor_location,
                 ula_sensor_direction):
        """
        Initializing params related to ULA
        """

        # independent variables
        self.num_sensors            = num_sensors            # number of sensors
        self.intersensor_distance   = intersensor_distance   # space between sensors
        self.first_sensor_location  = first_sensor_location  # location of 1st sensor
        self.ula_sensor_direction   = ula_sensor_direction   # vector from origin to rest

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



# class representing project
class Projector:
    def __init__(self,
                 location,
                 direction):
        self.location   = location
        self.direction  = direction



# class representing AUV
class AUV:
    def __init__(self,
                 velocity):
        """
        Initializing parameters related to AUV
        """
        # self.speed = speed
        self.velocity               = None
        self.projector_starboard    = Projector()
        self.projector_portside     = Projector()
        self.projector_fbls         = Projector()
        self.ula                    = ULA()

    def update():
        """
        Updating the after each time step
        """
