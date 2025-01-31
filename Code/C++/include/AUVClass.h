#include "TransmitterClass.h"
#include "ULAClass.h"
#include <iostream>
#include <ostream>
#include <torch/torch.h>
#include <cmath>

#pragma once

// function to plot the thing
void fPlotTensors(){
    system("python /Users/vrsreeganesh/Documents/GitHub/AUV/Code/Python/TestingSaved_tensors.py");
}

// including class-definitions
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/include/ScattererClass.h"

// hash defines
#ifndef PRINTSPACE
#define PRINTSPACE      std::cout<<"\n\n\n\n\n\n\n\n"<<std::endl;
#endif
#ifndef PRINTSMALLLINE
#define PRINTSMALLLINE  std::cout<<"------------------------------------------------"<<std::endl;
#endif
#ifndef PRINTLINE
#define PRINTLINE       std::cout<<"================================================"<<std::endl;
#endif

#ifndef DEVICE
#define DEVICE          torch::kMPS
// #define DEVICE          torch::kCPU
#endif

#define PI              3.14159265
// #define DEBUGMODE_AUV   true
#define DEBUGMODE_AUV   false


class AUVClass{
public:
    // Intrinsic attributes
    torch::Tensor location;             // location of vessel
    torch::Tensor velocity;             // current speed of the vessel [a vector]
    torch::Tensor acceleration;         // current acceleration of vessel [a vector]
    torch::Tensor pointing_direction;   // direction to which the AUV is pointed
    
    // uniform linear-arrays
    ULAClass ULA_fls;                   // front-looking SONAR ULA
    ULAClass ULA_port;                  // mounted ULA [object of class, ULAClass]        
    ULAClass ULA_starboard;             // mounted ULA [object of class, ULAClass]        
    
    // transmitters
    TransmitterClass transmitter_fls;       // transmitter for front-looking SONAR
    TransmitterClass transmitter_port;      // mounted transmitter [obj of class, TransmitterClass]
    TransmitterClass transmitter_starboard; // mounted transmitter [obj of class, TransmitterClass]

    // derived or dependent attributes
    torch::Tensor signalMatrix_1;             // matrix containing the signals obtained from ULA_1
    torch::Tensor largeSignalMatrix_1;        // matrix holding signal of synthetic aperture
    torch::Tensor beamformedLargeSignalMatrix;// each column is the beamformed signal at each stop-hop

    // plotting mode
    bool plottingmode;    // to suppress plotting associated with classes

    // spotlight mode related
    torch::Tensor absolute_coords_patch_cart; // cartesian coordinates of patch

    // Synthetic Aperture Related 
    torch::Tensor ApertureSensorLocations;    // sensor locations of aperture


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
        if(DEBUGMODE_AUV) std::cout<<"\t AUVClass: line 99 \n";
        torch::Tensor ula_fls_sensor_direction_spherical    = fCart2Sph(this->pointing_direction);              // spherical coords
        if(DEBUGMODE_AUV) std::cout<<"\t AUVClass: line 101 \n";
        ula_fls_sensor_direction_spherical[0]               = ula_fls_sensor_direction_spherical[0] - 90;
        if(DEBUGMODE_AUV) std::cout<<"\t AUVClass: line 98 \n";
        torch::Tensor ula_fls_sensor_direction_cart         = fSph2Cart(ula_fls_sensor_direction_spherical);
        if(DEBUGMODE_AUV) std::cout<<"\t AUVClass: line 100 \n";

        this->ULA_fls.sensorDirection           = ula_fls_sensor_direction_cart;    // assigning sensor directionf or ULA-FLS
        this->ULA_port.sensorDirection          = -this->pointing_direction;        // assigning sensor direction for ULA-Port
        this->ULA_starboard.sensorDirection     = -this->pointing_direction;        // assigning sensor direction for ULA-Starboard
        if(DEBUGMODE_AUV) std::cout<<"\t AUVClass: line 105 \n";

