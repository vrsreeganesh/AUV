

// #pragma once

// // function to plot the thing
// void fPlotTensors(){
//     system("python /Users/vrsreeganesh/Documents/GitHub/AUV/Code/Python/TestingSaved_tensors.py");
// }


// void fSaveSeafloorScatteres(ScattererClass scatterer, \
//                             ScattererClass scatterer_fls, \
//                             ScattererClass scatterer_port, \
//                             ScattererClass scatterer_starboard){

//     // saving the ground-truth
//     ScattererClass SeafloorScatter_gt = scatterer;
//     save(SeafloorScatter_gt.coordinates, "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_gt.pt");
//     save(SeafloorScatter_gt.reflectivity, "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_gt_reflectivity.pt");
    
//     // saving coordinates 
//     save(scatterer_fls.coordinates, "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_fls_coordinates.pt");
//     save(scatterer_port.coordinates, "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_port_coordinates.pt");
//     save(scatterer_starboard.coordinates, "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_starboard.coordinates.pt");

//     // saving reflectivities
//     save(scatterer_fls.reflectivity, "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_fls_coordinates_reflectivity.pt");
//     save(scatterer_port.reflectivity, "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_port_coordinates_reflectivity.pt");
//     save(scatterer_starboard.reflectivity, "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_starboard.coordinates_reflectivity.pt");

//     // plotting tensors
//     fPlotTensors();

//     // // saving the tensors
//     // if (true) {

//     //     // getting time ID
//     //     auto timeID = fGetCurrentTimeFormatted();
        
//     //     cout<<"\t\t\t\t\t\t\t Saving Tensors (timeID: "<<timeID<<")"<<endl;

//     //     // saving the ground-truth
//     //     ScattererClass SeafloorScatter_gt = scatterer;
//     //     save(SeafloorScatter_gt.coordinates, \
//     //                 "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_gt.pt");
//     //     save(SeafloorScatter_gt.reflectivity, \
//     //                 "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_gt_reflectivity.pt");
        

//     //     // saving coordinates 
//     //     save(scatterer_fls.coordinates, \
//     //             "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_fls_coordinates.pt");
//     //     save(scatterer_port.coordinates, \
//     //                 "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_port_coordinates.pt");
//     //     save(scatterer_starboard.coordinates, \
//     //                 "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_starboard.coordinates.pt");

//     //     // saving reflectivities
//     //     save(scatterer_fls.reflectivity, \
//     //             "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_fls_coordinates_reflectivity.pt");
//     //     save(scatterer_port.reflectivity, \
//     //                 "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_port_coordinates_reflectivity.pt");
//     //     save(scatterer_starboard.reflectivity, \
//     //                 "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_starboard.coordinates_reflectivity.pt");

//     //     // plotting tensors
//     //     fPlotTensors();

//     //     // indicating end of thread 
//     //     cout<<"\t\t\t\t\t\t\t Ended (timeID: "<<timeID<<")"<<endl;
//     // }
// }


// // hash-defines
// #define PI                                  3.14159265
// #define DEBUGMODE_AUV                       false
// #define SAVE_SIGNAL_MATRIX                  true
// #define SAVE_DECIMATED_SIGNAL_MATRIX        true
// #define SAVE_MATCHFILTERED_SIGNAL_MATRIX    true

// class AUVClass{
// public:
//     // Intrinsic attributes
//     Tensor location;                     // location of vessel
//     Tensor velocity;                     // current speed of the vessel [a vector]
//     Tensor acceleration;                 // current acceleration of vessel [a vector]
//     Tensor pointing_direction;           // direction to which the AUV is pointed
    
//     // uniform linear-arrays
//     ULAClass ULA_fls;                           // front-looking SONAR ULA
//     ULAClass ULA_port;                          // mounted ULA [object of class, ULAClass]        
//     ULAClass ULA_starboard;                     // mounted ULA [object of class, ULAClass]        
    
//     // transmitters
//     TransmitterClass transmitter_fls;           // transmitter for front-looking SONAR
//     TransmitterClass transmitter_port;          // mounted transmitter [obj of class, TransmitterClass]
//     TransmitterClass transmitter_starboard;     // mounted transmitter [obj of class, TransmitterClass]

//     // derived or dependent attributes
//     Tensor signalMatrix_1;               // matrix containing the signals obtained from ULA_1
//     Tensor largeSignalMatrix_1;          // matrix holding signal of synthetic aperture
//     Tensor beamformedLargeSignalMatrix;  // each column is the beamformed signal at each stop-hop

