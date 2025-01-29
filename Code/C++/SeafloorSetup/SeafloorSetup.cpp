/* =====================================
Aim: Setup sea floor
======================================*/ 
#include <torch/torch.h>
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/include/ScattererClass.h"

#ifndef DEVICE
    // #define DEVICE          torch::kMPS
    #define DEVICE          torch::kCPU
#endif


void SeafloorSetup(ScattererClass* scatterers) {
    
    // sea-floor bounds
    int bed_width   = 100;  // width of the bed (x-dimension)
    int bed_length  = 100;  // length of the bed (y-dimension)
    
    // scatter-intensity
    int bed_width_density       = 100;  // density of points along x-dimension
    int bed_length_density      = 100;  // density of points along y-dimension

    // setting up coordinates
    auto xpoints = torch::linspace(0, \
                                    bed_width, \
                                    bed_width * bed_width_density).to(DEVICE);
    auto ypoints = torch::linspace(0, \
                                    bed_length,  \
                                    bed_length * bed_length_density).to(DEVICE);

    // creating mesh
    auto mesh_grid  = torch::meshgrid({xpoints, ypoints}, "ij");    
    auto X          = mesh_grid[0];                                 
    auto Y          = mesh_grid[1];
    X               = torch::reshape(X, {1, X.numel()});
    Y               = torch::reshape(Y, {1, Y.numel()});
    
    // creating heights of scattereres
    torch::Tensor Z = torch::zeros({1, Y.numel()}).to(DEVICE);

    // setting up floor coordinates
    torch::Tensor floorScatter_coordinates    = torch::cat({X, Y, Z}, 0);
    torch::Tensor floorScatter_reflectivity   = torch::ones({3, Y.numel()}).to(DEVICE);

    // populating the values of the incoming argument. 
    scatterers->coordinates     = floorScatter_coordinates; // assigning coordinates
    scatterers->reflectivity    = floorScatter_reflectivity;// assigning reflectivity
}