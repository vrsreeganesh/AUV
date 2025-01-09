/*
================================================================================
Aim: Signal Simulation
--------------------------------------------------------------------------------
Note:
  > Load simulated scatterers
  > Setup AUV

To Do:
  > write function to update 
================================================================================
*/ 

// including standard 
#include <ostream>
#include <torch/torch.h>
#include <iostream>

// including custom headers: for classes 
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/include/AUV.h"
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/include/Projector.h"
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/include/ULA.h"

// hash defines
#define PRINTSMALLLINE  std::cout<<"------------------------------------------------"<<std::endl;
#define PRINTLINE       std::cout<<"================================================"<<std::endl;
#define PI 3.14159265


// Functions ===============================================
// Functions ===============================================
// Functions ===============================================
// Functions ===============================================
// Functions ===============================================
// Functions ===============================================

// function to column normalize a tensor
torch::Tensor fColumnNormalize(torch::Tensor inputTensor){
  // finding norm
  torch::Tensor inputTensorNorm = torch::linalg::norm(inputTensor,    //A
                                                      2,              // order
                                                      0,              // dimension
                                                      true,           // keep-dims
                                                      torch::kFloat); // d-type       

  // dividing the input by the norm
  torch::Tensor outputTensor = inputTensor/inputTensorNorm;

  // returning result tensor
  return outputTensor;
}


// function to calculate cosine of two tensors
torch::Tensor fCalculateCosine(torch::Tensor inputTensor1,
                               torch::Tensor inputTensor2)
{
  // column normalizing the the two signals
  inputTensor1 = fColumnNormalize(inputTensor1);
  inputTensor2 = fColumnNormalize(inputTensor2);

  // finding their dot product
  torch::Tensor dotProduct = inputTensor1 * inputTensor2;
  torch::Tensor cosineBetweenVectors = torch::sum(dotProduct, 
                                                  0,
                                                  true);

  // returning the value
  return cosineBetweenVectors;

}

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
    std::unordered_map<std::string, torch::Tensor> outputScatterers;
    outputScatterers["coordinates"]     = inputScatterers["coordinates"].clone();
    outputScatterers["reflectivity"]    = inputScatterers["reflectivity"].clone();

    // change origin
    outputScatterers["coordinates"] = inputScatterers["coordinates"].clone() - location;


    // finding their corresponding planar projections
    torch::Tensor scatterer_coords_xy_projected   = outputScatterers["coordinates"].clone();
    scatterer_coords_xy_projected[2]              = torch::zeros({scatterer_coords_xy_projected.size(1)});
    
    // finding the angle between vector and xy-projected vector
    torch::Tensor elevationCosine        = fCalculateCosine(outputScatterers["coordinates"], 
                                                            scatterer_coords_xy_projected);


    // finding the angle between xy-projected vector and xz-plane
    torch::Tensor scatterer_coords_xz_projected   = outputScatterers["coordinates"].clone();
    scatterer_coords_xz_projected[1]              = torch::zeros({scatterer_coords_xz_projected.size(1)});
    
    // finding the angle between xy-projected vector and xz projected vector
    torch::Tensor azimuthalCosine       = fCalculateCosine(outputScatterers["coordinates"], 
                                                           scatterer_coords_xz_projected);



    // returning
    return outputScatterers;
  };

};



// function for ULA ============================================================
// function for ULA ============================================================
// function for ULA ============================================================
// function for ULA ============================================================
// function for ULA ============================================================
class ULA{

private:
  torch::Tensor ula_sensor_locations;   // location of sensors in the ULA

public:
  // public members: public cause we want the AUV to edit this
  int num_sensors;                      // number of sensors
  double intersensor_distance;          // distance between sensors
  torch::Tensor first_sensor_location;  // location of first sensor
  torch::Tensor ula_sensor_direction;   // vector from first sensor to rest
  


  // constructor
  ULA(int num_sensors                     = 32,
      double intersensor_distance         = 1e-3,
      torch::Tensor first_sensor_location = torch::zeros({3,1}),
      torch::Tensor ula_sensor_direction  = torch::zeros({3,1}))
      : num_sensors(num_sensors),
        intersensor_distance(intersensor_distance),
        first_sensor_location(first_sensor_location),
        ula_sensor_direction(ula_sensor_direction) {}