//     // plotting mode
//     bool plottingmode;    // to suppress plotting associated with classes

//     // spotlight mode related
//     Tensor absolute_coords_patch_cart; // cartesian coordinates of patch

//     // Synthetic Aperture Related 
//     Tensor ApertureSensorLocations;    // sensor locations of aperture








//     /* =========================================================================
//     Aim: Init
//     --------------------------------------------------------------------------*/ 
//     void init(){
        
//         // call sync-component attributes
//         this->syncComponentAttributes();
//         if (DEBUGMODE_AUV) cout << "AUVCLass::init: line 128" << endl;

//         // initializing all the ULAs 
//         this->ULA_fls.init(         &this->transmitter_fls); 
//         this->ULA_port.init(        &this->transmitter_port); 
//         this->ULA_starboard.init(   &this->transmitter_starboard);
//         if (DEBUGMODE_AUV) cout << "AUVCLass::init: line 134" << endl;
        

//         // precomputing delay-matrices for the ULA-class
//         thread ULA_fls_precompute_weights_t(&ULAClass::nfdc_precomputeDelayMatrices,       \
//                                                  &this->ULA_fls,                                \
//                                                  &this->transmitter_fls);
//         thread ULA_port_precompute_weights_t(&ULAClass::nfdc_precomputeDelayMatrices,      \
//                                                   &this->ULA_port,                              \
//                                                   &this->transmitter_port);
//         thread ULA_starboard_precompute_weights_t(&ULAClass::nfdc_precomputeDelayMatrices, \
//                                                        &this->ULA_starboard,                    \
//                                                        &this->transmitter_starboard);
//         if (DEBUGMODE_AUV) cout << "AUVCLass::init: line 145" << endl;

//         // joining the threads back
//         ULA_fls_precompute_weights_t.join();
//         ULA_port_precompute_weights_t.join();
//         ULA_starboard_precompute_weights_t.join();

//     }


    
//     /*==========================================================================
//     Aim: stepping motion
//     --------------------------------------------------------------------------*/ 
//     void step(float timestep){
        
//         // updating location
//         this->location = this->location + this->velocity * timestep;
//         if(DEBUGMODE_AUV) cout<<"\t AUVClass: page 81 \n";
        
//         // updating attributes of members
//         this->syncComponentAttributes();
//         if(DEBUGMODE_AUV) cout<<"\t AUVClass: page 85 \n";
//     }



//     /*==========================================================================
//     Aim: updateAttributes
//     --------------------------------------------------------------------------*/
//     void syncComponentAttributes(){
        
//         // updating ULA attributes
//         if(DEBUGMODE_AUV) cout<<"\t AUVClass: page 97 \n";

//         // updating locations
//         this->ULA_fls.location          = this->location;
//         this->ULA_port.location         = this->location;
//         this->ULA_starboard.location    = this->location;

//         // updating the pointing direction of the ULAs
//         Tensor ula_fls_sensor_direction_spherical    = \
//             fCart2Sph(this->pointing_direction);              // spherical coords
//         ula_fls_sensor_direction_spherical[0]               = \
//             ula_fls_sensor_direction_spherical[0] - 90;
//         Tensor ula_fls_sensor_direction_cart         = \
//             fSph2Cart(ula_fls_sensor_direction_spherical);

//         this->ULA_fls.sensorDirection           = ula_fls_sensor_direction_cart;    // assigning sensor directionf or ULA-FLS
//         this->ULA_port.sensorDirection          = -this->pointing_direction;        // assigning sensor direction for ULA-Port
//         this->ULA_starboard.sensorDirection     = -this->pointing_direction;        // assigning sensor direction for ULA-Starboard

//         // // calling the function to update the arguments
//         // this->ULA_fls.buildCoordinatesBasedOnLocation();        if(DEBUGMODE_AUV) cout<<"\t AUVClass: line 109 \n";
//         // this->ULA_port.buildCoordinatesBasedOnLocation();       if(DEBUGMODE_AUV) cout<<"\t AUVClass: line 111 \n";
//         // this->ULA_starboard.buildCoordinatesBasedOnLocation();  if(DEBUGMODE_AUV) cout<<"\t AUVClass: line 113 \n";
        
//         // updating transmitter locations
//         this->transmitter_fls.location       = this->location;
//         this->transmitter_port.location      = this->location;
//         this->transmitter_starboard.location = this->location;

