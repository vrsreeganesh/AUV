

#pragma once

// function to plot the thing
void fPlotTensors(){
    system("python /Users/vrsreeganesh/Documents/GitHub/AUV/Code/Python/TestingSaved_tensors.py");
}


void fSaveSeafloorScatteres(ScattererClass scatterer, \
                            ScattererClass scatterer_fls, \
                            ScattererClass scatterer_port, \
                            ScattererClass scatterer_starboard){

    // saving the ground-truth
    ScattererClass SeafloorScatter_gt = scatterer;
    torch::save(SeafloorScatter_gt.coordinates, "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_gt.pt");
    torch::save(SeafloorScatter_gt.reflectivity, "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_gt_reflectivity.pt");
    
    // saving coordinates 
    torch::save(scatterer_fls.coordinates, "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_fls_coordinates.pt");
    torch::save(scatterer_port.coordinates, "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_port_coordinates.pt");
    torch::save(scatterer_starboard.coordinates, "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_starboard.coordinates.pt");

    // saving reflectivities
    torch::save(scatterer_fls.reflectivity, "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_fls_coordinates_reflectivity.pt");
    torch::save(scatterer_port.reflectivity, "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_port_coordinates_reflectivity.pt");
    torch::save(scatterer_starboard.reflectivity, "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_starboard.coordinates_reflectivity.pt");

    // plotting tensors
    fPlotTensors();

    // // saving the tensors
    // if (true) {

    //     // getting time ID
    //     auto timeID = fGetCurrentTimeFormatted();
        
    //     std::cout<<"\t\t\t\t\t\t\t Saving Tensors (timeID: "<<timeID<<")"<<std::endl;

    //     // saving the ground-truth
    //     ScattererClass SeafloorScatter_gt = scatterer;
    //     torch::save(SeafloorScatter_gt.coordinates, \
    //                 "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_gt.pt");
    //     torch::save(SeafloorScatter_gt.reflectivity, \
    //                 "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_gt_reflectivity.pt");
        

    //     // saving coordinates 
    //     torch::save(scatterer_fls.coordinates, \
    //             "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_fls_coordinates.pt");
    //     torch::save(scatterer_port.coordinates, \
    //                 "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_port_coordinates.pt");
    //     torch::save(scatterer_starboard.coordinates, \
    //                 "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_starboard.coordinates.pt");

    //     // saving reflectivities
    //     torch::save(scatterer_fls.reflectivity, \
    //             "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_fls_coordinates_reflectivity.pt");
    //     torch::save(scatterer_port.reflectivity, \
    //                 "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_port_coordinates_reflectivity.pt");
    //     torch::save(scatterer_starboard.reflectivity, \
    //                 "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_starboard.coordinates_reflectivity.pt");

    //     // plotting tensors
    //     fPlotTensors();

    //     // indicating end of thread 
    //     std::cout<<"\t\t\t\t\t\t\t Ended (timeID: "<<timeID<<")"<<std::endl;
    // }
}


// hash-defines
#define PI                                  3.14159265
#define DEBUGMODE_AUV                       false
#define SAVE_SIGNAL_MATRIX                  true
#define SAVE_DECIMATED_SIGNAL_MATRIX        true
#define SAVE_MATCHFILTERED_SIGNAL_MATRIX    true

class AUVClass{
public:
    // Intrinsic attributes
    torch::Tensor location;                     // location of vessel
    torch::Tensor velocity;                     // current speed of the vessel [a vector]
    torch::Tensor acceleration;                 // current acceleration of vessel [a vector]
    torch::Tensor pointing_direction;           // direction to which the AUV is pointed
    
    // uniform linear-arrays
    ULAClass ULA_fls;                           // front-looking SONAR ULA
    ULAClass ULA_port;                          // mounted ULA [object of class, ULAClass]        
    ULAClass ULA_starboard;                     // mounted ULA [object of class, ULAClass]        
    
