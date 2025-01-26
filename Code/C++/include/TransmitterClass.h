#include <iostream>
#include <ostream>
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/include/ScattererClass.h"

#pragma once

// hash defines
#define PRINTSPACE      std::cout<<"\n\n\n\n\n\n\n\n"<<std::endl;
#define PRINTSMALLLINE  std::cout<<"------------------------------------------------"<<std::endl;
#define PRINTLINE       std::cout<<"================================================"<<std::endl;
#define PI              3.14159265


class TransmitterClass{
public:
    // basic parameters
    torch::Tensor location;     // location tensor 
    torch::Tensor Signal;       // transmitted signal (LFM)
    float azimuthal_angle;      // transmitter's azimuthal pointing direction
    float elevation_angle;      // transmitter's elevation pointing direction
    float azimuthal_beamwidth;  // azimuthal beamwidth of transmitter
    float elevation_beamwidth;  // elevation beamwidth of transmitter
    float range;                // a parameter used for spotlight mode. 

    // transmitted signal attributes
    float f_low;                // lowest frequency of LFM
    float f_high;               // highest frequency of LFM
    float fc;                   // center frequency of LFM
    float bandwidth;            // bandwidth of LFM

    // shadowing properties
    int azimuthQuantDensity;            // quantization of angles along the azimuth
    int elevationQuantDensity;          // quantization of angles along the elevation
    float rangeQuantSize;               // range-cell size when shadowing
    float azimuthShadowThreshold;       // azimuth thresholding 
    float elevationShadowThreshold;     // elevation thresholding
    torch::Tensor checkbox;             // box indicating whether a scatter for a range-angle pair has been found
    torch::Tensor finalScatterBox;      // a 3D tensor where the third dimension represnets the vector length
    torch::Tensor finalReflectivityBox; // to store the reflectivity



    // Constructor
    TransmitterClass(torch::Tensor location     = torch::zeros({3,1}), 
                     torch::Tensor Signal       = torch::zeros({10,1}), 
                     float azimuthal_angle      = 0, 
                     float elevation_angle      = -30, 
                     float azimuthal_beamwidth  = 30, 
                     float elevation_beamwidth  = 30):
                     location(location), 
                     Signal(Signal), 
                     azimuthal_angle(azimuthal_angle), 
                     elevation_angle(elevation_angle), 
                     azimuthal_beamwidth(azimuthal_beamwidth), 
                     elevation_beamwidth(elevation_beamwidth) {}

    // overloading output 
    friend std::ostream& operator<<(std::ostream& os, TransmitterClass& transmitter){
        os<<"\t> azimuth            : "<<transmitter.azimuthal_angle    <<std::endl;
        os<<"\t> elevation          : "<<transmitter.elevation_angle    <<std::endl;
        os<<"\t> azimuthal beamwidth: "<<transmitter.azimuthal_beamwidth<<std::endl;
        os<<"\t> elevation beamwidth: "<<transmitter.elevation_beamwidth<<std::endl;
        PRINTSMALLLINE
        return os;
    }

    // overloading copyign operator
    TransmitterClass& operator=(const TransmitterClass& other){

        // checking self-assignment
        if(this==&other){
            return *this;
        }

        // allocating memory
        this->location              = other.location;
        this->Signal                = other.Signal;
        this->azimuthal_angle       = other.azimuthal_angle;
        this->elevation_angle       = other.elevation_angle;
        this->azimuthal_beamwidth   = other.azimuthal_beamwidth;
        this->elevation_beamwidth   = other.elevation_beamwidth;
        this->range                 = other.range;

        // transmitted signal attributes
        this->f_low                 = other.f_low;
        this->f_high                = other.f_high;
        this->fc                    = other.fc;
        this->bandwidth             = other.bandwidth;

        // shadowing properties
        this->azimuthQuantDensity       = other.azimuthQuantDensity;
        this->elevationQuantDensity     = other.elevationQuantDensity;
        this->rangeQuantSize            = other.rangeQuantSize;
        this->azimuthShadowThreshold    = other.azimuthShadowThreshold;
        this->elevationShadowThreshold  = other.elevationShadowThreshold;
        this->checkbox                  = other.checkbox;
        this->finalScatterBox           = other.finalScatterBox;
        this->finalReflectivityBox      = other.finalReflectivityBox;

        // returning 
        return *this;

    };

    // subsetting scatterers
    void subsetScatters(ScattererClass& scatters){

        // changing coordinates of scatterers to that of transmitter
        scatters.coordinates = scatters.coordinates - this->location;
    }

};