//         // updating transmitter pointing directions
//         this->transmitter_fls.updatePointingAngle(          this->pointing_direction);
//         this->transmitter_port.updatePointingAngle(         this->pointing_direction);
//         this->transmitter_starboard.updatePointingAngle(    this->pointing_direction);
//     }



//     /*==========================================================================
//     Aim: operator overriding for printing 
//     --------------------------------------------------------------------------*/ 
//     friend ostream& operator<<(ostream& os, AUVClass &auv){
//         os<<"\t location = "<<transpose(auv.location, 0, 1)<<endl;
//         os<<"\t velocity = "<<transpose(auv.velocity, 0, 1)<<endl;
//         return os;
//     }


//     /*==========================================================================
//     Aim: Subsetting Scatterers
//     --------------------------------------------------------------------------*/ 
//     void subsetScatterers(ScattererClass* scatterers,\
//                           TransmitterClass* transmitterObj,\
//                           float tilt_angle){

//         // ensuring components are synced
//         this->syncComponentAttributes();
//         if(DEBUGMODE_AUV) cout<<"\t AUVClass: page 120 \n";

//         // calling the method associated with the transmitter
//         if(DEBUGMODE_AUV) {cout<<"\t\t scatterers.shape = "; fPrintTensorSize(scatterers->coordinates);}
//         if(DEBUGMODE_AUV) cout<<"\t\t tilt_angle = "<<tilt_angle<<endl;
//         transmitterObj->subsetScatterers(scatterers, tilt_angle);
//         if(DEBUGMODE_AUV) cout<<"\t AUVClass: page 124 \n";
//     }

//     // yaw-correction matrix
//     Tensor createYawCorrectionMatrix(Tensor pointing_direction_spherical, \
//                                             float target_azimuth_deg){

//         // building parameters
//         Tensor azimuth_correction            = tensor({target_azimuth_deg}).to(DATATYPE).to(DEVICE) - \
//                                                       pointing_direction_spherical[0];
//         Tensor azimuth_correction_radians    = azimuth_correction * PI / 180;

//         Tensor yawCorrectionMatrix = \
//             tensor({cos(azimuth_correction_radians).item<float>(),      \
//                            cos(tensor({90}).to(DATATYPE).to(DEVICE)*PI/180 + azimuth_correction_radians).item<float>(), \
//                            (float)0,                                                         \
//                            sin(azimuth_correction_radians).item<float>(),      \
//                            sin(tensor({90}).to(DATATYPE).to(DEVICE)*PI/180 + azimuth_correction_radians).item<float>(), \
//                            (float)0,                                                         \
//                            (float)0,                                                         \
//                            (float)0,                                                         \
//                            (float)1}).reshape({3,3}).to(DATATYPE).to(DEVICE);

//         // returning the matrix
//         return yawCorrectionMatrix;
//     }

//     // pitch-correction matrix
//     Tensor createPitchCorrectionMatrix(Tensor pointing_direction_spherical, \
//                                               float target_elevation_deg){

//         // building parameters
//         Tensor elevation_correction            = tensor({target_elevation_deg}).to(DATATYPE).to(DEVICE) - \
//                                                         pointing_direction_spherical[1];
//         Tensor elevation_correction_radians    = elevation_correction * PI / 180;

//         // creating the matrix
//         Tensor pitchCorrectionMatrix = \
//             tensor({(float)1,                                                           \
//                            (float)0,                                                           \
//                            (float)0,                                                           \
//                            (float)0,                                                           \
//                            cos(elevation_correction_radians).item<float>(),      \
//                            cos(tensor({90}).to(DATATYPE).to(DEVICE)*PI/180 + elevation_correction_radians).item<float>(),\
//                            (float)0,                                                           \
//                            sin(elevation_correction_radians).item<float>(),      \
//                            sin(tensor({90}).to(DATATYPE).to(DEVICE)*PI/180 + elevation_correction_radians).item<float>()}).reshape({3,3}).to(DATATYPE);

//         // returning the matrix
//         return pitchCorrectionMatrix;
//     }

//     // Signal Simulation
//     void simulateSignal(ScattererClass& scatterer){

//         // printing status
//         cout << "\t AUVClass::simulateSignal: Began Signal Simulation" << endl;

//         // making three copies
//         ScattererClass scatterer_fls        = scatterer;
//         ScattererClass scatterer_port       = scatterer;
//         ScattererClass scatterer_starboard  = scatterer;

//         // finding the pointing direction in spherical
//         Tensor auv_pointing_direction_spherical = fCart2Sph(this->pointing_direction);

