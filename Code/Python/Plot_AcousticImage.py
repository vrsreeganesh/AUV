# Testing the sea-floor created in C++ scripts

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
tensor_fls          = "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/Assets/acoustic_image.pt"
tensor_fls          = load_tensors(tensor_fls);







# plt.imshow(tensor_fls[:, 0:tensor_fls.shape[0]], 
#            cmap='viridis', 
#            interpolation='nearest')
plt.imshow(tensor_fls, 
           cmap='viridis', 
           interpolation='nearest')
plt.title('Random Data')
plt.colorbar()

plt.tight_layout()
# plt.show()
plt.draw();
plt.pause(10);
# plt.savefig(os.path.join("/Users/vrsreeganesh/Documents/GitHub/AUV/Code/Python/Figures",
#                       "signalMatrix_fls"+get_current_time()));




