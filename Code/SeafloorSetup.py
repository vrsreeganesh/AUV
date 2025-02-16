"""
Aim: Setup a sea-floor
Note: 
"""

# packages/libraries
import numpy as np
import os
import pdb; import pickle
import matplotlib.pyplot as plt
from Classes.Class_Scatterer import Scatterer

# clearing python terminal
os.system('cls' if os.name == 'nt' else 'clear')

# Setting up flat sea-floor
del_steps   = 1e-1
x_steps     = del_steps;    # distance between points in x-direction
y_steps     = x_steps;      # distance between points in y-direction

# floor bounds
x_begin = 0; x_end = 10;
y_begin = 0; y_end = 10;

# setting up coordinates
x_flat_coordinates = np.arange(x_begin, x_end + x_steps,  x_steps);
y_flat_coordinates = np.arange(y_begin, y_end + y_steps,  y_steps);
x_temp, y_temp = np.meshgrid(x_flat_coordinates, y_flat_coordinates)

x_temp = x_temp.flatten()[None,:]   # flattening 
y_temp = y_temp.flatten()[None,:]   # flattening

floor_coordinates_2D = np.concatenate((x_temp, y_temp), axis = 0)

# setting up 3D coordinates
floor_coordinates_3D = np.concatenate((floor_coordinates_2D,
                                       np.zeros((1, floor_coordinates_2D.shape[1]))), 
                                       axis = 0);

# setting up scatter strengths
floor_scatterers_reflectivity = np.random.normal(loc    = 10.0,
                                    scale  = 1.0,
                                    size   = (1, floor_coordinates_3D.shape[1]))

# creating class object
floor_scatterers = Scatterer(coordinates    = floor_coordinates_3D,
                             reflectivity   = floor_scatterers_reflectivity)

# Saving objects
with open("/Users/vrsreeganesh/Documents/GitHub/AUV/Code/Assets/floor_scatterers.pkl", "wb") as file:
    pickle.dump(floor_scatterers, file)

# plotting - 2D
plt.figure(1)
plt.scatter(floor_coordinates_2D[0,:], floor_coordinates_2D[1,:],
            c = 'lightgreen', marker='o', s = 0.1, alpha = 0.5)
plt.xlabel('X-axis'); plt.ylabel('Y-axis')
plt.title('Top View')
plt.draw(); plt.pause(0.1)
plt.savefig("/Users/vrsreeganesh/Documents/GitHub/AUV/Code/Figures/floor_coordinates_2D.png")

# plotting - 3D
fig = plt.figure()
ax  = fig.add_subplot(111, projection='3d')

# Scatter plot
ax.scatter(floor_scatterers.coordinates[0,:], 
           floor_scatterers.coordinates[1,:],
           floor_scatterers.coordinates[2,:], 
           c = 'lightgreen', 
           marker   ='o',
           s        = 0.1,
           alpha    = 0.5)

# Add labels
ax.set_xlabel('X Label')
ax.set_ylabel('Y Label')
ax.set_zlabel('Z Label')
plt.title('Side View')
plt.savefig("/Users/vrsreeganesh/Documents/GitHub/AUV/Code/Figures/floor_coordinates_3D.png")
plt.draw(); plt.pause(1)
