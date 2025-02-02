/* =====================================
Aim: Setup sea floor
======================================*/ 
#include <torch/torch.h>
#include <cmath>

#ifndef DEVICE
    // #define DEVICE          torch::kMPS
    #define DEVICE          torch::kCPU
#endif



// function to calibrate the transmitters
void TransmitterSetup(TransmitterClass* transmitter_fls,
                      TransmitterClass* transmitter_port,
                      TransmitterClass* transmitter_starboard) {

    // Setting up transmitter
    float sampling_frequency  = 160e3;                      // sampling frequency
    float f1                  = 50e3;                       // first frequency of LFM
    float f2                  = 70e3;                       // second frequency of LFM
    float fc                  = (f1 + f2)/2;                // finding center-frequency
    float bandwidth           = std::abs(f2 - f1);  // bandwidth 
    float pulselength         = 0.2;                        // time of recording

    // building LFM 
    torch::Tensor timearray = torch::linspace(0, \
                                              pulselength, \
                                              floor(pulselength * sampling_frequency)).to(DEVICE);
    float K                 = (f2 - f1)/pulselength;                // calculating frequency-slope
    torch::Tensor Signal    = K * timearray;                        // frequency at each time-step, with f1 = 0
    Signal                  = torch::mul(2*PI*(f1 + Signal), \
                                         timearray);                // creating 
    Signal                  = cos(Signal);                          // calculating signal


    // Setting up transmitter
    torch::Tensor location                  = torch::zeros({3,1}).to(DEVICE);  // location of transmitter
    float azimuthal_angle_fls               = 0;                  // initial pointing direction
    float azimuthal_angle_port              = 90;                  // initial pointing direction
    float azimuthal_angle_starboard         = -90;                    // initial pointing direction
    
    float elevation_angle                   = -60;                  // initial pointing direction
    
    float azimuthal_beamwidth_fls           = 20;                   // azimuthal beamwidth of the signal cone
    float azimuthal_beamwidth_port          = 20;                   // azimuthal beamwidth of the signal cone
    float azimuthal_beamwidth_starboard     = 20;                   // azimuthal beamwidth of the signal cone
    
    float elevation_beamwidth_fls           = 20;                   // elevation beamwidth of the signal cone
    float elevation_beamwidth_port          = 20;                   // elevation beamwidth of the signal cone
    float elevation_beamwidth_starboard     = 20;                   // elevation beamwidth of the signal cone
    
    int azimuthQuantDensity         = 10;   // number of points, a degree is split into quantization density along azimuth (used for shadowing)
    int elevationQuantDensity       = 10;   // number of points, a degree is split into quantization density along elevation (used for shadowing)
    float rangeQuantSize            = 10;   // the length of a cell (used for shadowing)

    float azimuthShadowThreshold    = 1;      // azimuth threshold      (in degrees)
    float elevationShadowThreshold  = 1;      // elevation threshold    (in degrees)



    // transmitter-fls
    transmitter_fls->location                   = location;              // Assigning location
    transmitter_fls->Signal                     = Signal;                // Assigning signal
    transmitter_fls->azimuthal_angle            = azimuthal_angle_fls;  // assigning azimuth angle
    transmitter_fls->elevation_angle            = elevation_angle;       // assigning elevation angle
    transmitter_fls->azimuthal_beamwidth        = azimuthal_beamwidth_fls;   // assigning azimuth-beamwidth
    transmitter_fls->elevation_beamwidth        = elevation_beamwidth_fls;   // assigning elevation-beamwidth
    // updating quantization densities
    transmitter_fls->azimuthQuantDensity        = azimuthQuantDensity;        // assigning azimuth quant density
    transmitter_fls->elevationQuantDensity      = elevationQuantDensity;      // assigning elevation quant density
    transmitter_fls->rangeQuantSize             = rangeQuantSize;             // assigning range-quantization
    transmitter_fls->azimuthShadowThreshold     = azimuthShadowThreshold;     // azimuth-threshold in shadowing
    transmitter_fls->elevationShadowThreshold   = elevationShadowThreshold;   // elevation-threshold in shadowing
    // signal related
    transmitter_fls->f_low                      = f1;           // assigning lower frequency
    transmitter_fls->f_high                     = f2;           // assigning higher frequency
    transmitter_fls->fc                         = fc;           // assigning center frequency
    transmitter_fls->bandwidth                  = bandwidth;    // assigning bandwidth



    // transmitter-portside
    transmitter_port->location                  = location;                     // Assigning location
    transmitter_port->Signal                    = Signal;                       // Assigning signal
    transmitter_port->azimuthal_angle           = azimuthal_angle_port;         // assigning azimuth angle
    transmitter_port->elevation_angle           = elevation_angle;              // assigning elevation angle
    transmitter_port->azimuthal_beamwidth       = azimuthal_beamwidth_port;     // assigning azimuth-beamwidth
    transmitter_port->elevation_beamwidth       = elevation_beamwidth_port;     // assigning elevation-beamwidth
    // updating quantization densities
    transmitter_port->azimuthQuantDensity       = azimuthQuantDensity;          // assigning azimuth quant density
    transmitter_port->elevationQuantDensity     = elevationQuantDensity;        // assigning elevation quant density
    transmitter_port->rangeQuantSize            = rangeQuantSize;               // assigning range-quantization
    transmitter_port->azimuthShadowThreshold    = azimuthShadowThreshold;       // azimuth-threshold in shadowing
    transmitter_port->elevationShadowThreshold  = elevationShadowThreshold;     // elevation-threshold in shadowing
    // signal related
    transmitter_port->f_low                     = f1;                           // assigning lower frequency
    transmitter_port->f_high                    = f2;                           // assigning higher frequency
    transmitter_port->fc                        = fc;                           // assigning center frequency
    transmitter_port->bandwidth                 = bandwidth;                    // assigning bandwidth



    // transmitter-starboard
    transmitter_starboard->location                     = location;                     // assigning location
    transmitter_starboard->Signal                       = Signal;                       // assigning signal
    transmitter_starboard->azimuthal_angle              = azimuthal_angle_starboard;    // assigning azimuthal signal
    transmitter_starboard->elevation_angle              = elevation_angle;
    transmitter_starboard->azimuthal_beamwidth          = azimuthal_beamwidth_starboard;
    transmitter_starboard->elevation_beamwidth          = elevation_beamwidth_starboard;
    // updating quantization densities
    transmitter_starboard->azimuthQuantDensity          = azimuthQuantDensity;
    transmitter_starboard->elevationQuantDensity        = elevationQuantDensity;
    transmitter_starboard->rangeQuantSize               = rangeQuantSize;
    transmitter_starboard->azimuthShadowThreshold       = azimuthShadowThreshold;
    transmitter_starboard->elevationShadowThreshold     = elevationShadowThreshold;
    // signal related
    transmitter_starboard->f_low                        = f1;           // assigning lower frequency
    transmitter_starboard->f_high                       = f2;           // assigning higher frequency
    transmitter_starboard->fc                           = fc;           // assigning center frequency
    transmitter_starboard->bandwidth                    = bandwidth;    // assigning bandwidth

}