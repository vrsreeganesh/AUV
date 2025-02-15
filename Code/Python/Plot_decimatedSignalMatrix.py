''' ========================================================
Aim: Plotting Signal Matrix

---------------------------------------------------------'''

# importing libraries
import torch; import matplotlib.pyplot as plt; import pdb; 
import os; from datetime import datetime

# function definitions
def load_tensors(path):
    scatterfloor = torch.jit.load(path);
    scatterfloor = list(scatterfloor.parameters())[0];
    return scatterfloor
def get_current_time():
    now = datetime.now()
    return now.strftime("%H_%M_%S")

# setting paths
tensor_fls          = "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/decimated_signalMatrix_fls.pt";
tensor_port         = "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/decimated_signalMatrix_port.pt";
tensor_starboard    = "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/decimated_signalMatrix_starboard.pt";

# loading tensors
tensor_fls          = load_tensors(tensor_fls)
tensor_port         = load_tensors(tensor_port)
tensor_starboard    = load_tensors(tensor_starboard)

# putting tensors together
tensors_together = torch.cat((tensor_fls, 
                              tensor_port, 
                              tensor_starboard), 1);

# subsetting
tensors_together = tensors_together[0:-1:2, :];
print(f"torch.mean(tensors_together[:]) = {torch.mean(tensors_together[:])}")

# printing sizes
print(f"\t tensor_fls.shape = {tensor_fls.shape}")
print(f"\t tensor_port.shape = {tensor_port.shape}")
print(f"\t tensor_starboard.shape = {tensor_starboard.shape}")

# plotting
plt.figure(figsize=(10, 10))
plt.imshow(torch.abs(tensors_together), 
           cmap='viridis', 
           interpolation='nearest')
plt.title('Signal Matrix - Port')
plt.draw(); plt.pause(5);

# plt.tight_layout()
# # plt.show()
# plt.draw();
# plt.pause(5);
# # plt.savefig(os.path.join("/Users/vrsreeganesh/Documents/GitHub/AUV/Code/Python/Figures",
# #                       "signalMatrix_fls"+get_current_time()));




