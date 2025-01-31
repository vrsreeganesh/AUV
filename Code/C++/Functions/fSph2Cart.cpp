/* =====================================
Aim: Setup sea floor
======================================*/ 
#include <torch/torch.h>

#pragma once

// hash-defines
#define PI          3.14159265
#define MYDEBUGFLAG false

#ifndef DEVICE
    // #define DEVICE          torch::kMPS
    #define DEVICE          torch::kCPU
#endif


torch::Tensor fSph2Cart(torch::Tensor spherical_vector){



    // sending argument to device
    spherical_vector = spherical_vector.to(DEVICE);

    // creating cartesian vector
    torch::Tensor cartesian_vector = torch::zeros({3,(int)(spherical_vector.numel()/3)}).to(torch::kFloat).to(DEVICE);

    // populating it
    cartesian_vector[0] = spherical_vector[2] * \
                          torch::cos(spherical_vector[1] * PI/180) * \
                          torch::cos(spherical_vector[0] * PI/180);
    cartesian_vector[1] = spherical_vector[2] * \
                          torch::cos(spherical_vector[1] * PI/180) * \
                          torch::sin(spherical_vector[0] * PI/180);
    cartesian_vector[2] = spherical_vector[2] * \
                          torch::sin(spherical_vector[1] * PI/180);

    // returning the value
    return cartesian_vector;
}