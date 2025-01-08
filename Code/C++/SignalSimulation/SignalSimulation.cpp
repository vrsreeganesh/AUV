/*
========================================
Aim: Signal Simulation
========================================
*/ 

// including standard 
#include <ostream>
#include <torch/torch.h>
#include <iostream>
#include <fstream>
#include <unordered_map>

// including custom headers 
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/include/AUV.h"
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/include/Projector.h"
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/include/ULA.h"




// Projector Class ============================================================
// Projector Class ============================================================
// Projector Class ============================================================
// Projector Class ============================================================
// Projector Class ============================================================
// Projector Class ============================================================
// Projector Class ============================================================
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

// function for ULA ============================================================
// function for ULA ============================================================
// function for ULA ============================================================
// function for ULA ============================================================
// function for ULA ============================================================
// function for ULA ============================================================
// function for ULA ============================================================
class ULA{

public:
  // public members: public cause we want the AUV to edit this
  int num_sensors;                      // number of sensors
  double intersensor_distance;          // distance between sensors
  torch::Tensor first_sensor_location;  // location of first sensor
  torch::Tensor ula_sensor_direction;   // vector from first sensor to rest
  torch::Tensor ula_sensor_locations;   // location of sensors in the ULA

  // constructor
  ULA(int num_sensors                     = 32,
      double intersensor_distance         = 1e-3,
      torch::Tensor first_sensor_location = torch::zeros({3,1}),
      torch::Tensor ula_sensor_direction  = torch::zeros({3,1}))
      : num_sensors(num_sensors),
        intersensor_distance(intersensor_distance),
        first_sensor_location(first_sensor_location),
        ula_sensor_direction(ula_sensor_direction) {}

  // copy assignment operator
  ULA& operator=(const ULA& inputobject){
    num_sensors     = inputobject.num_sensors;
    intersensor_distance  = inputobject.intersensor_distance;
    first_sensor_location = inputobject.first_sensor_location;
    ula_sensor_direction  = inputobject.ula_sensor_direction;
    ula_sensor_locations  = inputobject.ula_sensor_locations;

    return *this;
  }


  // printing state
  friend std::ostream& operator<<(std::ostream& os, const ULA& ula){
    os<<"\t> num sensors = "<<ula.num_sensors<<std::endl;
    os<<"\t> intersensor distance = "<<ula.intersensor_distance<<std::endl;
    return os;
  }
};




// Class defintion for DSP =====================================================
// Class defintion for DSP =====================================================
// Class defintion for DSP =====================================================
// Class defintion for DSP =====================================================
// Class defintion for DSP =====================================================
class DSP{
public:

  // members
  int c;
  int chirp_start_frequency;
  float recording_time; 
  int sampling_frequency;
  float transmission_duration;
  float max_range;

  torch::Tensor timeArray;
  torch::Tensor transmissionSignal;

  // constructor
  DSP(int c                             = 1500,
      int chirp_start_frequency         = 100,
      float recording_time              = 2e-1, 
      int sampling_frequency            = 16e3,
      float transmission_duration       = 1e-2):
    c(c),
    chirp_start_frequency(chirp_start_frequency),
    recording_time(recording_time),
    sampling_frequency(sampling_frequency),
    transmission_duration(transmission_duration) {
      
      // dependent stuff
      max_range     = transmission_duration * c / 2;
      timeArray     = torch::arange(0, 
                                    transmission_duration + 1/sampling_frequency,
                                    1/sampling_frequency);

    }

};





// class definition for AUV ====================================================
// class definition for AUV ====================================================
// class definition for AUV ====================================================
// class definition for AUV ====================================================
// class definition for AUV ====================================================
// class definition for AUV ====================================================
class AUV{
private:
  // independent attributes
  torch::Tensor location;               // current location of AUV
  torch::Tensor velocity;               // velocity of AUV
  torch::Tensor acceleration;           // acceleration of AUV
  torch::Tensor pointing_direction;     // direction in which AUV is pointed

public:

  // Projectors
  Projector projector_portside;         // Projector to the left of the AUV
  Projector projector_fbls;             // projector to the front of the AUV
  Projector projector_starboard;        // projector to the right of the AUV

  // ULAs
  ULA ula_portside;   // ULA mounted on the left-side
  ULA ula_fbls;       // ULA mounted on the front-side
  ULA ula_starboard;  // ULA mounted on the right-side
  
  // constructor
  AUV(torch::Tensor location            = torch::zeros({3,1}),
      torch::Tensor velocity            = torch::zeros({3,1}),
      torch::Tensor acceleration        = torch::zeros({3,1}),
      torch::Tensor pointing_direction  = torch::zeros({3,1}))
      : location(location),
        velocity(velocity),
        acceleration(acceleration),
        pointing_direction(pointing_direction) {}

