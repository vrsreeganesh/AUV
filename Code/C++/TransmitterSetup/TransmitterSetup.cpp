/* =====================================
Aim: Setup sea floor
======================================*/ 
#include <torch/torch.h>
#include <cmath>
#include <iostream>
// #include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/include/ScattererClass.h"
// #include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/include/TransmitterClass.h"

// int main() {

//     // Setting up transmitter
//     float sampling_frequency  = 48e3;                       // sampling frequency
//     float f1                  = 3e3;                        // first frequency of LFM
//     float f2                  = 6e3;                        // second frequency of LFM
//     float fc                  = (f1 + f2)/2;                // finding center-frequency
//     float bandwidth           = std::abs(f2 - f1);  // bandwidth 
//     float pulselength         = 0.2; 

//     // building LFM 
//     torch::Tensor timearray = torch::linspace(0, pulselength, floor(pulselength * sampling_frequency));
//     float K                 = (f2 - f1)/pulselength;
//     torch::Tensor Signal    = K * timearray;
//     Signal                  = torch::mul((f1 + Signal), timearray);
//     Signal                  = cos(Signal);


//     // Setting up transmitter
//     torch::Tensor location    = torch::zeros({3,1}); // location of transmitter (this will be overwritten by vessel)
//     float azimuthal_angle     = 0;                   // initial pointing direction
//     float elevation_angle     = -30;                 // initial pointing direction
//     float azimuthal_beamwidth = 5;                   // azimuthal beamwidth of the signal cone
//     float elevation_beamwidth = 5;                   // elevation beamwidth of the signal cone
    
//     int azimuthQuantDensity     = 20;   // quantization density along azimuth (used for shadowing)
//     int elevationQuantDensity   = 20;   // quantization density along elevation (used for shadowing)
//     float rangeQuantSize        = 20;   // cell-dimension (used for shadowing)

//     // creating an obj of class
//     TransmitterClass transmitter(location,
//                                  Signal,
//                                  azimuthal_angle,
//                                  elevation_angle,
//                                  azimuthal_beamwidth,
//                                  elevation_beamwidth);

//     // updating quantization densities
//     transmitter.azimuthQuantDensity         = azimuthQuantDensity;
//     transmitter.elevationQuantDensity       = elevationQuantDensity;
//     transmitter.rangeQuantSize              = rangeQuantSize;
//     transmitter.azimuthShadowThreshold      = 0.5;
//     transmitter.elevationShadowThreshold    = 0.5;

//     // signal related
//     transmitter.f_low       = f1;           // assigning lower frequency
//     transmitter.f_high      = f2;           // assigning higher frequency
//     transmitter.fc          = fc;           // assigning center frequency
//     transmitter.bandwidth   = bandwidth;    // assigning bandwidth

//     // printing transmitter specifications
//     std::cout<<"transmittertesting"<<std::endl;
//     std::cout<<transmitter<<std::endl;

//     // returning
//     return 0;
// }

void TransmitterSetup(TransmitterClass* transmitter) {

    // Setting up transmitter
    float sampling_frequency  = 48e3;                       // sampling frequency
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

    // populating contents of the class
    transmitter->location = location;
    transmitter->Signal = Signal;
    transmitter->azimuthal_angle = azimuthal_angle;
    transmitter->elevation_angle = elevation_angle;
    transmitter->azimuthal_beamwidth = azimuthal_beamwidth;
    transmitter->elevation_beamwidth =elevation_beamwidth;

    // updating quantization densities
    transmitter->azimuthQuantDensity         = azimuthQuantDensity;
    transmitter->elevationQuantDensity       = elevationQuantDensity;
    transmitter->rangeQuantSize              = rangeQuantSize;
    transmitter->azimuthShadowThreshold      = 0.5;
    transmitter->elevationShadowThreshold    = 0.5;

    // signal related
    transmitter->f_low       = f1;           // assigning lower frequency
    transmitter->f_high      = f2;           // assigning higher frequency
    transmitter->fc          = fc;           // assigning center frequency
    transmitter->bandwidth   = bandwidth;    // assigning bandwidth

}