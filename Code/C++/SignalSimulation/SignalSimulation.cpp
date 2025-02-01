/*==============================================================================
Aim: Signal Simulation
--------------------------------------------------------------------------------
==============================================================================*/ 

// including standard 
#include <cstdint>
#include <ostream>
#include <torch/torch.h>
#include <iostream>
#include <thread>
#include "math.h"
#include <chrono>
#include <Python.h>
#include <cstdlib>

// hash defines
#ifndef PRINTSPACE
#define PRINTSPACE      std::cout<<"\n\n\n";
#endif
#ifndef PRINTSMALLLINE
#define PRINTSMALLLINE  std::cout<<"--------------------------------------------------------------------"<<std::endl;
#endif
#ifndef PRINTDOTS
#define PRINTDOTS       std::cout<<"...................................................................."<<std::endl;
#endif
#ifndef PRINTLINE
#define PRINTLINE       std::cout<<"===================================================================="<<std::endl;
#endif
#ifndef PI
#define PI              3.14159265
#endif

// debugging hashdefine
#ifndef DEBUGMODE
#define DEBUGMODE       false
#endif

// deciding to save tensors or not
#ifndef SAVETENSORS
    #define SAVETENSORS       true
    // #define SAVETENSORS       false
#endif

// choose device here
#ifndef DEVICE
    #define DEVICE          torch::kCPU
    // #define DEVICE          torch::kMPS
    // #define DEVICE          torch::kCUDA
#endif

// class definitions
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/include/ScattererClass.h"
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/include/ULAClass.h"
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/include/TransmitterClass.h"
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/include/AUVClass.h"

// setup-scripts
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/ULASetup/ULASetup.cpp"
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/TransmitterSetup/TransmitterSetup.cpp"
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/SeafloorSetup/SeafloorSetup.cpp"
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/AUVSetup/AUVSetup.cpp"

// functions
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Functions/fPrintTensorSize.cpp"
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Functions/fSph2Cart.cpp"
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Functions/fCart2Sph.cpp"
// #include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Functions/fGetCurrentTimeFormatted.cpp"
// #include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Functions/fAnglesToTensor.cpp"
// #include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Functions/fCalculateCosine.cpp"
// #include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Functions/fColumnNormalize.cpp"
// // #include ""

// function to plot the thing
// void fPlotTensors(){
//     system("python /Users/vrsreeganesh/Documents/GitHub/AUV/Code/Python/TestingSaved_tensors.py");
// }

// main-function
int main() {

    // Builing Sea-floor
    ScattererClass SeafloorScatter;
    std::thread scatterThread_t(SeafloorSetup, \
                                &SeafloorScatter);

    // Building ULA
    ULAClass ula_fls, ula_port, ula_starboard;
    std::thread ulaThread_t(ULASetup, \
                            &ula_fls, \
                            &ula_port, \
                            &ula_starboard);
    
    // Building Transmitter
    TransmitterClass transmitter_fls, transmitter_port, transmitter_starboard; 
    std::thread transmitterThread_t(TransmitterSetup, 
                                    &transmitter_fls, 
                                    &transmitter_port, 
                                    &transmitter_starboard);

    // Joining threads
    ulaThread_t.join();         // making the ULA population thread join back
    transmitterThread_t.join(); // making the transmitter population thread join back
    scatterThread_t.join();     // making the scattetr population thread join back

    // building AUV 
    AUVClass auv;               // instantiating class object
    AUVSetup(&auv);        // populating 
    
    // attaching
    auv.ULA_fls                 = ula_fls;
    auv.ULA_port                = ula_port;
    auv.ULA_starboard           = ula_starboard;
    
    auv.transmitter_fls         = transmitter_fls;
    auv.transmitter_port        = transmitter_port;
    auv.transmitter_starboard   = transmitter_starboard;

    // storing 
    ScattererClass SeafloorScatter_deepcopy = SeafloorScatter;


    // mimicking movement
    int number_of_stophops = 1;
    for(int i = 0; i<number_of_stophops; ++i){

        // time measuring
        auto start_time = std::chrono::high_resolution_clock::now();
        
        // printing some spaces
        PRINTSPACE; PRINTLINE; std::cout<<"i = "<<i<<std::endl; PRINTLINE
        
        // making the deep copy
        ScattererClass SeafloorScatter      = SeafloorScatter_deepcopy; // copy for FLS
        
        // subsetting the scatterers
        auv.simulateSignal(SeafloorScatter);

        // measuring time 
        auto end_time   = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time_duration = end_time - start_time;
        PRINTDOTS; std::cout<<"Time taken (i = "<<i<<") = "<<time_duration.count()<<" seconds"<<std::endl; PRINTDOTS        


        // moving to next position
        auv.step(0.5);


        // Printing end-of-step
        PRINTSPACE

    }



























































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