    // transmitters
    TransmitterClass transmitter_fls;           // transmitter for front-looking SONAR
    TransmitterClass transmitter_port;          // mounted transmitter [obj of class, TransmitterClass]
    TransmitterClass transmitter_starboard;     // mounted transmitter [obj of class, TransmitterClass]

    // derived or dependent attributes
    torch::Tensor signalMatrix_1;               // matrix containing the signals obtained from ULA_1
    torch::Tensor largeSignalMatrix_1;          // matrix holding signal of synthetic aperture
    torch::Tensor beamformedLargeSignalMatrix;  // each column is the beamformed signal at each stop-hop

    // plotting mode
    bool plottingmode;    // to suppress plotting associated with classes

    // spotlight mode related
    torch::Tensor absolute_coords_patch_cart; // cartesian coordinates of patch

    // Synthetic Aperture Related 
    torch::Tensor ApertureSensorLocations;    // sensor locations of aperture








    /* =========================================================================
    Aim: Init
    --------------------------------------------------------------------------*/ 
    void init(){
        
        // call sync-component attributes
        this->syncComponentAttributes();
        if (DEBUGMODE_AUV) std::cout << "AUVCLass::init: line 128" << std::endl;

        // initializing all the ULAs 
        this->ULA_fls.init(         &this->transmitter_fls); 
        this->ULA_port.init(        &this->transmitter_port); 
        this->ULA_starboard.init(   &this->transmitter_starboard);
        if (DEBUGMODE_AUV) std::cout << "AUVCLass::init: line 134" << std::endl;
        

        // precomputing delay-matrices for the ULA-class
        std::thread ULA_fls_precompute_weights_t(&ULAClass::nfdc_precomputeDelayMatrices,       \
                                                 &this->ULA_fls,                                \
                                                 &this->transmitter_fls);
        std::thread ULA_port_precompute_weights_t(&ULAClass::nfdc_precomputeDelayMatrices,      \
                                                  &this->ULA_port,                              \
                                                  &this->transmitter_port);
        std::thread ULA_starboard_precompute_weights_t(&ULAClass::nfdc_precomputeDelayMatrices, \
                                                       &this->ULA_starboard,                    \
                                                       &this->transmitter_starboard);
        if (DEBUGMODE_AUV) std::cout << "AUVCLass::init: line 145" << std::endl;

        // joining the threads back
        ULA_fls_precompute_weights_t.join();
        ULA_port_precompute_weights_t.join();
        ULA_starboard_precompute_weights_t.join();

    }


    
    /*==========================================================================
    Aim: stepping motion
    --------------------------------------------------------------------------*/ 
    void step(float timestep){
        
        // updating location
        this->location = this->location + this->velocity * timestep;
        if(DEBUGMODE_AUV) std::cout<<"\t AUVClass: page 81 \n";
        
        // updating attributes of members
        this->syncComponentAttributes();
        if(DEBUGMODE_AUV) std::cout<<"\t AUVClass: page 85 \n";
    }



