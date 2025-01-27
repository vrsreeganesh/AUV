/* =====================================
Aim: Setup sea floor
======================================*/ 
#include <torch/torch.h>
#include <iostream>

// hash-defines
#define PI          3.14159265
#define MYDEBUGFLAG false

// bringing in functions
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Functions/fPrintTensorSize.cpp"


torch::Tensor fCart2Sph(torch::Tensor cartesian_vector){

    // splatting the point onto xy plane
    torch::Tensor xysplat = cartesian_vector.clone();
    xysplat[2] = 0;
    if (MYDEBUGFLAG)
        std::cout<<"\t fCart2Sph > reached line  15\n";
    
    // finding splat lengths
    torch::Tensor xysplat_lengths = torch::linalg_norm(xysplat, 2, 0, true, torch::kFloat);
    if (MYDEBUGFLAG)
        std::cout<<"\t fCart2Sph > reached line  19\n";
    
    // finding azimuthal and elevation angles
    torch::Tensor azimuthal_angles = torch::atan2(xysplat[1],           xysplat[0])      * 180/PI;
    azimuthal_angles = azimuthal_angles.reshape({1, azimuthal_angles.numel()});
    if (MYDEBUGFLAG)
        std::cout<<"\t fCart2Sph > reached line  23\n";
    torch::Tensor elevation_angles = torch::atan2(cartesian_vector[2],  xysplat_lengths) * 180/PI;
    if (MYDEBUGFLAG)
        std::cout<<"\t fCart2Sph > reached line  25\n";
    torch::Tensor rho_values       = torch::linalg_norm(cartesian_vector, 2, 0, true, torch::kFloat);
    if (MYDEBUGFLAG)
        std::cout<<"\t fCart2Sph > reached line  27\n";
    


    if (MYDEBUGFLAG){
        std::cout<<"azimuthal_angles.shape  = "; fPrintTensorSize(azimuthal_angles);
        std::cout<<"elevation_angles.shape  = "; fPrintTensorSize(elevation_angles);
        std::cout<<"rho_values.shape        = "; fPrintTensorSize(rho_values);
    }
    
    

    // creating tensor to send back
    torch::Tensor spherical_vector = torch::cat({azimuthal_angles, \
                                                 elevation_angles, \
                                                 rho_values}, 0);
    if (MYDEBUGFLAG)
        std::cout<<"\t fCart2Sph > reached line  33\n";

    // returning the value
    return spherical_vector;
    

}