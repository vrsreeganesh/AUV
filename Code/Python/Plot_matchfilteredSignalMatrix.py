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
asset_folder        = "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets"
tensor_fls          = os.path.join(asset_folder, "matchfiltered_signalMatrix_fls.pt"        )
tensor_port         = os.path.join(asset_folder, "matchfiltered_signalMatrix_port.pt"       )
tensor_starboard    = os.path.join(asset_folder, "matchfiltered_signalMatrix_starboard.pt"  )

# loading tensors
tensor_fls          = load_tensors(tensor_fls)
tensor_port         = load_tensors(tensor_port)
tensor_starboard    = load_tensors(tensor_starboard)

# putting tensors together
tensors_together = torch.cat((tensor_fls, 
                              tensor_port, 
                              tensor_starboard), 1);

# subsetting
tensors_together = tensors_together[0:-1:10, :];
print(f"torch.mean(tensors_together[:]) = {torch.mean(tensors_together[:])}")

# scaling the image
tensors_together_abs = torch.abs(tensors_together)
tensors_together_abs = tensors_together_abs - torch.min(torch.abs(tensors_together_abs));
tensors_together_abs = tensors_together_abs/torch.max(tensors_together_abs);

# plotting
plt.figure(figsize=(10, 10))
# plt.imshow(torch.real(tensors_together), 
#            cmap='viridis', 
#            interpolation='nearest')
plt.imshow(tensors_together_abs, 
           cmap='viridis', 
           interpolation='nearest')
plt.title('match-filtered Signal Matrix')
plt.draw(); plt.pause(5);

# plt.tight_layout()
# # plt.show()
# plt.draw();
# plt.pause(5);
# # plt.savefig(os.path.join("/Users/vrsreeganesh/Documents/GitHub/AUV/Code/Python/Figures",
# #                       "signalMatrix_fls"+get_current_time()));




