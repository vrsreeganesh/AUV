/* =====================================
Aim: Setup sea floor
======================================*/ 
#include <torch/torch.h>
#include <cmath>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/include/ScattererClass.h"
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/include/TransmitterClass.h"

int main() {

    // Setting up transmitter
    float sampling_frequency  = 16e3;                       // sampling frequency
    float f1                  = 3e3;                        // first frequency of LFM
    float f2                  = 6e3;                        // second frequency of LFM
    float fc                  = (f1 + f2)/2;                // finding center-frequency
    float bandwidth           = std::abs(f2 - f1);  // bandwidth 
    float pulselength         = 0.2; 

    // building LFM 
    torch::Tensor timearray = torch::linspace(0, pulselength, floor(pulselength * sampling_frequency));
    float K                 = (f2 - f1)/pulselength;
    torch::Tensor Signal    = K * timearray;
    Signal                  = torch::mul((f1 + Signal), timearray);
    Signal                  = cos(Signal);


    // Setting up transmitter
    torch::Tensor location    = torch::zeros({3,1}); // location of transmitter (this will be overwritten by vessel)
    float azimuthal_angle     = 0;                   // initial pointing direction
    float elevation_angle     = -30;                 // initial pointing direction
    float azimuthal_beamwidth = 5;                   // azimuthal beamwidth of the signal cone
    float elevation_beamwidth = 5;                   // elevation beamwidth of the signal cone

    int azimuthQuantDensity     = 20;   // quantization density along azimuth (used for shadowing)
    int elevationQuantDensity   = 20;   // quantization density along elevation (used for shadowing)
    float rangeQuantSize        = 20;   // cell-dimension (used for shadowing)




    //  Creating and saving object of class
    // creating an obj of class
    TransmitterClass transmitter(location,
                                 Signal,
                                 azimuthal_angle,
                                 elevation_angle,
                                 azimuthal_beamwidth,
                                 elevation_beamwidth);

    // updating quantization densities
    transmitter.azimuthQuantDensity     = azimuthQuantDensity;
    transmitter.elevationQuantDensity   = elevationQuantDensity;
    transmitter.rangeQuantSize          = rangeQuantSize;

    transmitter.azimuthShadowThreshold      = 0.5;
    transmitter.elevationShadowThreshold    = 0.5;

    std::cout<<"transmittertesting"<<std::endl;
    std::cout<<transmitter<<std::endl;

    // signal related
    transmitter.f_low       = f1;           // assigning lower frequency
    transmitter.f_high      = f2;           // assigning higher frequency
    transmitter.fc          = fc;           // assigning center frequency
    transmitter.bandwidth   = bandwidth;    // assigning bandwidth











































    // // sea-floor bounds
    // int along_track_length  = 100; 
    // int across_track_length = 100;

    // // scatter-intensity
    // int along_track_scatter_density   = 10;
    // int across_track_scatter_density  = 10;

    // // setting up coordinates
    // auto xpoints = torch::linspace(0, \
    //                                 across_track_length, \
    //                                 across_track_length * across_track_scatter_density);
    // auto ypoints = torch::linspace(0, \
    //                                 along_track_length,  \
    //                                 along_track_length * along_track_scatter_density);

    // // creating mesh
    // auto mesh_grid  = torch::meshgrid({xpoints, ypoints}, "ij");
    // auto X          = mesh_grid[0]; 
    // auto Y          = mesh_grid[1];

    // X               = torch::reshape(X, {1, X.numel()});
    // Y               = torch::reshape(Y, {1, Y.numel()});
    // torch::Tensor Z = torch::zeros({1, Y.numel()});

    // // setting up floor coordinates
    // torch::Tensor floorScatter_coordinates    = torch::cat({X, Y, Z}, 0);
    // torch::Tensor floorScatter_reflectivity   = torch::ones({3, Y.numel()});

    // // initializing object of class
    // ScattererClass SeafloorScatter(floorScatter_coordinates,
    //                                 floorScatter_reflectivity);

    // // printing 
    // std::cout<<"SeafloorScatter = "<<std::endl;
    // std::cout<<SeafloorScatter;

    // returning
    return 0;
}