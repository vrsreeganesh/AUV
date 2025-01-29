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


os.system('clear');

# setting paths
tensor_fls          = "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_fls_coordinates.pt";
tensor_portside     = "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_port_coordinates.pt";
tensor_starboard    = "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_seaboard.coordinates.pt";

# loading tensors
tensor_fls          = load_tensors(tensor_fls);
tensor_portside     = load_tensors(tensor_portside);
tensor_starboard    = load_tensors(tensor_starboard);

scatterfloor = torch.cat((tensor_fls, tensor_portside, tensor_starboard), 1);
print("scatterfloor.shape (before subsetting)    = ", scatterfloor.shape);



scatterfloor = scatterfloor[:, 1:-1:100];
print("scatterfloor.shape (after subsetting)    = ", scatterfloor.shape);

# plotting
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
# scatter = ax.scatter(scatterfloor[0], 
#                      scatterfloor[1], 
#                      scatterfloor[2], 
#                      c=colors, cmap='viridis', s=50)
scatter = ax.scatter(scatterfloor[0,:], 
                     scatterfloor[1,:], 
                     scatterfloor[2,:]);

# setting dimensional limitations
ax.set_xlim(0, 100);
ax.set_ylim(0, 100);
ax.set_zlim(0, 10);

# Add labels
ax.set_xlabel('X Label')
ax.set_ylabel('Y Label')
ax.set_zlabel('Z Label')

# Add a color bar
plt.colorbar(scatter, ax=ax, shrink=0.5, aspect=10)

# Show the plot
# plt.show()
plt.draw();
plt.savefig(os.path.join("/Users/vrsreeganesh/Documents/GitHub/AUV/Code/Python/Figures",
                      get_current_time()));
plt.pause(0.1);





