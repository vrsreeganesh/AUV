/*==============================================================================
Aim: Signal Simulation
--------------------------------------------------------------------------------
==============================================================================*/ 

// including
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/include/packages.h"
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/include/config.h"       // hash-defines
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/include/classes.h"      // class defs
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/include/setupscripts.h" // setup-scripts
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/include/functions.h"    // functions


// main-function
int main() {

    // Ensuring no-gradients are built
    NoGradGuard no_grad;

    // Builing Sea-floor
    ScattererClass SeafloorScatter;
    thread scatterThread_t(SeafloorSetup, \
                           ref(SeafloorScatter));

    // Building ULA
    ULAClass ula_fls, ula_port, ula_starboard;
    thread ulaThread_t(ULASetup, \
                       ref(ula_fls), \
                       ref(ula_port), \
                       ref(ula_starboard));
    
    // Building Transmitter
    TransmitterClass transmitter_fls, transmitter_port, transmitter_starboard; 
    thread transmitterThread_t(TransmitterSetup, 
                               ref(transmitter_fls), 
                               ref(transmitter_port), 
                               ref(transmitter_starboard));

    // recombining threads
    scatterThread_t.join();     // making the scattetr population thread join back
    ulaThread_t.join();         // making the ULA population thread join back
    transmitterThread_t.join(); // making the transmitter population thread join back

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

    // mimicking movement
    int number_of_stophops = 4;
    // if (true) return 0;
    for(int i = 0; i<number_of_stophops; ++i){

        // time measuring
        auto start_time = high_resolution_clock::now();
        
        // printing some spaces
        PRINTSPACE; PRINTSPACE; PRINTLINE; cout<<"i = "<<i<<endl; PRINTLINE
        
        // making the deep copy
        ScattererClass SeafloorScatter = SeafloorScatter_deepcopy;

        // signal simulation
        auv.simulateSignal(SeafloorScatter);

        // saving simulated signal
        if (SAVETENSORS) {

            // saving the signal matrix tensors
            save(auv.ULA_fls.signalMatrix, \
                        "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/signalMatrix_fls.pt");
            save(auv.ULA_port.signalMatrix, \
                        "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/signalMatrix_port.pt");
            save(auv.ULA_starboard.signalMatrix, \
                        "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/signalMatrix_starboard.pt");

            // running python script
            string script_to_run = \
                "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/Python/Plot_SignalMatrix.py";
            thread plotSignalMatrix_t(fRunSystemScriptInSeperateThread, \
                                           script_to_run);
            plotSignalMatrix_t.detach();
        
        }


        if (IMAGING_TOGGLE) {

            // creating image from signals
            auv.image();

            // saving the tensors
            if(SAVETENSORS){
                // saving the beamformed images
                save(auv.ULA_fls.beamformedImage, \
                            "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/ULA_fls_image.pt");
                // save(auv.ULA_port.beamformedImage, \
                //             "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/ULA_port_image.pt");
                // save(auv.ULA_starboard.beamformedImage, \
                //             "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/ULA_starboard_image.pt");

                // saving cartesian image
                save(auv.ULA_fls.cartesianImage, \
                            "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/ULA_fls_cartesianImage.pt");

                // // running python file
                // system("python /Users/vrsreeganesh/Documents/GitHub/AUV/Code/Python/Plot_BeamformedImage.py");
                system("python /Users/vrsreeganesh/Documents/GitHub/AUV/Code/Python/Plot_cartesianImage.py");
            }
        }
        


        // measuring and printing time taken
        auto end_time   = high_resolution_clock::now();
        duration<double> time_duration = end_time - start_time;
        PRINTDOTS; cout<<"Time taken (i = "<<i<<") = "<<time_duration.count()<<" seconds"<<endl; PRINTDOTS        

        // moving to next position
        auv.step(0.5);

    }







  // returning
  return 0;
}