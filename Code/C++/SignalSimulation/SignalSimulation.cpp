/*
========================================
Aim: Signal Simulation
========================================
*/ 
#include <ostream>
#include <torch/torch.h>
#include <iostream>
#include <fstream>
// #include <unordered_map>


// Projector Class
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
};

// function for ULA
class ULA{
private:
  // member attributes
  int num_sensors;                      // number of sensors
  double intersensor_distance;          // distance between sensors

public:
  // public members: public cause we want the AUV to edit this
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

  // // function to calculate sensor locations
  // void fUpdateSensorLocations(){
  //   // 
  //   ;
  // }

  // printing state
  friend std::ostream& operator<<(std::ostream& os, const ULA& ula){
    os<<"\t> num sensors = "<<ula.num_sensors<<std::endl;
    os<<"\t> intersensor distance = "<<ula.intersensor_distance<<std::endl;
    return os;
  }
};


// class definition for AUV
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
        pointing_direction(pointing_direction) {
          
          // setting up projector's dependent quantities
          projector_portside.location   = location;
          projector_fbls.location       = location;
          projector_starboard.location  = location;

          // setting up ULA's dependent quantities


        }

  // printing
  friend std::ostream& operator<<(std::ostream& os, const AUV& auv){
    os<<"location = \n"<<auv.location<<std::endl;
    os<<"velocity = \n"<<auv.velocity<<std::endl;
    os<<"acceleration = \n"<<auv.acceleration<<std::endl;
    os<<"pointing_direction = \n"<<auv.pointing_direction<<std::endl;
    return os;
  }

  // step function
  void step(float timestep = 1e-3){
    /*
    Performing every step thing done here. 
    */

    // updating parameters of AUVs
    location = location + \
               velocity * timestep + \
               0.5 * acceleration * timestep * timestep;
    velocity = velocity + acceleration * timestep;

    // updating parameters of projectors

    // updating parameters of ULAs

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

// printing line
void printline(){
  std::cout<<"=============================================="<<std::endl;
}

int main() {
  // encapsulating coordinates and reflectivity in a dictionary
  std::unordered_map<std::string, torch::Tensor> floor_scatterers;
  torch::load(floor_scatterers["coordinates"],
              "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/floor_coordinates_3D.pt");
  torch::load(floor_scatterers["reflectivity"], 
              "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/floor_scatterers_reflectivity.pt");



  // AUV Setup
  torch::Tensor auv_initial_location = torch::tensor({0.0, 
                                                      2.0, 
                                                      2.0}).view({3,1}); // auv's initial location
  torch::Tensor auv_initial_velocity = torch::tensor({1.0, 
                                                      0.0, 
                                                      0.0}).view({3,1}); // auv's initial velocity
  torch::Tensor auv_initial_acceleration = torch::tensor({0.0, 
                                                          0.0, 
                                                          0.0}).view({3,1}); // auv's initial acceleration
  torch::Tensor auv_initial_pointing_direction = torch::tensor({1.0, 
                                                                0.0, 
                                                                0.0}).view({3,1}); // auv's initial pointing direction
  

  // Initializing a member of class, AUV
  AUV auv(auv_initial_location,
          auv_initial_velocity,
          auv_initial_acceleration,
          auv_initial_pointing_direction);

  // printing running
  printline();
  std::cout<<auv<<std::endl;
  printline();
  auv.step(1.0);
  std::cout<<auv<<std::endl;
  printline();


  // Setting up ULAs
  const int num_sensors             = 32;
  const double intersensor_distance = 1e-3; 

  


  ULA test(num_sensors, intersensor_distance);

  std::cout<<"test = "<<std::endl;
  std::cout<<test<<std::endl;



  // // setting up flat sea-floor
  // double del_steps  = 1.0;
  // double x_steps    = del_steps;
  // double y_steps    = del_steps;

  // // floor bounds
  // double x_begin  = 0.0; 
  // double x_end    = 4.0;
  // double y_begin  = 0.0;
  // double y_end    = 4.0;

  // // setting up coordinates
  // torch::Tensor x_flat_coordinates = torch::arange(x_begin, 
  //                                                  x_end + x_steps, 
  //                                                  x_steps);
  // torch::Tensor y_flat_coordinates = torch::arange(y_begin, 
  //                                                  y_end + y_steps, 
  //                                                  y_steps);
  // auto grids  = torch::meshgrid({x_flat_coordinates, y_flat_coordinates}, "xy");
  // torch::Tensor x_temp = grids[0];
  // torch::Tensor y_temp = grids[1];
  // y_temp = torch::flip(y_temp, {0});    // flipping the tensor

  // // flattening the tensor
  // x_temp  = x_temp.flatten().unsqueeze(0);
  // y_temp  = y_temp.flatten().unsqueeze(0);

  // // concatenating dimensions
  // torch::Tensor floor_coordinates_2D  = torch::cat({x_temp, y_temp}, 0);

  // // setting up 3D coordinates
  // torch::Tensor floor_coordinates_3D  = torch::cat({floor_coordinates_2D,
  //                                                   torch::zeros({1, floor_coordinates_2D.size(1)})},
  //                                                   0);

  // // setting up scatter strengths
  // torch::Tensor floor_scatterers_reflectivity = torch::normal(10.0,
  //                                                             1.0,
  //                                                             {1, floor_coordinates_3D.size(1)});

  // // // instantiating an unordered map
  // // std::unordered_map<std::string, torch::Tensor> floor_scatterers;
  // // floor_scatterers["coordinates"] = floor_coordinates_3D;
  // // floor_scatterers["reflectivity"] = floor_scatterers_reflectivity;

  // // printing
  // std::cout<<"=================================================================="<<std::endl;
  // std::cout<<"floor_coordinates_3D \n"<<floor_coordinates_3D<<std::endl;
  // std::cout<<"floor_scatterers_reflectivity \n"<<floor_scatterers_reflectivity<<std::endl;


  // // saving directly
  // torch::save(floor_coordinates_3D, 
  //             "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/floor_coordinates_3D.pt");
  // torch::save(floor_scatterers_reflectivity, 
  //             "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/floor_scatterers_reflectivity.pt");
  // std::cout<<"Saving torch::tensors"<<std::endl;

  // // loading directly
  // torch::Tensor floor_coordinates_3D_loaded;
  // torch::Tensor floor_scatterers_reflectivity_loaded;

  // torch::load(floor_coordinates_3D_loaded,
  //             "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/floor_coordinates_3D.pt");
  // torch::load(floor_scatterers_reflectivity_loaded, 
  //             "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/floor_scatterers_reflectivity.pt");
  // std::cout<<"Loaded torch::tensors"<<std::endl;

  // returning
  return 0;
}