//         // asking the transmitters to subset the scatterers by multithreading
//         thread transmitterFLSSubset_t(&AUVClass::subsetScatterers, this, \
//                                            &scatterer_fls,\
//                                            &this->transmitter_fls, \
//                                            (float)0);
//         thread transmitterPortSubset_t(&AUVClass::subsetScatterers, this, \
//                                             &scatterer_port,\
//                                             &this->transmitter_port, \
//                                             auv_pointing_direction_spherical[1].item<float>());
//         thread transmitterStarboardSubset_t(&AUVClass::subsetScatterers, this, \
//                                                  &scatterer_starboard, \
//                                                  &this->transmitter_starboard, \
//                                                  - auv_pointing_direction_spherical[1].item<float>());

//         // joining the subset threads back
//         transmitterFLSSubset_t.join(); 
//         transmitterPortSubset_t.join(); 
//         transmitterStarboardSubset_t.join();


//         // multithreading the saving tensors part. 
//         thread savetensor_t(fSaveSeafloorScatteres,    \
//                                  scatterer,                 \
//                                  scatterer_fls,             \
//                                  scatterer_port,            \
//                                  scatterer_starboard);


//         // asking ULAs to simulate signal through multithreading
//         thread ulafls_signalsim_t(&ULAClass::nfdc_simulateSignal,          \
//                                        &this->ULA_fls,                          \
//                                        &scatterer_fls,                          \
//                                        &this->transmitter_fls);
//         thread ulaport_signalsim_t(&ULAClass::nfdc_simulateSignal,         \
//                                         &this->ULA_port,                        \
//                                         &scatterer_port,                        \
//                                         &this->transmitter_port);
//         thread ulastarboard_signalsim_t(&ULAClass::nfdc_simulateSignal,    \
//                                              &this->ULA_starboard,              \
//                                              &scatterer_starboard,              \
//                                              &this->transmitter_starboard);

//         // joining them back
//         ulafls_signalsim_t.join();          // joining back the thread for ULA-FLS
//         ulaport_signalsim_t.join();         // joining back the signals-sim thread for ULA-Port
//         ulastarboard_signalsim_t.join();    // joining back the signal-sim thread for ULA-Starboard
//         savetensor_t.join();                // joining back the signal-sim thread for tensor-saving


//     }

//     // Imaging Function
//     /* =========================================================================
//     ------------------------------------------------------------------------- */ 
//     void image(){
        
//         // asking ULAs to decimate the signals obtained at each time step
//         thread ULA_fls_image_t(&ULAClass::nfdc_decimateSignal,             \
//                                     &this->ULA_fls,                             \
//                                     &this->transmitter_fls);
//         thread ULA_port_image_t(&ULAClass::nfdc_decimateSignal,            \
//                                      &this->ULA_port,                           \
//                                      &this->transmitter_port);
//         thread ULA_starboard_image_t(&ULAClass::nfdc_decimateSignal,       \
//                                           &this->ULA_starboard,                 \
//                                           &this->transmitter_starboard);

//         // joining the threads back
//         ULA_fls_image_t.join(); 
//         ULA_port_image_t.join(); 
//         ULA_starboard_image_t.join();

//         // saving the decimated signal
//         if (SAVE_DECIMATED_SIGNAL_MATRIX) {
//             cout << "\t AUVClass::image: saving decimated signal matrix" \
//                       << endl;
//             save(this->ULA_fls.signalMatrix, \
//                     "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/decimated_signalMatrix_fls.pt");
//             save(this->ULA_port.signalMatrix, \
//                         "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/decimated_signalMatrix_port.pt");
//             save(this->ULA_starboard.signalMatrix, \
//                         "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/decimated_signalMatrix_starboard.pt");
//         }
        
//         // asking ULAs to match-filter the signals
//         thread ULA_fls_matchfilter_t(                  \
//             &ULAClass::nfdc_matchFilterDecimatedSignal,     \
//             &this->ULA_fls);
//         thread ULA_port_matchfilter_t(                 \
//             &ULAClass::nfdc_matchFilterDecimatedSignal,     \
//             &this->ULA_port);
//         thread ULA_starboard_matchfilter_t(            \
//             &ULAClass::nfdc_matchFilterDecimatedSignal,     \
//             &this->ULA_starboard);

//         // joining the threads back
//         ULA_fls_matchfilter_t.join();
//         ULA_port_matchfilter_t.join();
//         ULA_starboard_matchfilter_t.join();


