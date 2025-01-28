// header-files
#include <iostream>
#include <ostream>

// Including classes
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/include/ScattererClass.h"

// Including functions
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Functions/fCart2Sph.cpp"
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Functions/fPrintTensorSize.cpp"

#pragma once

// hash defines
#ifndef PRINTSPACE
#   define PRINTSPACE      std::cout<<"\n\n\n\n\n\n\n\n"<<std::endl;
#endif
#ifndef PRINTSMALLLINE
#   define PRINTSMALLLINE  std::cout<<"------------------------------------------------"<<std::endl;
#endif
#ifndef PRINTLINE
#   define PRINTLINE       std::cout<<"================================================"<<std::endl;
#endif

#define PI              3.14159265
#define DEBUGMODE_TRANSMITTER       false

#ifndef DEVICE
    #define DEVICE          torch::kMPS
    // #define DEVICE          torch::kCPU
#endif


class TransmitterClass{
public:

    // physical/intrinsic properties
    torch::Tensor location;             // location tensor 
    torch::Tensor pointing_direction;   // pointing direction

    // basic parameters
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
    void subsetScatterers(ScattererClass* scatterers){

        
        if (DEBUGMODE_TRANSMITTER) {
            std::cout<<"scatterers->coordinates.shape           = "; 
            fPrintTensorSize(scatterers->coordinates);
        }


        // converting from cartesian tensors to spherical tensors
        torch::Tensor scatterers_spherical = fCart2Sph(scatterers->coordinates);
        if (DEBUGMODE_TRANSMITTER) std::cout<<"\t TransmitterClass::subsetScatterers 140 \n";
        scatterers_spherical = scatterers_spherical.to(DEVICE);
        if (DEBUGMODE_TRANSMITTER){ 
            std::cout<<"scatterers_spherical.shape              = ";
            fPrintTensorSize(scatterers_spherical); PRINTSMALLLINE
        }

        // printing some status
        if (DEBUGMODE_TRANSMITTER){
            PRINTSPACE
            PRINTLINE
            std::cout<<"\t TransmitterClass > this->azimuthal_angle = "     <<this->azimuthal_angle     <<std::endl;
            std::cout<<"\t TransmitterClass > this->elevation_angle = "     <<this->elevation_angle     <<std::endl;
            std::cout<<"\t TransmitterClass > this->azimuthal_beamwidth = " <<this->azimuthal_beamwidth <<std::endl;
            std::cout<<"\t TransmitterClass > this->elevation_beamwidth = " <<this->elevation_beamwidth <<std::endl;
            PRINTLINE
            PRINTSPACE
        }



        if (DEBUGMODE_TRANSMITTER) std::cout<<"\t TransmitterClass >reached line 136 \n";

        // finding points that are in the cone
        torch::Tensor scatter_boolean = \
            (torch::square((scatterers_spherical[0] - \
                            torch::tensor({this->azimuthal_angle}).to(torch::kFloat).to(DEVICE)))/torch::square(torch::tensor({this->azimuthal_beamwidth/2}).to(DEVICE))
             + \
             torch::square((scatterers_spherical[1] - \
                            torch::tensor({this->elevation_angle}).to(torch::kFloat).to(DEVICE)))/torch::square(torch::tensor({this->elevation_beamwidth/2}).to(DEVICE)) \
             < 1);

        if (DEBUGMODE_TRANSMITTER) std::cout<<"\t TransmitterClass >reached line 141 \n";
        if (DEBUGMODE_TRANSMITTER){ 
            std::cout<<"scatter_boolean.shape                   = "; 
            fPrintTensorSize(scatter_boolean); 
            PRINTSMALLLINE;
        }

        // subsetting points within the elliptical beam
        auto mask = (scatter_boolean == 1);    // creating a mask 
        if (DEBUGMODE_TRANSMITTER) std::cout<<"\t TransmitterClass >reached line 146 \n";
        scatterers->coordinates = scatterers->coordinates.index({torch::indexing::Slice(), mask});
        if (DEBUGMODE_TRANSMITTER) {
            std::cout<<"scatterers->coordinates.shape                   = "; 
            fPrintTensorSize(scatterers->coordinates); PRINTSMALLLINE;
        }
        if (DEBUGMODE_TRANSMITTER) std::cout<<"\t TransmitterClass >reached line 148 \n";
        scatterers->reflectivity = scatterers->reflectivity.index({torch::indexing::Slice(), mask});
        if (DEBUGMODE_TRANSMITTER) std::cout<<"\t TransmitterClass >reached line 150 \n";

        // this is where histogram shadowing comes in (later)


        if (DEBUGMODE_TRANSMITTER) std::cout<<"\t TransmitterClass > reached line 156 \n";

    }

};