// header-files
#include <iostream>
#include <ostream>

// Including classes
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/include/ScattererClass.h"

// Including functions
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Functions/fCart2Sph.cpp"
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Functions/fPrintTensorSize.cpp"
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Functions/fSph2Cart.cpp"

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


    /*==========================================================================
    Aim: Update pointing angle
    ----------------------------------------------------------------------------
    Note:
        > This function updates pointing angle based on AUV's pointing angle
        > for now, we're assuming no roll;
    --------------------------------------------------------------------------*/
    void updatePointingAngle(torch::Tensor AUV_pointing_vector){

        // calculate yaw and pitch
        if(DEBUGMODE_TRANSMITTER) std::cout<<"\t TransmitterClass: page 140 \n";
        torch::Tensor AUV_pointing_vector_spherical = fCart2Sph(AUV_pointing_vector);
        torch::Tensor yaw                           = AUV_pointing_vector_spherical[0];
        torch::Tensor pitch                         = AUV_pointing_vector_spherical[1];
        if(DEBUGMODE_TRANSMITTER) std::cout<<"\t TransmitterClass: page 144 \n";

        // std::cout<<"\t TransmitterClass: AUV_pointing_vector = "<<torch::transpose(AUV_pointing_vector, 0, 1)<<std::endl;
        // std::cout<<"\t TransmitterClass: AUV_pointing_vector_spherical = "<<torch::transpose(AUV_pointing_vector_spherical, 0, 1)<<std::endl;

        // calculating azimuth and elevation of transmitter object
        torch::Tensor absolute_azimuth_of_transmitter   = yaw + torch::tensor({this->azimuthal_angle}).to(torch::kFloat).to(DEVICE);
        torch::Tensor absolute_elevation_of_transmitter = pitch + torch::tensor({this->elevation_angle}).to(torch::kFloat).to(DEVICE);
        if(DEBUGMODE_TRANSMITTER) std::cout<<"\t TransmitterClass: page 149 \n";

        // std::cout<<"\t TransmitterClass: this->azimuthal_angle = "<<this->azimuthal_angle<<std::endl;
        // std::cout<<"\t TransmitterClass: this->elevation_angle = "<<this->elevation_angle<<std::endl;
        // std::cout<<"\t TransmitterClass: absolute_azimuth_of_transmitter = "<<absolute_azimuth_of_transmitter<<std::endl;
        // std::cout<<"\t TransmitterClass: absolute_elevation_of_transmitter = "<<absolute_elevation_of_transmitter<<std::endl;

        // converting back to Cartesian
        torch::Tensor pointing_direction_spherical  = torch::zeros({3,1}).to(torch::kFloat).to(DEVICE);
        pointing_direction_spherical[0]             = absolute_azimuth_of_transmitter;
        pointing_direction_spherical[1]             = absolute_elevation_of_transmitter;
        pointing_direction_spherical[2]             = torch::tensor({1}).to(torch::kFloat).to(DEVICE);
        if(DEBUGMODE_TRANSMITTER) std::cout<<"\t TransmitterClass: page 60 \n";

        this->pointing_direction = fSph2Cart(pointing_direction_spherical);
        if(DEBUGMODE_TRANSMITTER) std::cout<<"\t TransmitterClass: page 169 \n";
        
    }

    /*==========================================================================
    Aim: Subsetting Scatterers inside the cone
    ............................................................................
    steps:
        1. Find azimuth and range of all points. 
        2. Fint azimuth and range of current pointing vector. 
        3. Subtract azimuth and range of points from that of azimuth and range of current pointing vector
        4. Use tilted ellipse equation to find points in the ellipse
    --------------------------------------------------------------------------*/
    void subsetScatterers(ScattererClass* scatterers,
                          float tilt_angle){

        // translationally change origin 
        scatterers->coordinates = scatterers->coordinates - this->location;

        // Finding spherical coordinates of scatterers and pointing direction
        torch::Tensor scatterers_spherical          = fCart2Sph(scatterers->coordinates);
        torch::Tensor pointing_direction_spherical  = fCart2Sph(this->pointing_direction);

        // sending them to the right device
        scatterers_spherical            = scatterers_spherical.to(DEVICE);
        pointing_direction_spherical    = pointing_direction_spherical.to(DEVICE);

        // Calculating relative azimuths and radians
        torch::Tensor relative_spherical = scatterers_spherical - pointing_direction_spherical;

        // tensor corresponding to switch. 
        torch::Tensor tilt_angle_Tensor = torch::tensor({tilt_angle}).to(torch::kFloat).to(DEVICE);

        torch::Tensor axis_a    = torch::tensor({this->azimuthal_beamwidth / 2}).to(torch::kFloat).to(DEVICE);
        torch::Tensor axis_b    = torch::tensor({this->elevation_beamwidth / 2}).to(torch::kFloat).to(DEVICE);
        
        torch::Tensor xcosa     = relative_spherical[0] * torch::cos(tilt_angle_Tensor * PI/180);
        torch::Tensor ysina     = relative_spherical[1] * torch::sin(tilt_angle_Tensor * PI/180);
        torch::Tensor xsina     = relative_spherical[0] * torch::sin(tilt_angle_Tensor * PI/180);
        torch::Tensor ycosa     = relative_spherical[1] * torch::cos(tilt_angle_Tensor * PI/180);

        // findings points inside the cone
        // torch::Tensor scatter_boolean = torch::square(xcosa + ysina)/torch::square(axis_a) + \
        //                                 torch::square(xsina - ycosa)/torch::square(axis_b) <= 1;
        torch::Tensor scatter_boolean = torch::div(torch::square(xcosa + ysina), \
                                                   torch::square(axis_a)) + \
                                        torch::div(torch::square(xsina - ycosa), \
                                                   torch::square(axis_b))       <= 1;

        // subsetting points within the elliptical beam
        auto mask                   = (scatter_boolean == 1);    // creating a mask 
        scatterers->coordinates     = scatterers->coordinates.index({torch::indexing::Slice(),  mask});
        scatterers->reflectivity    = scatterers->reflectivity.index({torch::indexing::Slice(), mask});

        // this is where histogram shadowing comes in (later)


        // translating back to the points
        scatterers->coordinates = scatterers->coordinates + this->location;

    }

};