//         // saving the decimated signal
//         if (SAVE_MATCHFILTERED_SIGNAL_MATRIX) {

//             // saving the tensors
//             cout << "\t AUVClass::image: saving match-filtered signal matrix" \
//                       << endl;
//             save(this->ULA_fls.signalMatrix, \
//                     "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/matchfiltered_signalMatrix_fls.pt");
//             save(this->ULA_port.signalMatrix, \
//                         "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/matchfiltered_signalMatrix_port.pt");
//             save(this->ULA_starboard.signalMatrix, \
//                         "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/matchfiltered_signalMatrix_starboard.pt");

//             // running python-script
            
//         }

        
//         // performing the beamforming
//         thread ULA_fls_beamforming_t(&ULAClass::nfdc_beamforming,          \
//                                           &this->ULA_fls,                       \
//                                           &this->transmitter_fls);
//         thread ULA_port_beamforming_t(&ULAClass::nfdc_beamforming,         \
//                                            &this->ULA_port,                     \
//                                            &this->transmitter_port);
//         thread ULA_starboard_beamforming_t(&ULAClass::nfdc_beamforming,    \
//                                                 &this->ULA_starboard,           \
//                                                 &this->transmitter_starboard);

//         // joining the filters back
//         ULA_fls_beamforming_t.join(); 
//         ULA_port_beamforming_t.join(); 
//         ULA_starboard_beamforming_t.join();


//     }


    

//     /* =========================================================================
//     Aim: directly create acoustic image
//     ------------------------------------------------------------------------- */ 
//     void createAcousticImage(ScattererClass* scatterers){
        
//         // making three copies
//         ScattererClass scatterer_fls        = scatterers;
//         ScattererClass scatterer_port       = scatterers;
//         ScattererClass scatterer_starboard  = scatterers;

//         // printing size of scatterers before subsetting
//         PRINTSMALLLINE
//         cout<< "\t > AUVClass::createAcousticImage: Beginning Scatterer Subsetting"<<endl;
//         cout<<"\t AUVClass::createAcousticImage: scatterer_fls.coordinates.shape (before)        = "; fPrintTensorSize(scatterer_fls.coordinates);
//         cout<<"\t AUVClass::createAcousticImage: scatterer_port.coordinates.shape (before)       = "; fPrintTensorSize(scatterer_port.coordinates);
//         cout<<"\t AUVClass::createAcousticImage: scatterer_starboard.coordinates.shape (before)  = "; fPrintTensorSize(scatterer_starboard.coordinates);

//         // finding the pointing direction in spherical
//         Tensor auv_pointing_direction_spherical = fCart2Sph(this->pointing_direction);

//         // asking the transmitters to subset the scatterers by multithreading
//         thread transmitterFLSSubset_t(&AUVClass::subsetScatterers, this, \
//                                            &scatterer_fls,\
//                                            &this->transmitter_fls, \
//                                            (float)0);
//         thread transmitterPortSubset_t(&AUVClass::subsetScatterers, this, \
//                                             &scatterer_port,\
//                                             &this->transmitter_port, \
//                                             auv_pointing_direction_spherical[1].item<float>());
//         thread transmitterStarboardSubset_t(&AUVClass::subsetScatterers, this, \
//                                                  &scatterer_starboard, \
//                                                  &this->transmitter_starboard, \
//                                                  - auv_pointing_direction_spherical[1].item<float>());

//         // joining the subset threads back
//         transmitterFLSSubset_t.join(        ); 
//         transmitterPortSubset_t.join(       ); 
//         transmitterStarboardSubset_t.join(  );


//         // asking the ULAs to directly create acoustic images
//         thread ULA_fls_acoustic_image_t(&ULAClass::nfdc_createAcousticImage, this->ULA_fls, \
//                                              &scatterer_fls, &this->transmitter_fls);
//         thread ULA_port_acoustic_image_t(&ULAClass::nfdc_createAcousticImage, &this->ULA_port, \
//                                              &scatterer_port, &this->transmitter_port);
//         thread ULA_starboard_acoustic_image_t(&ULAClass::nfdc_createAcousticImage, &this->ULA_starboard, \
//                                                    &scatterer_starboard, &this->transmitter_starboard);

//         // joining the threads back
//         ULA_fls_acoustic_image_t.join(      );
//         ULA_port_acoustic_image_t.join(     );
//         ULA_starboard_acoustic_image_t.join();
        
//     }


// };






















