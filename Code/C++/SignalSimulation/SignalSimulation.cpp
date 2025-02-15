/*==============================================================================
Aim: Signal Simulation
--------------------------------------------------------------------------------
==============================================================================*/ 

// including standard 
#include <ostream>
#include <torch/torch.h>
#include <iostream>
#include <thread>
#include "math.h"
#include <chrono>
#include <Python.h>
#include <Eigen/Dense>
#include <cstdlib>          // For terminal access
#include <omp.h>            // the openMP

// hash defines
#ifndef PRINTSPACE
    #define PRINTSPACE      std::cout<<"\n\n\n";
#endif
#ifndef PRINTSMALLLINE
    #define PRINTSMALLLINE  std::cout<<"------------------------------------------------------------------------------------"<<std::endl;
#endif
#ifndef PRINTDOTS
    #define PRINTDOTS       std::cout<<"...................................................................................."<<std::endl;
#endif
#ifndef PRINTLINE
    #define PRINTLINE       std::cout<<"===================================================================================="<<std::endl;
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

// Enable Imaging
#define IMAGING_TOGGLE                  true


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
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Functions/fConvolveColumns.cpp"
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Functions/fRunSystemScriptInSeperateThread.cpp"


// main-function
int main() {

    // Ensuring no-gradients are calculated in this scope
    torch::NoGradGuard no_grad;

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
    AUVClass auv;                   // instantiating class object
    AUVSetup(&auv);             // populating 
    
    // attaching components to the AUV
    auv.ULA_fls                 = ula_fls;                  // attaching ULA-FLS to AUV
    auv.ULA_port                = ula_port;                 // attaching ULA-Port to AUV
    auv.ULA_starboard           = ula_starboard;            // attaching ULA-Starboard to AUV
    auv.transmitter_fls         = transmitter_fls;          // attaching Transmitter-FLS to AUV
    auv.transmitter_port        = transmitter_port;         // attaching Transmitter-Port to AUV
    auv.transmitter_starboard   = transmitter_starboard;    // attaching Transmitter-Starboard to AUV

    // storing 
    ScattererClass SeafloorScatter_deepcopy = SeafloorScatter;

    // pre-computing the data-structures required for processing
    auv.init();

    // printing sampling frequency and bandwidth
    std::cout << "main:: auv.transmitter_fls.bandwidth = " << auv.transmitter_fls.bandwidth<< std::endl;
    std::cout << "main:: auv.ULA_fls.sampling_frequency = " << auv.ULA_fls.sampling_frequency << std::endl;

    // mimicking movement
    int number_of_stophops = 1;
    // if (true) return 0;
    for(int i = 0; i<number_of_stophops; ++i){

        // time measuring
        auto start_time = std::chrono::high_resolution_clock::now();
        
        // printing some spaces
        PRINTSPACE; PRINTSPACE; PRINTLINE; std::cout<<"i = "<<i<<std::endl; PRINTLINE
        
        // making the deep copy
        ScattererClass SeafloorScatter = SeafloorScatter_deepcopy;
        
        // signal simulation
        auv.simulateSignal(SeafloorScatter);



        // saving simulated signal
        if (SAVETENSORS) {

            // saving the signal matrix tensors
            torch::save(auv.ULA_fls.signalMatrix, \
                        "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/signalMatrix_fls.pt");
            torch::save(auv.ULA_port.signalMatrix, \
                        "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/signalMatrix_port.pt");
            torch::save(auv.ULA_starboard.signalMatrix, \
                        "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/signalMatrix_starboard.pt");

            // running python script
            std::string script_to_run = "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/Python/Plot_SignalMatrix.py";
            std::thread plotSignalMatrix_t(fRunSystemScriptInSeperateThread, \
                                           script_to_run);
            plotSignalMatrix_t.detach();
        
        }


        if (IMAGING_TOGGLE) {
            // creating image from signals
            auv.image();

            // saving the tensors
            if(SAVETENSORS){
                // saving the beamformed images
                torch::save(auv.ULA_fls.beamformedImage, \
                            "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/ULA_fls_image.pt");
                // torch::save(auv.ULA_port.beamformedImage, \
                //             "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/ULA_port_image.pt");
                // torch::save(auv.ULA_starboard.beamformedImage, \
                //             "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/ULA_starboard_image.pt");

                // saving cartesian image
                torch::save(auv.ULA_fls.cartesianImage, \
                            "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/ULA_fls_cartesianImage.pt");

                // // running python file
                // system("python /Users/vrsreeganesh/Documents/GitHub/AUV/Code/Python/Plot_BeamformedImage.py");
                system("python /Users/vrsreeganesh/Documents/GitHub/AUV/Code/Python/Plot_cartesianImage.py");
            }
        }
        


        // measuring and printing time taken
        auto end_time   = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> time_duration = end_time - start_time;
        PRINTDOTS; std::cout<<"Time taken (i = "<<i<<") = "<<time_duration.count()<<" seconds"<<std::endl; PRINTDOTS        

        // moving to next position
        auv.step(0.5);

    }







  // returning
  return 0;
}