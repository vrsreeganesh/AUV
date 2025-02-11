/* =====================================
Aim: Setup sea floor
======================================*/ 

// including headerfiles
#include <torch/torch.h>
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/include/ScattererClass.h"

// including functions
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Functions/fCreateHills.cpp"
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Functions/fCreateBoxes.cpp"

#ifndef DEVICE
    #define DEVICE          torch::kCPU
    // #define DEVICE          torch::kMPS
    // #define DEVICE          torch::kCUDA
#endif

// adding terrrain features
#define BOXES                   false
#define HILLS                   true
#define DEBUG_SEAFLOOR          false
#define SAVETENSORS_Seafloor    false
#define PLOT_SEAFLOOR           false

// functin that setups the sea-floor
void SeafloorSetup(ScattererClass* scatterers) {
    
    // sea-floor bounds
    int bed_width   = 100;  // width of the bed (x-dimension)
    int bed_length  = 100;  // length of the bed (y-dimension)
    
    // creating some tensors to pass. This is put outside to maintain scope
    torch::Tensor box_coordinates   = torch::zeros({3,1}).to(torch::kFloat).to(DEVICE);
    torch::Tensor box_reflectivity  = torch::zeros({1,1}).to(torch::kFloat).to(DEVICE); 

    // creating boxes
    if (BOXES)
        fCreateBoxes(bed_width, \
                    bed_length, \
                    box_coordinates, \
                    box_reflectivity);
    
    // scatter-intensity
    // int bed_width_density       = 100;  // density of points along x-dimension
    // int bed_length_density      = 100;  // density of points along y-dimension
    int bed_width_density       = 10;  // density of points along x-dimension
    int bed_length_density      = 10;  // density of points along y-dimension

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
    if(HILLS == true){

        // setting up hill parameters
        int num_hills   = 10;

        // setting up placement of hills
        torch::Tensor points2D  = torch::cat({X, Y}, 0);
        torch::Tensor min2D     = std::get<0>(torch::min(points2D, 1, true));
        torch::Tensor max2D     = std::get<0>(torch::max(points2D, 1, true));
        torch::Tensor hill_means = \
            min2D                                           \
            + torch::mul(torch::rand({2, num_hills}), \
                         max2D - min2D);

        // setting up hill dimensions
        torch::Tensor hill_dimensions_min = \
            torch::tensor({10, \
                           10, \
                           2}).reshape({3,1});
        torch::Tensor hill_dimensions_max = \
            torch::tensor({30, \
                           30, \
                           7}).reshape({3,1});
        torch::Tensor hill_dimensions = \
            hill_dimensions_min + \
            torch::mul(hill_dimensions_max - hill_dimensions_min, \
                       torch::rand({3, num_hills}));

        // calling the hill-creation function
        fCreateHills(hill_means,    \
                     hill_dimensions, \
                     points2D);

        // setting up floor reflectivity
        torch::Tensor floorScatter_reflectivity = \
            torch::ones({1, Y.numel()}).to(DEVICE);

        // populating the values of the incoming argument. 
        scatterers->coordinates     = points2D; // assigning coordinates
        scatterers->reflectivity    = floorScatter_reflectivity;// assigning reflectivity
    }
    else{
        
        // assigning flat heights
        torch::Tensor Z = torch::zeros({1, Y.numel()}).to(DEVICE);

        // setting up floor coordinates
        torch::Tensor floorScatter_coordinates    = torch::cat({X, Y, Z}, 0);
        torch::Tensor floorScatter_reflectivity   = torch::ones({1, Y.numel()}).to(DEVICE);

        // populating the values of the incoming argument. 
        scatterers->coordinates     = floorScatter_coordinates; // assigning coordinates
        scatterers->reflectivity    = floorScatter_reflectivity;// assigning reflectivity
    }

    // combining the values
    if(DEBUG_SEAFLOOR) std::cout<<"\t SeafloorSetup: line 166 \n";
    if(DEBUG_SEAFLOOR) {std::cout<<"\t scatterers->coordinates.shape = "; fPrintTensorSize(scatterers->coordinates);}
    if(DEBUG_SEAFLOOR) {std::cout<<"\t box_coordinates.shape = "; fPrintTensorSize(box_coordinates);}
    if(DEBUG_SEAFLOOR) {std::cout<<"\t scatterers->reflectivity.shape = "; fPrintTensorSize(scatterers->reflectivity);}
    if(DEBUG_SEAFLOOR) {std::cout<<"\t box_reflectivity = "; fPrintTensorSize(box_reflectivity);}


    // assigning values to the coordinates
    scatterers->coordinates     = torch::cat({scatterers->coordinates,  box_coordinates},   1);
    scatterers->reflectivity    = torch::cat({scatterers->reflectivity, box_reflectivity},  1);

    // saving tensors
    if(SAVETENSORS_Seafloor){
        torch::save(scatterers->coordinates, \
                    "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_gt.pt");
        std::cout<<"SeafloorSetup: Saved Seafloor "<<std::endl;
    }

}