  // calculating sensor location
  void fCalculateSensorLocations(){
    // length-normalizing signal
    torch::Tensor length  = torch::norm(ula_sensor_direction, 2, 0, true);  // calculating length
    ula_sensor_direction  = ula_sensor_direction/length;                    // length normalizing
    ula_sensor_direction  = intersensor_distance * ula_sensor_direction;    // multiplying with intersensor distance

    // building the counting array
    torch::Tensor number_array = torch::arange(0, num_sensors, 1).view({1, num_sensors});

    // building sensor locations
    ula_sensor_locations  = ula_sensor_direction * number_array;
  }


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
  friend std::ostream& operator<<(std::ostream& os, ULA& ula){
    os<<"\t> num sensors = "<<ula.num_sensors<<std::endl;
    os<<"\t> intersensor distance = "<<ula.intersensor_distance<<std::endl;
    ula.fCalculateSensorLocations();  
    os<<"\t> ULA sensor locations = "<<ula.ula_sensor_locations.view({1, ula.num_sensors})<<std::endl;
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
class AUV{
public:
  // independent attributes
  torch::Tensor location;               // current location of AUV
  torch::Tensor velocity;               // velocity of AUV
  torch::Tensor acceleration;           // acceleration of AUV
  torch::Tensor pointing_direction;     // direction in which AUV is pointed

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

    // printing the AUV parameters
    PRINTLINE
    os<<"AUV Parameters"<<std::endl;
    os<<"> location = "<<auv.location.view({1,3})<<std::endl;                     
    os<<"> velocity = "<<auv.velocity.view({1,3})<<std::endl;                     
    os<<"> acceleration = "<<auv.acceleration.view({1,3})<<std::endl;             
    os<<"> pointing_direction = "<<auv.pointing_direction.view({1,3})<<std::endl; 



    // printing the projector parameters
    PRINTLINE
    os<<"Projector Parameters"<<std::endl;
    os<<"\t Projector: Portside"<<std::endl;
    os<<"\t\t > location = "<<auv.projector_portside.location.view({1,3})<<std::endl;
    os<<"\t\t > azimuthal_angle = "<<auv.projector_portside.azimuthal_angle<<std::endl;
    os<<"\t\t > elevation_angle = "<<auv.projector_portside.elevation_angle<<std::endl;
    os<<"\t\t > azimuthal_beamwidth = "<<auv.projector_portside.azimuthal_beamwidth<<std::endl;
    os<<"\t\t > vertical_beamwidth = "<<auv.projector_portside.vertical_beamwidth<<std::endl;
    PRINTSMALLLINE
    os<<"\t Projector: fbls"<<std::endl;
    os<<"\t\t > location = "<<auv.projector_fbls.location.view({1,3})<<std::endl;       
    os<<"\t\t > azimuthal_angle = "<<auv.projector_fbls.azimuthal_angle<<std::endl;
    os<<"\t\t > elevation_angle = "<<auv.projector_fbls.elevation_angle<<std::endl;
    os<<"\t\t > azimuthal_beamwidth = "<<auv.projector_fbls.azimuthal_beamwidth<<std::endl;
    os<<"\t\t > vertical_beamwidth = "<<auv.projector_fbls.vertical_beamwidth<<std::endl;
    PRINTSMALLLINE
    os<<"\t Projector: Portside"<<std::endl;
    os<<"\t\t > location = "<<auv.projector_starboard.location.view({1,3})<<std::endl;
    os<<"\t\t > azimuthal_angle = "<<auv.projector_starboard.azimuthal_angle<<std::endl;
    os<<"\t\t > elevation_angle = "<<auv.projector_starboard.elevation_angle<<std::endl;
    os<<"\t\t > azimuthal_beamwidth = "<<auv.projector_starboard.azimuthal_beamwidth<<std::endl;
    os<<"\t\t > vertical_beamwidth = "<<auv.projector_starboard.vertical_beamwidth<<std::endl;
    


    // printing the ULA parameters
    PRINTLINE
    os<<"ULA Parameters"<<std::endl;
    os<<"\t ULA: portside"<<std::endl;
    os<<"\t\t > num_sensors = "<<auv.ula_portside.num_sensors<<std::endl;
    os<<"\t\t > intersensor_distance = "<<auv.ula_portside.intersensor_distance<<std::endl;
    os<<"\t\t > location = "<<auv.ula_portside.first_sensor_location.view({1,3})<<std::endl;
    os<<"\t\t > ula_sensor_direction = "<<auv.ula_portside.ula_sensor_direction.view({1,3})<<std::endl;
    // os<<"\t\t\t > sensor locations = "<<auv.ula_portside.ula_sensor_locations.view({1,3})<<std::endl;
    PRINTSMALLLINE
    os<<"\t ULA: fbls"<<std::endl;
    os<<"\t\t > num_sensors = "<<auv.ula_fbls.num_sensors<<std::endl;
    os<<"\t\t > intersensor_distance = "<<auv.ula_fbls.intersensor_distance<<std::endl;
    os<<"\t\t > location = "<<auv.ula_fbls.first_sensor_location.view({1,3})<<std::endl;
    os<<"\t\t > ula_sensor_direction"<<auv.ula_fbls.ula_sensor_direction.view({1,3})<<std::endl;
    // os<<"\t\t\t > sensor locations = "<<auv.ula_fbls.ula_sensor_locations.view({1,3})<<std::endl;
    PRINTSMALLLINE
    os<<"\t ULA: starboard"<<std::endl;
    os<<"\t\t > num_sensors = "<<auv.ula_starboard.num_sensors<<std::endl;
    os<<"\t\t > intersensor_distance = "<<auv.ula_starboard.intersensor_distance<<std::endl;
    os<<"\t\t > location = "<<auv.ula_starboard.first_sensor_location.view({1,3})<<std::endl;
    os<<"\t\t > ula_sensor_direction"<<auv.ula_starboard.ula_sensor_direction.view({1,3})<<std::endl;
    // os<<"\t\t\t > sensor locations = "<<auv.ula_starboard.ula_sensor_locations.view({1,3})<<std::endl;

    return os;
  }

  // synchronizing 
  void _fAttach(){
    // attach ULA:portside
    ula_portside.first_sensor_location  = location;                 // assigning ULA location to ULA:portside
    ula_portside.ula_sensor_direction   = -1 * pointing_direction;  // assigning opposite direction as that of AUV
    ula_portside.fCalculateSensorLocations();                       // updating sensor-locations

    // attach ULA:fbls
    ula_fbls.first_sensor_location      = location;                 // assigning ULA location to ULA:fbls
    ula_fbls.ula_sensor_direction       = -1 * pointing_direction;  // assigning opposite direction as that of AUV
    ula_fbls.fCalculateSensorLocations();                           // updating sensor-locations

    // attach ULA:starboard
    ula_starboard.first_sensor_location = location;                 // assigning ULA location to ULA:starboard
    ula_starboard.ula_sensor_direction  = -1 * pointing_direction;  // assigning opposite direction as that of AUV
    ula_starboard.fCalculateSensorLocations();                      // updating sensor-locations

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
float fDeg2Rad(float deg){
  float deg_value = static_cast<float>(PI * deg / 180);
  return deg_value;
}






// MAIN ====================================================
// MAIN ====================================================
// MAIN ====================================================
// MAIN ====================================================
// MAIN ====================================================
// MAIN ====================================================
// MAIN ====================================================
// MAIN ====================================================
// MAIN ====================================================
// MAIN ====================================================
// MAIN ====================================================
// MAIN ====================================================
// MAIN ====================================================
// MAIN ====================================================
// MAIN ====================================================
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
  const int num_sensors             = 32;                   // number of sensors
  const double intersensor_distance = 1e-4;                 // distance between sensors
  ULA ula_portside(num_sensors, intersensor_distance);      // ULA onbject for portside
  ULA ula_fbls(num_sensors, intersensor_distance);          // ULA object for front-side
  ULA ula_starboard(num_sensors, intersensor_distance);     // ULA object for starboard
  auv.ula_portside  = ula_portside;                         // attaching portside-ULAs to the AUV
  auv.ula_fbls      = ula_fbls;                             // attaching front-ULA to the AUV
  auv.ula_starboard = ula_starboard;                        // attaching starboard-ULA to the AUV



  // Setting up Projector: front, portside and starboard
  Projector projector_portside(torch::zeros({3,1}),         // location
                               fDeg2Rad(90),                // azimuthal angle
                               fDeg2Rad(-30),               // elevation angle
                               fDeg2Rad(30),                // azimuthal beamwidth
                               fDeg2Rad(20));               // elevation beamwidth
  Projector projector_fbls(torch::zeros({3,1}),             // location
                           fDeg2Rad(0),                     // azimuthal angle
                           fDeg2Rad(-30),                   // elevation angle
                           fDeg2Rad(120),                   // azimuthal beamwidth
                           fDeg2Rad(60));                   // elevation beamwidth;
  Projector projector_starboard(torch::zeros({3,1}),        // location
                                fDeg2Rad(-90),              // azimuthal angle
                                fDeg2Rad(-30),              // elevation angle
                                fDeg2Rad(30),               // azimuthal beamwidth
                                fDeg2Rad(20));              // elevation beamwidth;

  // Attaching projectors to AUV
  auv.projector_portside  = projector_portside;
  auv.projector_fbls      = projector_fbls;
  auv.projector_starboard = projector_starboard;


  // // testing something
  // std::cout<<"num_sensors = "<<num_sensors<<std::endl;
  // torch::Tensor number_array = torch::arange(0, num_sensors, 1).view({1, num_sensors}).view({4, 8}).to(torch::kFloat);
  // torch::Tensor bruh = number_array.clone();
  // bruh[0] = torch::zeros({bruh.size(1)});
  
  // std::cout<<"number_array = \n"<<number_array<<std::endl;
  // PRINTLINE
  // // std::cout<<"bruh = \n"<<bruh[0]<<std::endl;
  // std::cout<<"bruh = \n"<<bruh<<std::endl;

  // testing projection
  
  torch::Tensor coordinates = torch::tensor({ 1,  2,  3,  4,
                                              0,  0,  0,  0,
                                             -1, -1, -1, -1}).view({3,4}).to(torch::kFloat);
  torch::Tensor coordinates_normalized = fColumnNormalize(coordinates);
  torch::Tensor coordinates_projected = coordinates.clone();
  coordinates_projected[2]  = torch::zeros({coordinates.size(1)});

  torch::Tensor innerproduct = coordinates * coordinates_projected;
  innerproduct = torch::sum(innerproduct, 0, true);


  PRINTLINE
  // std::cout<<"coordinates = \n"<<coordinates<<std::endl;                                            PRINTSMALLLINE
  // std::cout<<"coordinates_normalized = \n"<<coordinates_normalized<<std::endl;                      PRINTSMALLLINE
  // torch::Tensor coordinates_abs = torch::norm(coordinates_normalized, 2, 0, true, torch::kFloat);   PRINTSMALLLINE
  // std::cout<<"coordinates_abs = \n"<<coordinates_abs<<std::endl;                                    PRINTSMALLLINE

  torch::Tensor bruh = fCalculateCosine(coordinates, coordinates_projected);
  std::cout<<"bruh = \n"<<bruh<<std::endl;


  // std::cout<<"coordinates_projected = \n"<<coordinates_projected<<std::endl;
  // std::cout<<"innerproduct = \n"<<innerproduct<<std::endl;
  PRINTSMALLLINE





  
  // // checking all the properties
  // PRINTLINE
  // torch::Tensor bruh = torch::tensor({2, 2, 0, 0, 0, 0}).view({3,2}).to(torch::kFloat);
  // torch::Tensor column_norms = torch::norm(bruh, 2, 0, true);
  // torch::Tensor normalized_bruh = bruh/column_norms;

  // PRINTSMALLLINE
  // std::cout<<"bruh.shape = "<<std::endl;
  // print_tensor_size(bruh);

  // std::cout<<"column_norms.shape = "<<std::endl;
  // print_tensor_size(column_norms);
  
  // std::cout<<"normalized_bruh.shape = "<<std::endl;
  // print_tensor_size(normalized_bruh);

  // PRINTSMALLLINE
  // std::cout<<"bruh = \n"<<bruh<<std::endl;
  // std::cout<<"column_norms = \n"<<column_norms<<std::endl;
  // std::cout<<"normalized_bruh = \n"<<normalized_bruh<<std::endl;
  
  


  // returning
  return 0;
}