        // // calling the function to update the arguments
        // this->ULA_fls.buildCoordinatesBasedOnLocation();        if(DEBUGMODE_AUV) std::cout<<"\t AUVClass: line 109 \n";
        // this->ULA_port.buildCoordinatesBasedOnLocation();       if(DEBUGMODE_AUV) std::cout<<"\t AUVClass: line 111 \n";
        // this->ULA_starboard.buildCoordinatesBasedOnLocation();  if(DEBUGMODE_AUV) std::cout<<"\t AUVClass: line 113 \n";
        
        // updating transmitter locations
        this->transmitter_fls.location       = this->location;
        this->transmitter_port.location      = this->location;
        this->transmitter_starboard.location = this->location;
        if(DEBUGMODE_AUV) std::cout<<"\t AUVClass: page 102 \n";

        // updating transmitter pointing directions
        this->transmitter_fls.updatePointingAngle(          this->pointing_direction);
        this->transmitter_port.updatePointingAngle(         this->pointing_direction);
        this->transmitter_starboard.updatePointingAngle(    this->pointing_direction);
        if(DEBUGMODE_AUV) std::cout<<"\t AUVClass: page 108 \n";
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


    // pitch-correction matrix
    torch::Tensor createYawCorrectionMatrix(torch::Tensor pointing_direction_spherical, \
                                            float target_azimuth_deg){

        // building parameters
        torch::Tensor azimuth_correction            = torch::tensor({target_azimuth_deg}).to(torch::kFloat).to(DEVICE) - \
                                                      pointing_direction_spherical[0];
        torch::Tensor azimuth_correction_radians    = azimuth_correction * PI / 180;

        torch::Tensor yawCorrectionMatrix = \
            torch::tensor({torch::cos(azimuth_correction_radians).item<float>(),      \
                           torch::cos(torch::tensor({90}).to(torch::kFloat).to(DEVICE)*PI/180 + azimuth_correction_radians).item<float>(), \
                           (float)0,                                                         \
                           torch::sin(azimuth_correction_radians).item<float>(),      \
                           torch::sin(torch::tensor({90}).to(torch::kFloat).to(DEVICE)*PI/180 + azimuth_correction_radians).item<float>(), \
                           (float)0,                                                         \
                           (float)0,                                                         \
                           (float)0,                                                         \
                           (float)1}).reshape({3,3}).to(torch::kFloat).to(DEVICE);

        // returning the matrix
        return yawCorrectionMatrix;
    }

    // pitch-correction matrix
    torch::Tensor createPitchCorrectionMatrix(torch::Tensor pointing_direction_spherical, \
                                              float target_elevation_deg){

        // building parameters
        torch::Tensor elevation_correction            = torch::tensor({target_elevation_deg}).to(torch::kFloat).to(DEVICE) - \
                                                        pointing_direction_spherical[1];
        torch::Tensor elevation_correction_radians    = elevation_correction * PI / 180;

        // creating the matrix
        torch::Tensor pitchCorrectionMatrix = \
            torch::tensor({(float)1,                                                           \
                           (float)0,                                                           \
                           (float)0,                                                           \
                           (float)0,                                                           \
                           torch::cos(elevation_correction_radians).item<float>(),      \
                           torch::cos(torch::tensor({90}).to(torch::kFloat).to(DEVICE)*PI/180 + elevation_correction_radians).item<float>(),\
                           (float)0,                                                           \
                           torch::sin(elevation_correction_radians).item<float>(),      \
                           torch::sin(torch::tensor({90}).to(torch::kFloat).to(DEVICE)*PI/180 + elevation_correction_radians).item<float>()}).reshape({3,3}).to(torch::kFloat);

        // returning the matrix
        return pitchCorrectionMatrix;
    }