// // 0.0000, 
// // 0.0000, 
// // 0.0000, 
// // 0.0000, 
// // 0.0000, 
// // 0.0000, 
// // 0.0000, 
// // 0.0000, 
// // 0.0000, 
// // 0.0000, 
// // 0.0000, 
// // 0.0000, 
// // 0.0000, 
// // 0.0000, 
// // 0.0000, 
// // 0.0000, 
// // 0.0000, 
// // 0.0000, 
// // 0.0000, 
// // 0.0000, 
// // 0.0000, 
// // 0.0000, 
// // 0.0000, 
// // 0.0000, 
// // 0.0000, 
// // 0.0000, 
// // 0.0000, 
// // 0.0000, 
// // 0.0000, 
// // 0.0000, 
// // 0.0000, 
// // 0.0001, 
// // 0.0001, 
// // 0.0002, 
// // 0.0003, 
// // 0.0006, 
// // 0.0009, 
// // 0.0014, 
// // 0.0022, 0.0032, 0.0047, 0.0066, 0.0092, 0.0126, 0.0168, 0.0219, 0.0281, 0.0352, 0.0432, 0.0518, 0.0609, 0.0700, 0.0786, 0.0861, 0.0921, 0.0958, 0.0969, 0.0950, 0.0903, 0.0833, 0.0755, 0.0694, 0.0693, 0.0825, 0.1206












template <
    svr::PureFloatingPointType          T,
    svr::FFT_SourceDestination_Type     sourceType,
    svr::FFT_SourceDestination_Type     destinationType,
    svr::PureComplexFloatingType        T_PureComplex
>
class   AUVClass{
public:
    
    // Intrinsic attributes
    std::vector<T>      location;               // location of vessel
    std::vector<T>      velocity;               // velocity of the vessel
    std::vector<T>      acceleration;           // acceleration of vessel
    std::vector<T>      pointing_direction;     // AUV's pointing direction
    
    // uniform linear-arrays
    ULAClass<T, sourceType, destinationType, T_PureComplex>         ULA_fls;                // front-looking SONAR ULA
    ULAClass<T, sourceType, destinationType, T_PureComplex>         ULA_portside;           // mounted ULA [object of class, ULAClass]        
    ULAClass<T, sourceType, destinationType, T_PureComplex>         ULA_starboard;          // mounted ULA [object of class, ULAClass]        
    
    // transmitters
    TransmitterClass<T>     transmitter_fls;           // transmitter for front-looking SONAR
    TransmitterClass<T>     transmitter_portside;      // portside transmitter
    TransmitterClass<T>     transmitter_starboard;     // starboard transmitter

    // derived or dependent attributes
    std::vector<std::vector<T>>     signalMatrix_1;               // matrix containing the signals obtained from ULA_1
    std::vector<std::vector<T>>     largeSignalMatrix_1;          // matrix holding signal of synthetic aperture
    std::vector<std::vector<T>>     beamformedLargeSignalMatrix;  // each column is the beamformed signal at each stop-hop

    // plotting mode
    bool plottingmode;    // to suppress plotting associated with classes

    // spotlight mode related
    std::vector<std::vector<T>>     absolute_coords_patch_cart;     // cartesian coordinates of patch

    // Synthetic Aperture Related 
    std::vector<std::vector<T>>     ApertureSensorLocations;        // sensor locations of aperture

    // functions
    void    syncComponentAttributes();
    void    init(
        svr::ThreadPool&                                                 thread_pool,
        svr::FFTPlanUniformPoolHandle<  T_PureComplex, T_PureComplex>&   fph_match_filter,
        svr::IFFTPlanUniformPoolHandle< T_PureComplex, T_PureComplex>&   ifph_match_filter);
    void    simulate_signal(
        const     ScattererClass<T>&                                            seafloor,
        svr::ThreadPool&                                                        thread_pool,
        svr::FFTPlanUniformPoolHandle< sourceType, destinationType >&           fft_pool_handle,
        svr::IFFTPlanUniformPoolHandle< destinationType, sourceType >&          ifft_pool_handle);
    void    subset_scatterers(
        const  ScattererClass<T>&       seafloor,
        svr::ThreadPool&                thread_pool,
        std::vector<std::size_t>&       fls_scatterer_indices,
        std::vector<std::size_t>&       portside_scatterer_indices,
        std::vector<std::size_t>&       starboard_scatterer_indices);
    void    step(
        T time_step);
    void    image(
        svr::ThreadPool&                                                            thread_pool,
        svr::FFTPlanUniformPoolHandle<      sourceType,         destinationType>&   fft_pool_handle,
        svr::IFFTPlanUniformPoolHandle<     destinationType,    sourceType>&        ifft_pool_handle);
};

