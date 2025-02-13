# Plotting beamformed image

# importing libraries
import torch; import matplotlib.pyplot as plt; import pdb; import os; 
from datetime import datetime; import torchvision;

# function definitions
def load_tensors(path):
    scatterfloor = torch.jit.load(path);
    scatterfloor = list(scatterfloor.parameters())[0];
    return scatterfloor
def get_current_time():
    now = datetime.now()
    return now.strftime("%H_%M_%S")

# setting paths
tensor_fls          = "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/ULA_fls_image.pt"
tensor_fls          = load_tensors(tensor_fls)
tensor_port         = "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/ULA_port_image.pt"
tensor_port         = load_tensors(tensor_port)
tensor_starboard    = "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/ULA_starboard_image.pt"
tensor_starboard    = load_tensors(tensor_starboard)

# printing the dimensions
print(f"tensor_fls.shape = {tensor_fls.shape}")
print(f"tensor_port.shape = {tensor_port.shape}")
print(f"tensor_starboard.shape = {tensor_starboard.shape}")

# plotting
plt.imshow(torch.abs(tensor_fls), 
           cmap='viridis', 
           interpolation='nearest')
plt.title('Random Data')
# plt.colorbar()


# printing
image_fls = torch.abs(tensor_fls).tile(1,1,3).unsqueeze(0);
image_fls = image_fls - torch.min(image_fls)
image_fls = image_fls/torch.std(image_fls)
print("average image_fls = ", torch.mean(image_fls));


# saving the tensor as image
imagepath = os.path.join("/Users/vrsreeganesh/Documents/GitHub/AUV/Code/Python/Figures",
                         "BeamformedImage_fls_timeID"+get_current_time()+".jpg");
torchvision.utils.save_image(image_fls, imagepath);



# plt.tight_layout()
# plt.show()
# plt.draw();
# plt.pause(4);
# plt.savefig(os.path.join("/Users/vrsreeganesh/Documents/GitHub/AUV/Code/Python/Figures",
#                          "BeamformedImage_fls"+get_current_time()));




