# Testing the sea-floor created in C++ scripts

# importing libraries
import torch
import matplotlib.pyplot as plt
import pdb
import os

# function definitions
def load_tensors(path):
    scatterfloor = torch.jit.load(path);
    scatterfloor = list(scatterfloor.parameters())[0];
    return scatterfloor

from datetime import datetime

def get_current_time():
    now = datetime.now()
    return now.strftime("%H_%M_%S")


# os.system('clear');

# setting paths
tensor_fls          = "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_fls_coordinates.pt";
tensor_portside     = "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_port_coordinates.pt";
tensor_starboard    = "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_starboard.coordinates.pt";
tensor_gt           = "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_gt.pt";


tensor_fls_reflectivity          = "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_fls_coordinates_reflectivity.pt";
tensor_portside_reflectivity     = "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_port_coordinates_reflectivity.pt";
tensor_starboard_reflectivity    = "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_starboard.coordinates_reflectivity.pt";
tensor_gt_reflectivity           = "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_gt_reflectivity.pt";

# loading tensors
tensor_fls          = load_tensors(tensor_fls);
tensor_portside     = load_tensors(tensor_portside);
tensor_starboard    = load_tensors(tensor_starboard);
tensor_gt           = load_tensors(tensor_gt);

tensor_fls_reflectivity          = load_tensors(tensor_fls_reflectivity);
tensor_portside_reflectivity     = load_tensors(tensor_portside_reflectivity);
tensor_starboard_reflectivity   = load_tensors(tensor_starboard_reflectivity);
tensor_gt_reflectivity = load_tensors(tensor_gt_reflectivity);


# print(tensor_fls_reflectivity.shape)
# print(tensor_portside_reflectivity.shape)
# print(tensor_starboard_reflectivity.shape)

# putting it all together
scatterfloor = torch.cat((tensor_fls, tensor_portside, tensor_starboard), 1);
scatterfloor_reflectivity = torch.cat((tensor_fls_reflectivity.squeeze(), 
                                       tensor_portside_reflectivity.squeeze(), 
                                       tensor_starboard_reflectivity.squeeze()), 0);


# # just for fls
# scatterfloor = tensor_fls
# scatterfloor_reflectivity = tensor_fls_reflectivity


# =============================================================================
# # plotting
# fig = plt.figure()
# ax = fig.add_subplot(111, projection='3d')
# scatter = ax.scatter(scatterfloor[0,:], 
#                      scatterfloor[1,:], 
#                      scatterfloor[2,:], s= 0.1);
# ax.view_init(elev=30, azim=-160)

# # setting dimensional limitations
# ax.set_xlim(0, 100);
# ax.set_ylim(0, 100);
# ax.set_zlim(0, 70);

# # Add labels
# ax.set_xlabel('X Label')
# ax.set_ylabel('Y Label')
# ax.set_zlabel('Z Label')

# # Add a color bar
# plt.colorbar(scatter, ax=ax, shrink=0.5, aspect=10)

# Show the plot
# plt.show()
# plt.draw();
# plt.savefig(os.path.join("/Users/vrsreeganesh/Documents/GitHub/AUV/Code/Python/Figures",
#                       get_current_time()));
# plt.pause(0.1);



# ------------------------------------------------------------------------------

# Create figure
fig = plt.figure(figsize=(40, 20))

# First subplot (Bottom)
ax1 = fig.add_subplot(121, projection='3d')  # 2 rows, 1 column, 1st plot
ax1.set_box_aspect([1,1,1])
scatter1 = ax1.scatter(scatterfloor[0,:], 
                       scatterfloor[1,:], 
                       scatterfloor[2,:], 
                       s=0.1,
                       c = scatterfloor_reflectivity,
                       cmap='jet');
ax1.view_init(elev=30, azim=-160)


# Setting limits
ax1.set_xlim(0, 100)
ax1.set_ylim(0, 100)
ax1.set_zlim(0, 40)

# Labels
ax1.set_xlabel('X Label')
ax1.set_ylabel('Y Label')
ax1.set_zlabel('Z Label')

# # Colorbar
# plt.colorbar(scatter1, ax=ax1, shrink=0.5, aspect=10)

# Second subplot (Top)
ax2 = fig.add_subplot(122, projection='3d')  # 2nd plot
ax2.set_box_aspect([1,1,1])
scatter2 = ax2.scatter(tensor_gt[0,:], 
                       tensor_gt[1,:], 
                       tensor_gt[2,:], 
                       c= tensor_gt_reflectivity, 
                       s=0.01, cmap='jet');
ax2.view_init(elev=30, azim=-160)


# Setting limits
ax2.set_xlim(0, 100)
ax2.set_ylim(0, 100)
ax2.set_zlim(0, 40)

# Labels
ax2.set_xlabel('X Label')
ax2.set_ylabel('Y Label')
ax2.set_zlabel('Z Label')

# # Colorbar
# plt.colorbar(scatter2, ax=ax2, shrink=0.5, aspect=10)

plt.tight_layout()
# plt.show()
# plt.draw();
plt.savefig(os.path.join("/Users/vrsreeganesh/Documents/GitHub/AUV/Code/Python/Figures",
                      get_current_time()));




