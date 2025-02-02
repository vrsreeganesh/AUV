#include <cstdint>
#include <iostream>
#include <torch/torch.h>

#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Functions/fPrintTensorSize.cpp"
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Functions/fConvolveColumns.cpp"
#include "ScattererClass.h"
#include "TransmitterClass.h"

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

// #define DEBUG_ULA   true
#define DEBUG_ULA   false



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

    // decimation-related
    torch::Tensor lowpassFilterCoefficientsForDecimation;   // 
    
    // constructor
    ULAClass(int numsensors              = 32,
             float inter_element_spacing = 1e-3,
             torch::Tensor coordinates   = torch::zeros({3, 2}),
             float sampling_frequency    = 48e3,
             float recording_period      = 1,
             torch::Tensor location         = torch::zeros({3,1}),
             torch::Tensor signalMatrix     = torch::zeros({1, 32}), 
             torch::Tensor lowpassFilterCoefficientsForDecimation = torch::zeros({1,10})):
             num_sensors(numsensors),
             inter_element_spacing(inter_element_spacing),
             coordinates(coordinates),
             sampling_frequency(sampling_frequency),
             recording_period(recording_period),
             location(location),
             signalMatrix(signalMatrix),
             lowpassFilterCoefficientsForDecimation(lowpassFilterCoefficientsForDecimation){
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
        
        // new additions
        // this->location              = other.location;
        this->lowpassFilterCoefficientsForDecimation = other.lowpassFilterCoefficientsForDecimation;
        // this->sensorDirection       = other.sensorDirection.clone();
        // this->signalMatrix          = other.signalMatrix.clone();


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

    // Signal Simulation
    void nfdc_simulateSignal(ScattererClass* scatterers,
                             TransmitterClass* transmitterObj){

        // creating signal matrix
        int numsamples      = std::ceil((this->sampling_frequency * this->recording_period));
        this->signalMatrix  = torch::zeros({numsamples, this->num_sensors}).to(torch::kFloat);

        // getting shape of coordinates
        std::vector<int64_t> scatterers_coordinates_shape = scatterers->coordinates.sizes().vec();
        
        // making a slot out of the coordinates
        torch::Tensor slottedCoordinates = \
            torch::permute(scatterers->coordinates.reshape({scatterers_coordinates_shape[0],    \
                                                            scatterers_coordinates_shape[1],    \
                                                            1                                   }), \
                           {2, 1, 0}).reshape({1, (int)(scatterers->coordinates.numel()/3), 3});
        
        // repeating along the y-direction number of sensor times. 
        slottedCoordinates = torch::tile(slottedCoordinates, {this->num_sensors, 1, 1});
        std::vector<int64_t> slottedCoordinates_shape = slottedCoordinates.sizes().vec();
        
        // finding the shape of the sensor-coordinates
        std::vector<int64_t> sensor_coordinates_shape = this->coordinates.sizes().vec();

        // creating a slot tensor out of the sensor-coordinates
        torch::Tensor slottedSensors = \
            torch::permute(this->coordinates.reshape({sensor_coordinates_shape[0], \
                                                      sensor_coordinates_shape[1], \
                                                      1}), {2, 1, 0}).reshape({(int)(this->coordinates.numel()/3), \
                                                                               1, \
                                                                               3});
        
        // repeating slices along the y-coordinates
        slottedSensors  = torch::tile(slottedSensors, {1, slottedCoordinates_shape[1], 1});

        // slotting the coordinate of the transmitter
        torch::Tensor slotted_location = torch::permute(this->location.reshape({3, 1, 1}), \
                                                        {2, 1, 0}).reshape({1,1,3});
        slotted_location = torch::tile(slotted_location, \
                                       {slottedCoordinates_shape[0], slottedCoordinates_shape[1], 1});

        // subtracting to find the relative distances
        torch::Tensor distBetweenScatterersAndSensors = \
            torch::linalg_norm(slottedCoordinates - slottedSensors,     2, 2, true, torch::kFloat);

        // substracting distance between relative fields
        torch::Tensor distBetweenScatterersAndTransmitter = \
            torch::linalg_norm(slottedCoordinates - slotted_location,   2, 2, true, torch::kFloat);

        // adding up the distances
        torch::Tensor distOfFlight      = distBetweenScatterersAndSensors + distBetweenScatterersAndTransmitter;
        torch::Tensor timeOfFlight      = distOfFlight/1500;
        torch::Tensor samplesOfFlight   = torch::floor(timeOfFlight.squeeze() * this->sampling_frequency);

        // Adding pulses 
        for(int sensor_index = 0; sensor_index < this->num_sensors; ++sensor_index){
            for(int scatter_index = 0; scatter_index < samplesOfFlight[0].numel(); ++scatter_index){
                
                // getting the sample where the current scatter's contribution must be placed. 
                int where_to_place = samplesOfFlight.index({sensor_index, scatter_index}).item<int>();

                // checking whether that point is out of bounds
                if(where_to_place >= numsamples) continue;

                // placing a point in there
                this->signalMatrix.index_put_({where_to_place, sensor_index}, \
                                              this->signalMatrix.index({where_to_place, sensor_index}) + \
                                              torch::tensor({1}).to(torch::kFloat));

                this->signalMatrix.index_put_({where_to_place, sensor_index}, \
                                              this->signalMatrix.index({where_to_place, sensor_index}) + \
                                                scatterers->reflectivity.index({0, scatter_index}) );
            }
        }

        // Convolving signals with transmitted signal
        torch::Tensor signalTensorAsArgument = \
            transmitterObj->Signal.reshape({transmitterObj->Signal.numel(),1});
        signalTensorAsArgument = torch::tile(signalTensorAsArgument, \
                                             {1, this->signalMatrix.size(1)});

        // convolving the pulse-matrix with the signal matrix
        fConvolveColumns(this->signalMatrix,        \
                         signalTensorAsArgument);

        // trimming the convolved signal since the signal matrix length remains the same
        this->signalMatrix = this->signalMatrix.index({torch::indexing::Slice(0, numsamples), \
                                                       torch::indexing::Slice()});

        // printing the shape
        std::cout<<"\t\t\t> this->signalMatrix.shape (after signal sim) = "; fPrintTensorSize(this->signalMatrix);

        return;
    }

    // decimating the obtained signal
    void nfdc_decimateSignal(TransmitterClass* transmitterObj){
        
        if(DEBUG_ULA) std::cout<<"\t\t\t ULAClass::nfdc_decimateSignal: entered "<<std::endl;

        // basebanding the signal
        torch::Tensor integerArray  = torch::linspace(0, \
                                                      this->signalMatrix.size(0)-1, \
                                                      this->signalMatrix.size(0)).reshape({this->signalMatrix.size(0), 1}); if(DEBUG_ULA) std::cout<<"\t\t\t ULAClass::nfdc_decimateSignal: 247"<<std::endl;
        if(DEBUG_ULA) std::cout<<"integerArray.shape = "<<integerArray.sizes().vec()<<std::endl;
        integerArray                = torch::tile(integerArray, \
                                                  {1, this->num_sensors}); if(DEBUG_ULA) std::cout<<"\t\t\t ULAClass::nfdc_decimateSignal: 249"<<std::endl;
        if(DEBUG_ULA) std::cout<<"integerArray.shape = "<<integerArray.sizes().vec()<<std::endl;
        integerArray                = torch::exp(torch::complex(torch::zeros({1}), torch::ones({1})) * \
                                                 transmitterObj->fc * \
                                                 integerArray); if(DEBUG_ULA) std::cout<<"\t\t\t ULAClass::nfdc_decimateSignal: 250"<<std::endl;

        if(DEBUG_ULA) std::cout<<"this->signalMatrix.shape = "<<this->signalMatrix.sizes().vec()<<std::endl;
        if(DEBUG_ULA) std::cout<<"integerArray.shape = "<<integerArray.sizes().vec()<<std::endl;
        this->signalMatrix = torch::mul(this->signalMatrix, \
                                        integerArray); std::cout<<"\t\t\t ULAClass::nfdc_decimateSignal: 254"<<std::endl;

        // low-pass filter
        torch::Tensor lowpassfilter_impulseresponse = \
            this->lowpassFilterCoefficientsForDecimation.reshape({this->lowpassFilterCoefficientsForDecimation.numel(), 1}); if(DEBUG_ULA) std::cout<<"\t\t\t ULAClass::nfdc_decimateSignal: 263"<<std::endl;
        lowpassfilter_impulseresponse = torch::tile(lowpassfilter_impulseresponse, \
                                      {1, this->signalMatrix.size(1)}); if(DEBUG_ULA) std::cout<<"\t\t\t ULAClass::nfdc_decimateSignal: 260"<<std::endl;

        // Convolving
        fConvolveColumns(this->signalMatrix, lowpassfilter_impulseresponse); if(DEBUG_ULA) std::cout<<"\t\t\t ULAClass::nfdc_decimateSignal: 263"<<std::endl;

        // downsampling
        int decimation_factor = std::floor(this->sampling_frequency/transmitterObj->bandwidth); if(DEBUG_ULA) std::cout<<"\t\t\t ULAClass::nfdc_decimateSignal: 284"<<std::endl;
        if(DEBUG_ULA) std::cout<<"\t\t\t\t\t\t decimation_factor = "<<decimation_factor<<std::endl;
        int numsamples_after_decimation = std::floor(this->signalMatrix.size(0)/decimation_factor); if(DEBUG_ULA) std::cout<<"\t\t\t ULAClass::nfdc_decimateSignal: 285"<<std::endl;
        torch::Tensor samplingIndices = \
            torch::linspace(0, \
                            numsamples_after_decimation * decimation_factor, \
                            numsamples_after_decimation).to(torch::kInt); if(DEBUG_ULA) std::cout<<"\t\t\t ULAClass::nfdc_decimateSignal: 289"<<std::endl;
        
        
        this->signalMatrix = \
            this->signalMatrix.index({samplingIndices, torch::indexing::Slice()}); if(DEBUG_ULA) std::cout<<"\t\t\t ULAClass::nfdc_decimateSignal: 273"<<std::endl;

        if(DEBUG_ULA) std::cout<<"this->signalMatrix.shape = "<<this->signalMatrix.sizes().vec()<<std::endl;

    }
};