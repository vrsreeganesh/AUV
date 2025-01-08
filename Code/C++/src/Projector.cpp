#include <iostream>
#include <torch/torch.h>

class Projector{
public:

  // public members
  torch::Tensor location;       // location of projector
  double azimuthal_angle;       // azimuthal angle to which the projector is pointing
  double elevation_angle;       // elevation angle to which the projector is pointing
  double azimuthal_beamwidth;   // azimuthal beamwidth of the projected signal
  double vertical_beamwidth;    // vertical beamwidth of the projected signal

  // constructor
  Projector(torch::Tensor location = torch::zeros({3,1}),
            double azimuthal_angle = 0.0,
            double elevation_angle = 0.0,
            double azimuthal_beamwidth = 0.0,
            double vertical_beamwidth = 0.0)
            : location(location),
              azimuthal_angle(azimuthal_angle),
              elevation_angle(elevation_angle),
              azimuthal_beamwidth(azimuthal_beamwidth),
              vertical_beamwidth(vertical_beamwidth) {}

  // copy constructor
  Projector(const Projector& input_object){    
    // copying one by one
    location              = input_object.location;
    azimuthal_angle       = input_object.azimuthal_angle;
    elevation_angle       = input_object.elevation_angle;
    azimuthal_beamwidth   = input_object.azimuthal_beamwidth;
    vertical_beamwidth    = input_object.vertical_beamwidth;
  }

  // illumination function
  std::unordered_map<std::string, torch::Tensor> fIlluminate(std::unordered_map<std::string, torch::Tensor> inputScatterers){
    
    // setting up output
    std::unordered_map<std::string, torch::Tensor> outputScatterers = inputScatterers;

    // change origin
    outputScatterers["coordinates"] = inputScatterers["coordinates"] - location;

    // 
    return outputScatterers;
  };

};