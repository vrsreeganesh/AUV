/* =====================================
Aim: Setup sea floor
======================================*/ 
#include <torch/torch.h>
#include <iostream>

// hash-defines
#define PI          3.14159265
#define MYDEBUGFLAG false

#ifndef DEVICE
    #define DEVICE          torch::kMPS
    // #define DEVICE          torch::kCPU
#endif


// bringing in functions
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Functions/fPrintTensorSize.cpp"


torch::Tensor fCart2Sph(torch::Tensor cartesian_vector){

    // sending argument to the device
    cartesian_vector = cartesian_vector.to(DEVICE);

    // splatting the point onto xy plane 
    torch::Tensor xysplat = cartesian_vector.clone().to(DEVICE);
    xysplat[2] = 0;
    
    // finding splat lengths
    torch::Tensor xysplat_lengths = torch::linalg_norm(xysplat, 2, 0, true, torch::kFloat).to(DEVICE);
    
    // finding azimuthal and elevation angles
    torch::Tensor azimuthal_angles = torch::atan2(xysplat[1],           xysplat[0]).to(DEVICE)      * 180/PI;
    azimuthal_angles               = azimuthal_angles.reshape({1, azimuthal_angles.numel()});
    torch::Tensor elevation_angles = torch::atan2(cartesian_vector[2],  xysplat_lengths).to(DEVICE) * 180/PI;
    torch::Tensor rho_values       = torch::linalg_norm(cartesian_vector, 2, 0, true, torch::kFloat).to(DEVICE);
    

    // printing values for debugging
    if (MYDEBUGFLAG){
        std::cout<<"azimuthal_angles.shape  = "; fPrintTensorSize(azimuthal_angles);
        std::cout<<"elevation_angles.shape  = "; fPrintTensorSize(elevation_angles);
        std::cout<<"rho_values.shape        = "; fPrintTensorSize(rho_values);
    }
    
    // creating tensor to send back
    torch::Tensor spherical_vector = torch::cat({azimuthal_angles, \
                                                 elevation_angles, \
                                                 rho_values}, 0).to(DEVICE);

    // returning the value
    return spherical_vector;
}