  // printing
  friend std::ostream& operator<<(std::ostream& os, const AUV& auv){
    os<<"location = \n"<<auv.location<<std::endl;
    os<<"velocity = \n"<<auv.velocity<<std::endl;
    os<<"acceleration = \n"<<auv.acceleration<<std::endl;
    os<<"pointing_direction = \n"<<auv.pointing_direction<<std::endl;
    return os;
  }

  // synchronizing 
  void _fAttach(){
    // attach ULA:portside
    ula_portside.first_sensor_location  = location;
    ula_portside.ula_sensor_direction   = -1 * pointing_direction;

    // attach ULA:fbls
    ula_fbls.first_sensor_location      = location;
    ula_fbls.ula_sensor_direction       = -1 * pointing_direction;

    // attach ULA:starboard
    ula_starboard.first_sensor_location = location;
    ula_starboard.ula_sensor_direction  = -1 * pointing_direction;

    // attach projector:portside
    projector_portside.location         = location;

    // attach projector:fbls
    projector_fbls.location             = location;

    // attach projector:starboard
    projector_starboard.location        = location;

  }

  // propagate signal

  // step function
  void step(float timestep = 1e-3){
    /*
    Performing every step thing done here. 
    */

    // synchronizing locations
    _fAttach();

    // project signals from the projectors

    // receive signals


    // updating parameters of AUVs
    location = location + \
               velocity * timestep + \
               0.5 * acceleration * timestep * timestep;
    velocity = velocity + acceleration * timestep;

    // synchronizing locations
    _fAttach();

  }
};

// function to print tensor size
void print_tensor_size(const torch::Tensor& inputTensor) {
    // Printing size
    std::cout << "Tensor size: ";
    for (const auto& size : inputTensor.sizes()) {
        std::cout << size << " ";
    }
    std::cout << std::endl;
}


// converting from degrees to radian
#define PRINTLINE std::cout<<"================================================"<<std::endl;
#define PI 3.14159265
float fDeg2Rad(float deg){
  float deg_value = static_cast<float>(PI * deg / 180);
  return deg_value;
}

int main() {

  // encapsulating coordinates and reflectivity in a dictionary
  std::unordered_map<std::string, torch::Tensor> floor_scatterers;
  torch::load(floor_scatterers["coordinates"],
              "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/floor_coordinates_3D.pt");
  torch::load(floor_scatterers["reflectivity"], 
              "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/floor_scatterers_reflectivity.pt");


  // AUV Setup
  torch::Tensor auv_initial_location            = torch::tensor({0.0, 2.0, 2.0}).view({3,1});
  torch::Tensor auv_initial_velocity            = torch::tensor({1.0, 0.0, 0.0}).view({3,1});
  torch::Tensor auv_initial_acceleration        = torch::tensor({0.0, 0.0, 0.0}).view({3,1});
  torch::Tensor auv_initial_pointing_direction  = torch::tensor({1.0, 0.0, 0.0}).view({3,1});
  

  // Initializing a member of class, AUV
  AUV auv(auv_initial_location,
          auv_initial_velocity,
          auv_initial_acceleration,
          auv_initial_pointing_direction);


  // Setting up ULAs for the AUV: front, portside and starboard
  const int num_sensors             = 32;
  const double intersensor_distance = 1e-4;
  ULA ula_portside(num_sensors, intersensor_distance);      // Initializing ULA objects
  ULA ula_fbls(num_sensors, intersensor_distance);
  ULA ula_starboard(num_sensors, intersensor_distance);
  auv.ula_portside  = ula_portside;     // Attaching ULAs to the AUVs
  auv.ula_fbls      = ula_fbls;
  auv.ula_starboard = ula_starboard;


  // Setting up Projector: front, portside and starboard
  Projector projector_portside(torch::zeros({3,1}),         // location
                               fDeg2Rad(90),                // azimuthal angle
                               fDeg2Rad(-30),               // elevation angle
                               fDeg2Rad(30),                // azimuthal beamwidth
                               fDeg2Rad(20));               // elevation beamwidth
  Projector projector_fbls(torch::zeros({3,1}),   // location
                           fDeg2Rad(0),           // azimuthal angle
                           fDeg2Rad(-30),         // elevation angle
                           fDeg2Rad(120),         // azimuthal beamwidth
                           fDeg2Rad(60));         // elevation beamwidth;
  Projector projector_starboard(torch::zeros({3,1}),        // location
                                fDeg2Rad(-90),              // azimuthal angle
                                fDeg2Rad(-30),              // elevation angle
                                fDeg2Rad(30),               // azimuthal beamwidth
                                fDeg2Rad(20));              // elevation beamwidth;


  // Attaching projectors to AUV
  auv.projector_portside  = projector_portside;
  auv.projector_fbls      = projector_fbls;
  auv.projector_starboard = projector_starboard;

  


  // returning
  return 0;
}