/*==========================================================================
Aim: update attributes
--------------------------------------------------------------------------*/
template <
    svr::PureFloatingPointType          T,
    svr::FFT_SourceDestination_Type     sourceType,
    svr::FFT_SourceDestination_Type     destinationType,
    svr::PureComplexFloatingType        T_PureComplex
>
void AUVClass<T, sourceType, destinationType, T_PureComplex>::syncComponentAttributes(){
    // updating locations of ULAs
    this->ULA_fls.location          = this->location;
    this->ULA_portside.location     = this->location;
    this->ULA_starboard.location    = this->location;
    
    // updating pointing-direction of ULAs
    auto    ula_fls_sensor_direction_spherical      {svr::cart2sph(this->pointing_direction)};
    ula_fls_sensor_direction_spherical[0]           -=  90;
    auto    ula_fls_sensor_direction_cart           {svr::sph2cart(ula_fls_sensor_direction_spherical)};

    this->ULA_fls.sensor_direction          =   ula_fls_sensor_direction_cart;
    this->ULA_portside.sensor_direction     =   -1  *   this->pointing_direction;
    this->ULA_starboard.sensor_direction    =   -1  *   this->pointing_direction;

    // calling function to update arguments (verified ula-coordinates)
    this->ULA_fls.buildCoordinatesBasedOnLocation(          this->location);
    this->ULA_portside.buildCoordinatesBasedOnLocation(     this->location);
    this->ULA_starboard.buildCoordinatesBasedOnLocation(    this->location);

    // updating transmitter location
    this->transmitter_fls.location                  =   this->location;
    this->transmitter_portside.location             =   this->location;
    this->transmitter_starboard.location            =   this->location;

    // updating transmitter pointing direction
    this->transmitter_fls.updatePointingAngle(          this->pointing_direction);
    this->transmitter_portside.updatePointingAngle(     this->pointing_direction);
    this->transmitter_starboard.updatePointingAngle(    this->pointing_direction);}
/* =========================================================================
Aim: Initializing objects and variables
--------------------------------------------------------------------------*/ 
template <
    svr::PureFloatingPointType          T,
    svr::FFT_SourceDestination_Type     sourceType,
    svr::FFT_SourceDestination_Type     destinationType,
    svr::PureComplexFloatingType        T_PureComplex
>
void AUVClass<T, sourceType, destinationType, T_PureComplex>::init(
    svr::ThreadPool&                                                 thread_pool,
    svr::FFTPlanUniformPoolHandle<  T_PureComplex, T_PureComplex>&   fph_match_filter,
    svr::IFFTPlanUniformPoolHandle< T_PureComplex, T_PureComplex>&   ifph_match_filter
){
    // call sync-component attributes
    this->syncComponentAttributes();

    // initializing the ULAs
    thread_pool.push_back([&]{this->ULA_fls.init(         
        std::ref(this->transmitter_fls),
        fph_match_filter,
        ifph_match_filter);});
    thread_pool.push_back([&]{this->ULA_portside.init(    
        std::ref(this->transmitter_portside),
        fph_match_filter,
        ifph_match_filter);});
    thread_pool.push_back([&]{this->ULA_starboard.init(   
        std::ref(this->transmitter_starboard),
        fph_match_filter,
        ifph_match_filter);});
    thread_pool.converge();

    // logging
    spdlog::info("Finished AUV Initialization");}
/*==============================================================================
Member-Function: Subsetting the scatterers in the transmitter-range
------------------------------------------------------------------------------*/ 
template <
    svr::PureFloatingPointType          T,
    svr::FFT_SourceDestination_Type     sourceType,
    svr::FFT_SourceDestination_Type     destinationType,
    svr::PureComplexFloatingType        T_PureComplex