    /*==========================================================================
    Aim: updateAttributes
    --------------------------------------------------------------------------*/
    void syncComponentAttributes(){
        
        // updating ULA attributes
        if(DEBUGMODE_AUV) std::cout<<"\t AUVClass: page 97 \n";

        // updating locations
        this->ULA_fls.location          = this->location;
        this->ULA_port.location         = this->location;
        this->ULA_starboard.location    = this->location;

        // updating the pointing direction of the ULAs
        torch::Tensor ula_fls_sensor_direction_spherical    = \
            fCart2Sph(this->pointing_direction);              // spherical coords
        ula_fls_sensor_direction_spherical[0]               = \
            ula_fls_sensor_direction_spherical[0] - 90;
        torch::Tensor ula_fls_sensor_direction_cart         = \
            fSph2Cart(ula_fls_sensor_direction_spherical);

        this->ULA_fls.sensorDirection           = ula_fls_sensor_direction_cart;    // assigning sensor directionf or ULA-FLS
        this->ULA_port.sensorDirection          = -this->pointing_direction;        // assigning sensor direction for ULA-Port
        this->ULA_starboard.sensorDirection     = -this->pointing_direction;        // assigning sensor direction for ULA-Starboard

        // // calling the function to update the arguments
        // this->ULA_fls.buildCoordinatesBasedOnLocation();        if(DEBUGMODE_AUV) std::cout<<"\t AUVClass: line 109 \n";
        // this->ULA_port.buildCoordinatesBasedOnLocation();       if(DEBUGMODE_AUV) std::cout<<"\t AUVClass: line 111 \n";
        // this->ULA_starboard.buildCoordinatesBasedOnLocation();  if(DEBUGMODE_AUV) std::cout<<"\t AUVClass: line 113 \n";
        
        // updating transmitter locations
        this->transmitter_fls.location       = this->location;
        this->transmitter_port.location      = this->location;
        this->transmitter_starboard.location = this->location;

        // updating transmitter pointing directions
        this->transmitter_fls.updatePointingAngle(          this->pointing_direction);
        this->transmitter_port.updatePointingAngle(         this->pointing_direction);
        this->transmitter_starboard.updatePointingAngle(    this->pointing_direction);
    }



    /*==========================================================================
    Aim: operator overriding for printing 
    --------------------------------------------------------------------------*/ 
    friend std::ostream& operator<<(std::ostream& os, AUVClass &auv){
        os<<"\t location = "<<torch::transpose(auv.location, 0, 1)<<std::endl;
        os<<"\t velocity = "<<torch::transpose(auv.velocity, 0, 1)<<std::endl;
        return os;
    }


    /*==========================================================================
    Aim: Subsetting Scatterers
    --------------------------------------------------------------------------*/ 
    void subsetScatterers(ScattererClass* scatterers,\
                          TransmitterClass* transmitterObj,\
                          float tilt_angle){

        // ensuring components are synced
        this->syncComponentAttributes();
        if(DEBUGMODE_AUV) std::cout<<"\t AUVClass: page 120 \n";

        // calling the method associated with the transmitter
        if(DEBUGMODE_AUV) {std::cout<<"\t\t scatterers.shape = "; fPrintTensorSize(scatterers->coordinates);}
        if(DEBUGMODE_AUV) std::cout<<"\t\t tilt_angle = "<<tilt_angle<<std::endl;
        transmitterObj->subsetScatterers(scatterers, tilt_angle);
        if(DEBUGMODE_AUV) std::cout<<"\t AUVClass: page 124 \n";
    }

    // yaw-correction matrix
    torch::Tensor createYawCorrectionMatrix(torch::Tensor pointing_direction_spherical, \
                                            float target_azimuth_deg){

        // building parameters
        torch::Tensor azimuth_correction            = torch::tensor({target_azimuth_deg}).to(DATATYPE).to(DEVICE) - \
                                                      pointing_direction_spherical[0];
        torch::Tensor azimuth_correction_radians    = azimuth_correction * PI / 180;

        torch::Tensor yawCorrectionMatrix = \
            torch::tensor({torch::cos(azimuth_correction_radians).item<float>(),      \
                           torch::cos(torch::tensor({90}).to(DATATYPE).to(DEVICE)*PI/180 + azimuth_correction_radians).item<float>(), \
                           (float)0,                                                         \
                           torch::sin(azimuth_correction_radians).item<float>(),      \
                           torch::sin(torch::tensor({90}).to(DATATYPE).to(DEVICE)*PI/180 + azimuth_correction_radians).item<float>(), \
                           (float)0,                                                         \
                           (float)0,                                                         \
                           (float)0,                                                         \
                           (float)1}).reshape({3,3}).to(DATATYPE).to(DEVICE);

        // returning the matrix
        return yawCorrectionMatrix;
    }

