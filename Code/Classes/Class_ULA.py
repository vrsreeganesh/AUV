"""
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