>
void AUVClass<T, sourceType, destinationType, T_PureComplex>::subset_scatterers(
    const  ScattererClass<T>&    seafloor,
    svr::ThreadPool&             thread_pool,
    std::vector<std::size_t>&    fls_scatterer_indices,
    std::vector<std::size_t>&    portside_scatterer_indices,
    std::vector<std::size_t>&    starboard_scatterer_indices
){
    // ensuring the components are synced
    this->syncComponentAttributes();

    // finding the pointing direction in spherical 
    auto    auv_pointing_direction_spherical    {svr::cart2sph(this->pointing_direction)};

    // spinning up the thread-pool
    thread_pool.push_back(
        [&]{transmitter_fls.subset_scatterers(seafloor,
                                              fls_scatterer_indices,
                                              0);}
    );
    thread_pool.push_back(
        [&]{transmitter_portside.subset_scatterers(seafloor,
                                                   portside_scatterer_indices,
                                                   auv_pointing_direction_spherical[1]);}
    );
    thread_pool.push_back(
        [&]{transmitter_starboard.subset_scatterers(seafloor,
                                                    starboard_scatterer_indices,
                                                    -1*auv_pointing_direction_spherical[1]);
                                                }
    );

    // waiting until all the work is done
    thread_pool.converge();
}

/*==============================================================================
Aim: Simulate Signals received by ULAs in the AUV
------------------------------------------------------------------------------*/ 
template <
    svr::PureFloatingPointType          T,
    svr::FFT_SourceDestination_Type     sourceType,
    svr::FFT_SourceDestination_Type     destinationType,
    svr::PureComplexFloatingType        T_PureComplex
>
void AUVClass<T, sourceType, destinationType, T_PureComplex>::simulate_signal(
    const     ScattererClass<T>&                                                seafloor,
    svr::ThreadPool&                                                            thread_pool,
    svr::FFTPlanUniformPoolHandle<  sourceType,      destinationType     >&     fft_pool_handle,
    svr::IFFTPlanUniformPoolHandle< destinationType, sourceType         >&      ifft_pool_handle
){
    // boolean-vector indicating which scatterers are present 
    auto    fls_scatterer_indices               {std::vector<std::size_t>()};
    auto    portside_scatterer_indices          {std::vector<std::size_t>()};
    auto    starboard_scatterer_indices         {std::vector<std::size_t>()};

    // asking the transmitters subset the scatterers 
    this->subset_scatterers(seafloor,
                            thread_pool,
                            fls_scatterer_indices,
                            portside_scatterer_indices,
                            starboard_scatterer_indices);


    // asking ULAs to simulate signal
    thread_pool.push_back(
        [&]{this->ULA_fls.simulate_signals(
            seafloor,
            fls_scatterer_indices,
            this->transmitter_fls,
            fft_pool_handle,
            ifft_pool_handle
        );});
    thread_pool.push_back(
        [&](){this->ULA_portside.simulate_signals(
            seafloor, 
            portside_scatterer_indices,
            this->transmitter_portside,
            fft_pool_handle,
            ifft_pool_handle
        );});
    thread_pool.push_back(
        [&](){this->ULA_starboard.simulate_signals(
            seafloor,
            starboard_scatterer_indices,
            this->transmitter_starboard,
            fft_pool_handle,
            ifft_pool_handle
        );});
    
    // waiting for threads to converge
    thread_pool.converge();

    // logging
    spdlog::info("Signal-Simulation Complete");

}
/*==============================================================================
Aim: Moving the AUV to the next discrete position in the trajectory
------------------------------------------------------------------------------*/ 
template <
    svr::PureFloatingPointType          T,
    svr::FFT_SourceDestination_Type     sourceType,
    svr::FFT_SourceDestination_Type     destinationType,
    svr::PureComplexFloatingType        T_PureComplex
>
void AUVClass<T, sourceType, destinationType, T_PureComplex>::step(
    T    time_step
){
    // updating location
    this->location  = this->location + this->velocity * time_step;

    // update parameters of attached components
    this->syncComponentAttributes();
}
/*==============================================================================
Aim: Function that begins imaging from the recorded signals
------------------------------------------------------------------------------*/
template <
    svr::PureFloatingPointType          T,
    svr::FFT_SourceDestination_Type     sourceType,
    svr::FFT_SourceDestination_Type     destinationType,
    svr::PureComplexFloatingType        T_PureComplex
>
void AUVClass<T, sourceType, destinationType, T_PureComplex>::image(
    svr::ThreadPool&                                                            thread_pool,
    svr::FFTPlanUniformPoolHandle<      sourceType,         destinationType>&   fft_pool_handle,
    svr::IFFTPlanUniformPoolHandle<     destinationType,    sourceType>&        ifft_pool_handle
)
{
    // // decimating signals obtained at each time-step
    // this->ULA_fls.decimate_signal(
    //     this->transmitter_fls,
    //     fft_pool_handle,
    //     ifft_pool_handle
    // );

    // decimating signals obtained at each time-step
    this->ULA_fls.decimate_signal(
        this->transmitter_fls
    );

    // 

}