    // pitch-correction matrix
    torch::Tensor createPitchCorrectionMatrix(torch::Tensor pointing_direction_spherical, \
                                              float target_elevation_deg){

        // building parameters
        torch::Tensor elevation_correction            = torch::tensor({target_elevation_deg}).to(DATATYPE).to(DEVICE) - \
                                                        pointing_direction_spherical[1];
        torch::Tensor elevation_correction_radians    = elevation_correction * PI / 180;

        // creating the matrix
        torch::Tensor pitchCorrectionMatrix = \
            torch::tensor({(float)1,                                                           \
                           (float)0,                                                           \
                           (float)0,                                                           \
                           (float)0,                                                           \
                           torch::cos(elevation_correction_radians).item<float>(),      \
                           torch::cos(torch::tensor({90}).to(DATATYPE).to(DEVICE)*PI/180 + elevation_correction_radians).item<float>(),\
                           (float)0,                                                           \
                           torch::sin(elevation_correction_radians).item<float>(),      \
                           torch::sin(torch::tensor({90}).to(DATATYPE).to(DEVICE)*PI/180 + elevation_correction_radians).item<float>()}).reshape({3,3}).to(DATATYPE);

        // returning the matrix
        return pitchCorrectionMatrix;
    }

    // Signal Simulation
    void simulateSignal(ScattererClass& scatterer){

        // printing status
        std::cout << "\t AUVClass::simulateSignal: Began Signal Simulation" << std::endl;

        // making three copies
        ScattererClass scatterer_fls        = scatterer;
        ScattererClass scatterer_port       = scatterer;
        ScattererClass scatterer_starboard  = scatterer;

        // finding the pointing direction in spherical
        torch::Tensor auv_pointing_direction_spherical = fCart2Sph(this->pointing_direction);

        // asking the transmitters to subset the scatterers by multithreading
        std::thread transmitterFLSSubset_t(&AUVClass::subsetScatterers, this, \
                                           &scatterer_fls,\
                                           &this->transmitter_fls, \
                                           (float)0);
        std::thread transmitterPortSubset_t(&AUVClass::subsetScatterers, this, \
                                            &scatterer_port,\
                                            &this->transmitter_port, \
                                            auv_pointing_direction_spherical[1].item<float>());
        std::thread transmitterStarboardSubset_t(&AUVClass::subsetScatterers, this, \
                                                 &scatterer_starboard, \
                                                 &this->transmitter_starboard, \
                                                 - auv_pointing_direction_spherical[1].item<float>());

        // joining the subset threads back
        transmitterFLSSubset_t.join(); 
        transmitterPortSubset_t.join(); 
        transmitterStarboardSubset_t.join();


        // multithreading the saving tensors part. 
        std::thread savetensor_t(fSaveSeafloorScatteres,    \
                                 scatterer,                 \
                                 scatterer_fls,             \
                                 scatterer_port,            \
                                 scatterer_starboard);


        // asking ULAs to simulate signal through multithreading
        std::thread ulafls_signalsim_t(&ULAClass::nfdc_simulateSignal,          \
                                       &this->ULA_fls,                          \
                                       &scatterer_fls,                          \
                                       &this->transmitter_fls);
        std::thread ulaport_signalsim_t(&ULAClass::nfdc_simulateSignal,         \
                                        &this->ULA_port,                        \
                                        &scatterer_port,                        \
                                        &this->transmitter_port);
        std::thread ulastarboard_signalsim_t(&ULAClass::nfdc_simulateSignal,    \
                                             &this->ULA_starboard,              \
                                             &scatterer_starboard,              \
                                             &this->transmitter_starboard);

        // joining them back
        ulafls_signalsim_t.join();          // joining back the thread for ULA-FLS
        ulaport_signalsim_t.join();         // joining back the signals-sim thread for ULA-Port
        ulastarboard_signalsim_t.join();    // joining back the signal-sim thread for ULA-Starboard
        savetensor_t.join();                // joining back the signal-sim thread for tensor-saving


    }

