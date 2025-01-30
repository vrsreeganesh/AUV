#include <iostream>
#include <torch/torch.h>

#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Functions/fPrintTensorSize.cpp"

#pragma once

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

#define DEBUG_ULA   true


class ULAClass{
public:
    // intrinsic parameters
    int num_sensors;                    // number of sensors
    float inter_element_spacing;        // space between sensors
    torch::Tensor coordinates;          // coordinates of each sensor
    float sampling_frequency;           // sampling frequency of the sensors
    float recording_period;             // recording period of the ULA
    torch::Tensor location;             // location of first coordinate

    // derived stuff
    torch::Tensor sensorDirection; 
    torch::Tensor signalMatrix;
    
    // constructor
    ULAClass(int numsensors              = 32,
             float inter_element_spacing = 1e-3,
             torch::Tensor coordinates   = torch::zeros({3, 2}),
             float sampling_frequency    = 48e3,
             float recording_period      = 1):
             num_sensors(numsensors),
             inter_element_spacing(inter_element_spacing),
             coordinates(coordinates),
             sampling_frequency(sampling_frequency),
             recording_period(recording_period) {
                // calculating ULA direction
                torch::Tensor sensorDirection = coordinates.slice(1, 0, 1) - coordinates.slice(1, 1, 2);

                // normalizing 
                float normvalue = torch::linalg_norm(sensorDirection, 2, 0, true, torch::kFloat).item<float>();
                if (normvalue != 0){
                    sensorDirection = sensorDirection / normvalue;
                }
                
                // copying direction 
                this->sensorDirection = sensorDirection;
        }

    // overrinding printing
    friend std::ostream& operator<<(std::ostream& os, ULAClass& ula){
        os<<"\t number of sensors    : "<<ula.num_sensors           <<std::endl;
        os<<"\t inter-element spacing: "<<ula.inter_element_spacing <<std::endl;
        os<<"\t sensor-direction "      <<torch::transpose(ula.sensorDirection, 0, 1)<<std::endl;
        PRINTSMALLLINE
        return os;
    }

    // overloading the "=" operator
    ULAClass& operator=(const ULAClass& other){
        // checking if copying to the same object
        if(this == &other){
            return *this;
        }

        // copying everything
        this->num_sensors           = other.num_sensors;
        this->inter_element_spacing = other.inter_element_spacing;
        this->coordinates           = other.coordinates.clone();
        this->sampling_frequency    = other.sampling_frequency;
        this->recording_period      = other.recording_period;
        this->sensorDirection       = other.sensorDirection.clone(); 

        // returning
        return *this;
    }

    /* =========================================================================
    Aim: Build coordinates on top of location. 
    ............................................................................
    Note: 
        > This function builds the location of the coordinates based on the location and direction member. 
    --------------------------------------------------------------------------*/ 
    void buildCoordinatesBasedOnLocation(){

        // length-normalize the sensor-direction
        this->sensorDirection = torch::div(this->sensorDirection, torch::linalg_norm(this->sensorDirection, \
                                                                           2, 0, true, \
                                                                           torch::kFloat));
        if(DEBUG_ULA) std::cout<<"\t ULAClass: line 105 \n";

        // multiply with inter-element distance
        this->sensorDirection = this->sensorDirection * this->inter_element_spacing;
        this->sensorDirection = this->sensorDirection.reshape({this->sensorDirection.numel(), 1});
        if(DEBUG_ULA) std::cout<<"\t ULAClass: line 110 \n";

        // create integer-array
        // torch::Tensor integer_array = torch::linspace(0, \
        //                                               this->num_sensors-1, \
        //                                               this->num_sensors).reshape({1, this->num_sensors}).to(torch::kFloat);
        torch::Tensor integer_array = torch::linspace(0, \
                                                      this->num_sensors-1, \
                                                      this->num_sensors).reshape({1, this->num_sensors});
        std::cout<<"integer_array = "; fPrintTensorSize(integer_array);
        if(DEBUG_ULA) std::cout<<"\t ULAClass: line 116 \n";    


        // this->coordinates = torch::mul(torch::tile(integer_array, {3, 1}).to(torch::kFloat), \
        //                                torch::tile(this->sensorDirection, {1, this->num_sensors}).to(torch::kFloat));
        torch::Tensor test = torch::mul(torch::tile(integer_array, {3, 1}).to(torch::kFloat), \
                                       torch::tile(this->sensorDirection, {1, this->num_sensors}).to(torch::kFloat));
        this->coordinates = this->location + test; 
        if(DEBUG_ULA) std::cout<<"\t ULAClass: line 120 \n";
    
    }
};