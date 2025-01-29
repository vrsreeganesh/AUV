#include "TransmitterClass.h"
#include "ULAClass.h"
#include <iostream>
#include <ostream>
#include <torch/torch.h>
#include <cmath>

#pragma once

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




// #define PRINTSPACE      std::cout<<"\n\n\n\n\n\n\n\n"<<std::endl;
// #define PRINTSMALLLINE  std::cout<<"------------------------------------------------"<<std::endl;
// #define PRINTLINE       std::cout<<"================================================"<<std::endl;
#define PI              3.14159265
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


    /*
    ============================================================================
    Aim: stepping motion
    ----------------------------------------------------------------------------
    */ 
    void step(float timestep){
        
        // updating location
        this->location = this->location + this->velocity * timestep;
        if(DEBUGMODE_AUV) std::cout<<"\t AUVClass: page 81 \n";
        
        // updating attributes of members
        this->syncComponentAttributes();
        if(DEBUGMODE_AUV) std::cout<<"\t AUVClass: page 85 \n";
    }

    /*
    ============================================================================
    Aim: updateAttributes
    ----------------------------------------------------------------------------
    */
    void syncComponentAttributes(){
        
        // updating sensor attributes
        
        if(DEBUGMODE_AUV) std::cout<<"\t AUVClass: page 97 \n";
        // updating transmitter locations
        this->transmitter_fls       = this->location;
        this->transmitter_port      = this->location;
        this->transmitter_starboard = this->location;
        if(DEBUGMODE_AUV) std::cout<<"\t AUVClass: page 102 \n";

        // updating transmitter pointing directions
        this->transmitter_fls.updatePointingAngle(          this->pointing_direction);
        this->transmitter_port.updatePointingAngle(         this->pointing_direction);
        this->transmitter_starboard.updatePointingAngle(    this->pointing_direction);
        if(DEBUGMODE_AUV) std::cout<<"\t AUVClass: page 108 \n";
    }


    /*==========================================================================
    Aim: Initialize AUV
    ............................................................................
    Note: 
        > Here, we ensure that all the components of the AUV share the same as that of AUVs. 
        > like same location, update AUV coordinates to that of the AUV and so on. 
    --------------------------------------------------------------------------*/ 
    void init(){
        // updating location of transmitteres
        this->transmitter_fls.location = this->location;
        this->transmitter_fls.location = this->location;
        this->transmitter_fls.location = this->location;


        // updating coordinates of ULAs


        // 
    }
    



    /*
    ============================================================================
    Aim: operator overriding for printing 
    ----------------------------------------------------------------------------
    */ 
    friend std::ostream& operator<<(std::ostream& os, AUVClass &auv){
        os<<"\t location = "<<torch::transpose(auv.location, 0, 1)<<std::endl;
        os<<"\t velocity = "<<torch::transpose(auv.velocity, 0, 1)<<std::endl;
        return os;
    }


    /*
    ============================================================================
    Aim: Changing Basis
    Note:
        - The subset-function in the transmitter class assumes the subsetting with the current basis. 
        - However, this is not ideal since we want the subsetting to be with respect to the AUV. 
        - So this function essentially changes the coordinates of the scatterers to that of the AUV's location and pointing direction. 
        - For now, we make the assumption that our AUV doesn't roll. That is, its belly is always to the sea-floor. 
        - we apply to the floor-scatterer coordinates, the very operations we need to apply to the pointing vector to make it point in the y-direction. 
        - not every operation works though cause the determinant should be one to ensure only rotations and no compressions take place. 
        - so we're gonna have to combine three transformations: yaw corrections and pitch correction. 
    ----------------------------------------------------------------------------
    */ 
    void subsetScatterers(ScattererClass* scatterers,\
                          TransmitterClass* transmitterObj,\
                          float tilt_angle){

        // updating location of the transmitter to that of AUV's current location
        transmitterObj->location = this->location;

        // calling the method associated with the transmitter
        transmitterObj->subsetScatterers(scatterers, tilt_angle);

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


};