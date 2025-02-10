# Plotting the tensors created from the sea-floor

# importing libraries
import torch; import matplotlib.pyplot as plt; import pdb; import os; 
from datetime import datetime

# function definitions
def load_tensors(path):
    scatterfloor = torch.jit.load(path);
    scatterfloor = list(scatterfloor.parameters())[0];
    return scatterfloor
def get_current_time():
    now = datetime.now()
    return now.strftime("%H_%M_%S")

# setting paths
tensor_fls          = "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_gt.pt";

# loading tensors
tensor_fls          = load_tensors(tensor_fls);

# subsetting points
tensor_fls = tensor_fls[:, 0:-1:10];

print("points.shape = ", tensor_fls.shape)

# Set up the 3D axes
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.scatter(tensor_fls[0, :], 
           tensor_fls[1, :], 
           tensor_fls[2, :], s = 0.1);
ax.set_zlim(0, 100)

# Customize labels and title
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')

plt.title('3D Scatter Plot', fontsize=14)

# Optional: Adjust viewing angle
ax.view_init(elev=30, azim=45)


# plt.draw()
# plt.pause(5)

plt.show()