    // Imaging Function
    /* =========================================================================
    ------------------------------------------------------------------------- */ 
    void image(){
        
        // asking ULAs to decimate the signals obtained at each time step
        std::thread ULA_fls_image_t(&ULAClass::nfdc_decimateSignal,             \
                                    &this->ULA_fls,                             \
                                    &this->transmitter_fls);
        std::thread ULA_port_image_t(&ULAClass::nfdc_decimateSignal,            \
                                     &this->ULA_port,                           \
                                     &this->transmitter_port);
        std::thread ULA_starboard_image_t(&ULAClass::nfdc_decimateSignal,       \
                                          &this->ULA_starboard,                 \
                                          &this->transmitter_starboard);

        // joining the threads back
        ULA_fls_image_t.join(); 
        ULA_port_image_t.join(); 
        ULA_starboard_image_t.join();

        // saving the decimated signal
        if (SAVE_DECIMATED_SIGNAL_MATRIX) {
            std::cout << "\t AUVClass::image: saving decimated signal matrix" \
                      << std::endl;
            torch::save(this->ULA_fls.signalMatrix, \
                    "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/decimated_signalMatrix_fls.pt");
            torch::save(this->ULA_port.signalMatrix, \
                        "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/decimated_signalMatrix_port.pt");
            torch::save(this->ULA_starboard.signalMatrix, \
                        "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/decimated_signalMatrix_starboard.pt");
        }
        
        // asking ULAs to match-filter the signals
        std::thread ULA_fls_matchfilter_t(                  \
            &ULAClass::nfdc_matchFilterDecimatedSignal,     \
            &this->ULA_fls);
        std::thread ULA_port_matchfilter_t(                 \
            &ULAClass::nfdc_matchFilterDecimatedSignal,     \
            &this->ULA_port);
        std::thread ULA_starboard_matchfilter_t(            \
            &ULAClass::nfdc_matchFilterDecimatedSignal,     \
            &this->ULA_starboard);

        // joining the threads back
        ULA_fls_matchfilter_t.join();
        ULA_port_matchfilter_t.join();
        ULA_starboard_matchfilter_t.join();


        // saving the decimated signal
        if (SAVE_MATCHFILTERED_SIGNAL_MATRIX) {

            // saving the tensors
            std::cout << "\t AUVClass::image: saving match-filtered signal matrix" \
                      << std::endl;
            torch::save(this->ULA_fls.signalMatrix, \
                    "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/matchfiltered_signalMatrix_fls.pt");
            torch::save(this->ULA_port.signalMatrix, \
                        "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/matchfiltered_signalMatrix_port.pt");
            torch::save(this->ULA_starboard.signalMatrix, \
                        "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/matchfiltered_signalMatrix_starboard.pt");

            // running python-script
            
        }

        
        // performing the beamforming
        std::thread ULA_fls_beamforming_t(&ULAClass::nfdc_beamforming,          \
                                          &this->ULA_fls,                       \
                                          &this->transmitter_fls);
        std::thread ULA_port_beamforming_t(&ULAClass::nfdc_beamforming,         \
                                           &this->ULA_port,                     \
                                           &this->transmitter_port);
        std::thread ULA_starboard_beamforming_t(&ULAClass::nfdc_beamforming,    \
                                                &this->ULA_starboard,           \
                                                &this->transmitter_starboard);

        // joining the filters back
        ULA_fls_beamforming_t.join(); 
        ULA_port_beamforming_t.join(); 
        ULA_starboard_beamforming_t.join();


    }


    