    // Signal Simulation
    void simulateSignal(ScattererClass& scatterer){

        // making three copies
        ScattererClass scatterer_fls        = scatterer;
        ScattererClass scatterer_port       = scatterer;
        ScattererClass scatterer_starboard  = scatterer;

        // printing the size of these points before subsetting
        std::cout<<"scatterer_fls.coordinates.shape (before)        = "; fPrintTensorSize(scatterer_fls.coordinates);
        std::cout<<"scatterer_port.coordinates.shape (before)       = "; fPrintTensorSize(scatterer_port.coordinates);
        std::cout<<"scatterer_starboard.coordinates.shape (before)  = "; fPrintTensorSize(scatterer_starboard.coordinates);

        // // asking the transmitters to subset the points in their vicinity
        // this->transmitter_fls.subsetScatterers(         scatterer_fls);
        // this->transmitter_port.subsetScatterers(        scatterer_port);
        // this->transmitter_starboard.subsetScatterers(   scatterer_starboard);

        // finding the pointing direction in spherical
        torch::Tensor auv_pointing_direction_spherical = fCart2Sph(this->pointing_direction);

        // asking the transmitters to subset by multithreading
        // std::thread transmitterFLSSubset_t(&AUVClass::subsetScatterers, &this, \
        //                                    &scatterer_fls,\
        //                                    &this->transmitter_fls, \
        //                                    (float)0);
        // std::thread transmitterPortSubset_t(&AUVClass::subsetScatterers, &this, \
        //                                     &scatterer_port,\
        //                                     &this->transmitter_port, \
        //                                     - auv_pointing_direction_spherical[1].item<float>());
        // std::thread transmitterStarboardSubset_t(&AUVClass::subsetScatterers, &this, \
        //                                          &scatterer_starboard, \
        //                                          &this->transmitter_starboard, \
        //                                          auv_pointing_direction_spherical[1].item<float>());

        std::thread transmitterFLSSubset_t(&AUVClass::subsetScatterers, this, \
                                           &scatterer_fls,\
                                           &this->transmitter_fls, \
                                           (float)0);
        std::thread transmitterPortSubset_t(&AUVClass::subsetScatterers, this, \
                                            &scatterer_port,\
                                            &this->transmitter_port, \
                                            - auv_pointing_direction_spherical[1].item<float>());
        std::thread transmitterStarboardSubset_t(&AUVClass::subsetScatterers, this, \
                                                 &scatterer_starboard, \
                                                 &this->transmitter_starboard, \
                                                 auv_pointing_direction_spherical[1].item<float>());

        // joining the subset threads back
        transmitterFLSSubset_t.join();
        transmitterPortSubset_t.join();
        transmitterStarboardSubset_t.join();


        // printing the size of these points before subsetting
        PRINTDOTS
        std::cout<<"scatterer_fls.coordinates.shape (after)        = "; fPrintTensorSize(scatterer_fls.coordinates);
        std::cout<<"scatterer_port.coordinates.shape (after)       = "; fPrintTensorSize(scatterer_port.coordinates);
        std::cout<<"scatterer_starboard.coordinates.shape (after)  = "; fPrintTensorSize(scatterer_starboard.coordinates);

        // saving the tensors
        if (true) {
            // saving the ground-truth
            ScattererClass SeafloorScatter_gt = scatterer;
            torch::save(SeafloorScatter_gt.coordinates, \
                        "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_gt.pt");
            torch::save(SeafloorScatter_gt.reflectivity, \
                        "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_gt_reflectivity.pt");
            

            // saving coordinates 
            torch::save(scatterer_fls.coordinates, \
                    "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_fls_coordinates.pt");
            torch::save(scatterer_port.coordinates, \
                        "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_port_coordinates.pt");
            torch::save(scatterer_starboard.coordinates, \
                        "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_starboard.coordinates.pt");

            // saving reflectivities
            torch::save(scatterer_fls.reflectivity, \
                    "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_fls_coordinates_reflectivity.pt");
            torch::save(scatterer_port.reflectivity, \
                        "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_port_coordinates_reflectivity.pt");
            torch::save(scatterer_starboard.reflectivity, \
                        "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/SeafloorScatter_starboard.coordinates_reflectivity.pt");

            // plotting tensors
            fPlotTensors();
        }

    }


};