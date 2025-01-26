#include "TransmitterClass.h"
#include "ULAClass.h"
#include <iostream>
#include <ostream>
#include <torch/torch.h>

#pragma once

// hash defines
#define PRINTSPACE      std::cout<<"\n\n\n\n\n\n\n\n"<<std::endl;
#define PRINTSMALLLINE  std::cout<<"------------------------------------------------"<<std::endl;
#define PRINTLINE       std::cout<<"================================================"<<std::endl;
#define PI              3.14159265


class AUVClass{
public:
    // Intrinsic attributes
    torch::Tensor location;                 // location of vessel
    torch::Tensor velocity;                 // current speed of the vessel [a vector]
    torch::Tensor acceleration;             // current acceleration of vessel [a vector]
    
    ULAClass ULA_port;                 // mounted ULA [object of class, ULAClass]        
    ULAClass ULA_starboard;            // mounted ULA [object of class, ULAClass]        
    
    TransmitterClass transmitter_port;         // mounted transmitter [obj of class, TransmitterClass]
    TransmitterClass transmitter_starboard;    // mounted transmitter [obj of class, TransmitterClass]

    // derived or dependent attributes
    torch::Tensor signalMatrix_1;             // matrix containing the signals obtained from ULA_1
    torch::Tensor largeSignalMatrix_1;        // matrix holding signal of synthetic aperture
    torch::Tensor beamformedLargeSignalMatrix;// where each column is the beamformed signal at each stop-hop

    // plotting mode
    bool plottingmode;    // to suppress plotting associated with classes

    // spotlight mode related
    torch::Tensor absolute_coords_patch_cart; // cartesian coordinates of patch


    // Synthetic Aperture Related 
    torch::Tensor ApertureSensorLocations;    // sensor locations of aperture


    
    // operator overriding for printing 
    friend std::ostream& operator<<(std::ostream& os, AUVClass &auv){
        os<<"\t location = "<<torch::transpose(auv.location, 0, 1)<<std::endl;
        os<<"\t velocity = "<<torch::transpose(auv.velocity, 0, 1)<<std::endl;
        return os;
    }

};