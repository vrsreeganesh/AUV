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
#include <thread>
#include "math.h"

// including custom headers: for classes 
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/include/ULAClass.h"
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/include/TransmitterClass.h"
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/ULASetup/ULASetup.cpp"
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/TransmitterSetup/TransmitterSetup.cpp"
// #include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/SeafloorSetup/SeafloorSetup.cpp"
// #include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Functions/fAnglesToTensor.cpp"
// #include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Functions/fCalculateCosine.cpp"
// #include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Functions/fColumnNormalize.cpp"
// // #include ""



// hash defines
#define PRINTSPACE      std::cout<<"\n\n\n\n\n\n\n\n"<<std::endl;
#define PRINTSMALLLINE  std::cout<<"------------------------------------------------"<<std::endl;
#define PRINTLINE       std::cout<<"================================================"<<std::endl;
#define PI 3.14159265

// Main = Main = Main = Main = Main = Main = Main = Main = Main
// Main = Main = Main = Main = Main = Main = Main = Main = Main
// Main = Main = Main = Main = Main = Main = Main = Main = Main
// Main = Main = Main = Main = Main = Main = Main = Main = Main
// Main = Main = Main = Main = Main = Main = Main = Main = Main
int main() {
    PRINTLINE

    // Building Sea-floor
    ULAClass ula;
    std::thread ulaThread_t(ULASetup, &ula);
    
    // Building Transmitter
    TransmitterClass transmitter; 
    std::thread transmitterThread_t(TransmitterSetup, &transmitter);






    // Joining threads
    ulaThread_t.join();
    transmitterThread_t.join();

    // printing status
    std::cout<<"ula = \n"<<ula<<std::endl;
    std::cout<<"transmitter = \n"<<transmitter<<std::endl;

    PRINTLINE

  // // encapsulating coordinates and reflectivity in a dictionary
  // std::unordered_map<std::string, torch::Tensor> floor_scatterers;
  // torch::load(floor_scatterers["coordinates"],
  //             "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/floor_coordinates_3D.pt");
  // torch::load(floor_scatterers["reflectivity"], 
  //             "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/floor_scatterers_reflectivity.pt");
  
  // // sending to GPU
  // floor_scatterers["coordinates"]   = floor_scatterers["coordinates"].to(   torch::kMPS);
  // floor_scatterers["reflectivity"]  = floor_scatterers["reflectivity"].to(  torch::kMPS);




  // // AUV Setup
  // torch::Tensor auv_initial_location            = torch::tensor({0.0, 2.0, 2.0}).view({3,1}).to(torch::kMPS);  // initial location
  // torch::Tensor auv_initial_velocity            = torch::tensor({1.0, 0.0, 0.0}).view({3,1}).to(torch::kMPS);  // initial velocity
  // torch::Tensor auv_initial_acceleration        = torch::tensor({0.0, 0.0, 0.0}).view({3,1}).to(torch::kMPS);  // initial acceleration
  // torch::Tensor auv_initial_pointing_direction  = torch::tensor({1.0, 0.0, 0.0}).view({3,1}).to(torch::kMPS);  // initial pointing direction

  // // Initializing a member of class, AUV
  // AUV auv(auv_initial_location,               // assigning initial location
  //         auv_initial_velocity,               // assigning initial velocity
  //         auv_initial_acceleration,           // assigning initial acceleration
  //         auv_initial_pointing_direction);    // assigning initial pointing direction



  // // Setting up ULAs for the AUV: front, portside and starboard
  // const int num_sensors             = 32;                   // number of sensors
  // const double intersensor_distance = 1e-4;                 // distance between sensors
  
  // ULA ula_portside(num_sensors,   intersensor_distance);    // ULA onbject for portside
  // ULA ula_fbls(num_sensors,       intersensor_distance);    // ULA object for front-side
  // ULA ula_starboard(num_sensors,  intersensor_distance);    // ULA object for starboard
  
  // auv.ula_portside                  = ula_portside;         // attaching portside-ULAs to the AUV
  // auv.ula_fbls                      = ula_fbls;             // attaching front-ULA to the AUV
  // auv.ula_starboard                 = ula_starboard;        // attaching starboard-ULA to the AUV



  // // Setting up Projector: front, portside and starboard
  // Projector projector_portside(torch::zeros({3,1}).to(torch::kMPS),   // location
  //                              fDeg2Rad(90),                          // azimuthal angle
  //                              fDeg2Rad(-30),                         // elevation angle
  //                              fDeg2Rad(30),                          // azimuthal beamwidth
  //                              fDeg2Rad(20));                         // elevation beamwidth
  // Projector projector_fbls(torch::zeros({3,1}).to(torch::kMPS),       // location
  //                          fDeg2Rad(0),                               // azimuthal angle
  //                          fDeg2Rad(-30),                             // elevation angle
  //                          fDeg2Rad(120),                             // azimuthal beamwidth
  //                          fDeg2Rad(60));                             // elevation beamwidth;
  // Projector projector_starboard(torch::zeros({3,1}).to(torch::kMPS),  // location
  //                               fDeg2Rad(-90),                        // azimuthal angle
  //                               fDeg2Rad(-30),                        // elevation angle
  //                               fDeg2Rad(30),                         // azimuthal beamwidth
  //                               fDeg2Rad(20));                        // elevation beamwidth;

  // auv.projector_portside  = projector_portside;             // Attaching projectors to AUV
  // auv.projector_fbls      = projector_fbls;                 // Attaching projectors to AUV
  // auv.projector_starboard = projector_starboard;            // Attaching projectors to AUV


  


  // // testing projection
  // torch::Tensor coordinates = torch::tensor({ 1,  2,  3,  4,
  //                                             0,  0,  0,  0,
  //                                            -1, -1, -1, -1}).view({3,4}).to(torch::kFloat).to(torch::kMPS);
  // torch::Tensor coordinates_normalized = fColumnNormalize(coordinates);
  // torch::Tensor coordinates_projected = coordinates.clone();
  // coordinates_projected[2]  = torch::zeros({coordinates.size(1)});

  // torch::Tensor innerproduct = coordinates * coordinates_projected;
  // innerproduct = torch::sum(innerproduct, 0, true);



  // PRINTLINE
  // torch::Tensor xy = coordinates.clone();
  // xy[2] = torch::zeros({xy.size(1)});
  // std::cout<<"coordinates = \n"<<coordinates<<std::endl;
  // PRINTSMALLLINE
  // std::cout<<"xy = \n"<<xy<<std::endl;
  // torch::Tensor xylengths = torch::norm(xy, 2, 0, true, torch::kFloat);
  // std::cout<<"xylengths = \n"<<xylengths<<std::endl;
  // PRINTLINE



  


  // returning
  return 0;
}