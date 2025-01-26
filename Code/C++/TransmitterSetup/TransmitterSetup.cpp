/* =====================================
Aim: Setup sea floor
======================================*/ 
#include <torch/torch.h>
#include <cmath>

// #include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/include/ScattererClass.h"
// #include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/include/TransmitterClass.h"

void TransmitterSetup(TransmitterClass* transmitter_port, 
                      TransmitterClass* transmitter_starboard) {

    // Setting up transmitter
    float sampling_frequency  = 160e3;                      // sampling frequency
    float f1                  = 50e3;                       // first frequency of LFM
    float f2                  = 70e3;                       // second frequency of LFM
    float fc                  = (f1 + f2)/2;                // finding center-frequency
    float bandwidth           = std::abs(f2 - f1);  // bandwidth 
    float pulselength         = 0.2;                        // time of recording

    // building LFM 
    torch::Tensor timearray = torch::linspace(0, pulselength, floor(pulselength * sampling_frequency));
    float K                 = (f2 - f1)/pulselength;
    torch::Tensor Signal    = K * timearray;
    Signal                  = torch::mul((f1 + Signal), timearray);
    Signal                  = cos(Signal);


    // Setting up transmitter
    torch::Tensor location          = torch::zeros({3,1});  // location of transmitter
    float azimuthal_angle_port      = 180;                  // initial pointing direction
    float azimuthal_angle_starboard = 0;                    // initial pointing direction
    float elevation_angle           = -30;                  // initial pointing direction
    float azimuthal_beamwidth       = 30;                   // azimuthal beamwidth of the signal cone
    float elevation_beamwidth       = 60;                   // elevation beamwidth of the signal cone
    float azimuthShadowThreshold    = 0.5;                  // azimuth threshold           
    float elevationShadowThreshold  = 0.5;                  // elevation threshold
    
    int azimuthQuantDensity     = 20;   // quantization density along azimuth (used for shadowing)
    int elevationQuantDensity   = 20;   // quantization density along elevation (used for shadowing)
    float rangeQuantSize        = 20;   // cell-dimension (used for shadowing)

    // populating contents of the class
    transmitter_port->location             = location;              // Assigning location
    transmitter_port->Signal               = Signal;                // Assigning signal
    transmitter_port->azimuthal_angle      = azimuthal_angle_port;  // assigning azimuth angle
    transmitter_port->elevation_angle      = elevation_angle;       // assigning elevation angle
    transmitter_port->azimuthal_beamwidth  = azimuthal_beamwidth;   // assigning azimuth-beamwidth
    transmitter_port->elevation_beamwidth  = elevation_beamwidth;   // assigning elevation-beamwidth

    // updating quantization densities
    transmitter_port->azimuthQuantDensity         = azimuthQuantDensity;        // assigning azimuth quant density
    transmitter_port->elevationQuantDensity       = elevationQuantDensity;      // assigning elevation quant density
    transmitter_port->rangeQuantSize              = rangeQuantSize;             // assigning range-quantization
    transmitter_port->azimuthShadowThreshold      = azimuthShadowThreshold;     // azimuth-threshold in shadowing
    transmitter_port->elevationShadowThreshold    = elevationShadowThreshold;   // elevation-threshold in shadowing

    // signal related
    transmitter_port->f_low       = f1;           // assigning lower frequency
    transmitter_port->f_high      = f2;           // assigning higher frequency
    transmitter_port->fc          = fc;           // assigning center frequency
    transmitter_port->bandwidth   = bandwidth;    // assigning bandwidth

    // populating contents of the class
    transmitter_starboard->location             = location;
    transmitter_starboard->Signal               = Signal;
    transmitter_starboard->azimuthal_angle      = azimuthal_angle_starboard;
    transmitter_starboard->elevation_angle      = elevation_angle;
    transmitter_starboard->azimuthal_beamwidth  = azimuthal_beamwidth;
    transmitter_starboard->elevation_beamwidth  = elevation_beamwidth;

    // updating quantization densities
    transmitter_starboard->azimuthQuantDensity         = azimuthQuantDensity;
    transmitter_starboard->elevationQuantDensity       = elevationQuantDensity;
    transmitter_starboard->rangeQuantSize              = rangeQuantSize;
    transmitter_starboard->azimuthShadowThreshold      = azimuthShadowThreshold;
    transmitter_starboard->elevationShadowThreshold    = elevationShadowThreshold;

    // signal related
    transmitter_starboard->f_low       = f1;           // assigning lower frequency
    transmitter_starboard->f_high      = f2;           // assigning higher frequency
    transmitter_starboard->fc          = fc;           // assigning center frequency
    transmitter_starboard->bandwidth   = bandwidth;    // assigning bandwidth

}