    /* =========================================================================
    Aim: directly create acoustic image
    ------------------------------------------------------------------------- */ 
    void createAcousticImage(ScattererClass* scatterers){
        
        // making three copies
        ScattererClass scatterer_fls        = scatterers;
        ScattererClass scatterer_port       = scatterers;
        ScattererClass scatterer_starboard  = scatterers;

        // printing size of scatterers before subsetting
        PRINTSMALLLINE
        std::cout<< "\t > AUVClass::createAcousticImage: Beginning Scatterer Subsetting"<<std::endl;
        std::cout<<"\t AUVClass::createAcousticImage: scatterer_fls.coordinates.shape (before)        = "; fPrintTensorSize(scatterer_fls.coordinates);
        std::cout<<"\t AUVClass::createAcousticImage: scatterer_port.coordinates.shape (before)       = "; fPrintTensorSize(scatterer_port.coordinates);
        std::cout<<"\t AUVClass::createAcousticImage: scatterer_starboard.coordinates.shape (before)  = "; fPrintTensorSize(scatterer_starboard.coordinates);

        // finding the pointing direction in spherical
        torch::Tensor auv_pointing_direction_spherical = fCart2Sph(this->pointing_direction);

        // asking the transmitters to subset the scatterers by multithreading
        std::thread transmitterFLSSubset_t(&AUVClass::subsetScatterers, this, \
                                           &scatterer_fls,\
                                           &this->transmitter_fls, \
                                           (float)0);
        std::thread transmitterPortSubset_t(&AUVClass::subsetScatterers, this, \
                                            &scatterer_port,\
                                            &this->transmitter_port, \
                                            auv_pointing_direction_spherical[1].item<float>());
        std::thread transmitterStarboardSubset_t(&AUVClass::subsetScatterers, this, \
                                                 &scatterer_starboard, \
                                                 &this->transmitter_starboard, \
                                                 - auv_pointing_direction_spherical[1].item<float>());

        // joining the subset threads back
        transmitterFLSSubset_t.join(        ); 
        transmitterPortSubset_t.join(       ); 
        transmitterStarboardSubset_t.join(  );


        // asking the ULAs to directly create acoustic images
        std::thread ULA_fls_acoustic_image_t(&ULAClass::nfdc_createAcousticImage, this->ULA_fls, \
                                             &scatterer_fls, &this->transmitter_fls);
        std::thread ULA_port_acoustic_image_t(&ULAClass::nfdc_createAcousticImage, &this->ULA_port, \
                                             &scatterer_port, &this->transmitter_port);
        std::thread ULA_starboard_acoustic_image_t(&ULAClass::nfdc_createAcousticImage, &this->ULA_starboard, \
                                                   &scatterer_starboard, &this->transmitter_starboard);

        // joining the threads back
        ULA_fls_acoustic_image_t.join(      );
        ULA_port_acoustic_image_t.join(     );
        ULA_starboard_acoustic_image_t.join();
        
    }


};






















// 0.0000, 
// 0.0000, 
// 0.0000, 
// 0.0000, 
// 0.0000, 
// 0.0000, 
// 0.0000, 
// 0.0000, 
// 0.0000, 
// 0.0000, 
// 0.0000, 
// 0.0000, 
// 0.0000, 
// 0.0000, 
// 0.0000, 
// 0.0000, 
// 0.0000, 
// 0.0000, 
// 0.0000, 
// 0.0000, 
// 0.0000, 
// 0.0000, 
// 0.0000, 
// 0.0000, 
// 0.0000, 
// 0.0000, 
// 0.0000, 
// 0.0000, 
// 0.0000, 
// 0.0000, 
// 0.0000, 
// 0.0001, 
// 0.0001, 
// 0.0002, 
// 0.0003, 
// 0.0006, 
// 0.0009, 
// 0.0014, 
// 0.0022, 0.0032, 0.0047, 0.0066, 0.0092, 0.0126, 0.0168, 0.0219, 0.0281, 0.0352, 0.0432, 0.0518, 0.0609, 0.0700, 0.0786, 0.0861, 0.0921, 0.0958, 0.0969, 0.0950, 0.0903, 0.0833, 0.0755, 0.0694, 0.0693, 0.0825, 0.1206