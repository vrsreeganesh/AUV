
// including headerfiles
#include <stdexcept>
#include <torch/torch.h>

#pragma once

#define DEBUG_SEAFLOOR          false


// Adding boxes
void fCreateBoxes(float across_track_length, \
                  float along_track_length, \
                  torch::Tensor& box_coordinates,\
                  torch::Tensor& box_reflectivity){

    // setting up parameters
    float min_width           = 2;      // minimum across-track dimension of the boxes in the sea-floor
    float max_width           = 5;     // maximum across-track dimension of the boxes in the sea-floor
    
    float min_length          = 2;      // minimum along-track dimension of the boxes in the sea-floor
    float max_length          = 5;     // maximum along-track dimension of the boxes in the sea-floor
    
    float min_height          = 3;     // minimum height of the boxes in the sea-floor
    float max_height          = 20;     // maximum height of the boxes in the sea-floor
    
    int meshdensity           = 10;      // number of points per meter. 
    float meshreflectivity    = 2;      // average reflectivity of the mesh
    
    int num_boxes             = 10;     // number of boxes in the sea-floor
    if(DEBUG_SEAFLOOR) std::cout<<"\t fCreateBoxes: line 41\n";

    // finding center point
    torch::Tensor midxypoints   = torch::rand({3, num_boxes}).to(torch::kFloat).to(DEVICE);
    midxypoints[0]              = midxypoints[0] * across_track_length;
    midxypoints[1]              = midxypoints[1] * along_track_length;
    midxypoints[2]              = 0;
    if(DEBUG_SEAFLOOR) std::cout<<"\t fCreateBoxes: line 48\n";

    // assigning dimensions to boxes
    torch::Tensor boxwidths  = torch::rand({num_boxes})*(max_width     - min_width)  + min_width;    // assigning widths to each boxes
    torch::Tensor boxlengths = torch::rand({num_boxes})*(max_length    - min_length) + min_length;   // assigning lengths to each boxes 
    torch::Tensor boxheights = torch::rand({num_boxes})*(max_height    - min_height) + min_height;   // assigning heights to each boxes
    if(DEBUG_SEAFLOOR) std::cout<<"\t fCreateBoxes: line 54\n";

    // creating mesh for each box
    for(int i = 0; i<num_boxes; ++i){

        // finding x-points
        torch::Tensor xpoints = torch::linspace(-boxwidths[i].item<float>()/2, \
                                                 boxwidths[i].item<float>()/2, \
                                                 (int)(boxwidths[i].item<float>() * meshdensity));
        torch::Tensor ypoints = torch::linspace(-boxlengths[i].item<float>()/2, \
                                                 boxlengths[i].item<float>()/2, \
                                                 (int)(boxlengths[i].item<float>() * meshdensity));
        torch::Tensor zpoints = torch::linspace(0, \
                                                boxheights[i].item<float>(),\
                                                (int)(boxheights[i].item<float>() * meshdensity));
        if(DEBUG_SEAFLOOR) std::cout<<"\t fCreateBoxes: line 69\n";

        // meshgridding 
        auto mesh_grid  = torch::meshgrid({xpoints, ypoints, zpoints}, "xy");
        auto X          = mesh_grid[0];                                 
        auto Y          = mesh_grid[1];
        auto Z          = mesh_grid[2];
        X               = torch::reshape(X, {1, X.numel()});
        Y               = torch::reshape(Y, {1, Y.numel()});
        Z               = torch::reshape(Z, {1, Z.numel()});
        if(DEBUG_SEAFLOOR) std::cout<<"\t fCreateBoxes: line 79\n";

        // coordinates
        torch::Tensor boxcoordinates = torch::cat({X, Y, Z}, 0).to(DEVICE);
        boxcoordinates[0]  = boxcoordinates[0] + midxypoints[0][i];
        boxcoordinates[1]  = boxcoordinates[1] + midxypoints[1][i];
        boxcoordinates[2]  = boxcoordinates[2] + midxypoints[2][i];
        if(DEBUG_SEAFLOOR) std::cout<<"\t fCreateBoxes: line 86\n";

        // creating some reflectivity points too. 
        torch::Tensor boxreflectivity = meshreflectivity + torch::rand({1, boxcoordinates[0].numel()}) - 0.5;
        boxreflectivity = boxreflectivity.to(DEVICE);
        if(DEBUG_SEAFLOOR) std::cout<<"\t fCreateBoxes: line 90\n";

        // adding to larger matrices
        if(DEBUG_SEAFLOOR) {std::cout<<"box_coordinates.shape = ";  fPrintTensorSize(box_coordinates);}
        if(DEBUG_SEAFLOOR) {std::cout<<"box_coordinates.shape = ";  fPrintTensorSize(boxcoordinates);}

        if(DEBUG_SEAFLOOR) {std::cout<<"box_reflectivity.shape = "; fPrintTensorSize(box_reflectivity);}
        if(DEBUG_SEAFLOOR) {std::cout<<"boxreflectivity.shape = ";  fPrintTensorSize(boxreflectivity);}

        box_coordinates     = torch::cat({box_coordinates.to(DEVICE),  boxcoordinates},    1);
        if(DEBUG_SEAFLOOR) std::cout<<"\t fCreateBoxes: line 95\n";
        box_reflectivity    = torch::cat({box_reflectivity.to(DEVICE), boxreflectivity},   1);
        if(DEBUG_SEAFLOOR) std::cout<<"\t fCreateBoxes: line 97\n";
    }
}
