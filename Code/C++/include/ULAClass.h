// bringing in the header files
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <torch/torch.h>


// bringing in the functions
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Functions/fPrintTensorSize.cpp"
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Functions/fConvolveColumns.cpp"
#include "ScattererClass.h"
#include "TransmitterClass.h"
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Functions/fBuffer2D.cpp"
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Functions/fConvolve1D.cpp"

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
    // #define DEVICE          torch::kMPS
    #define DEVICE          torch::kCPU
#endif

#define PI              3.14159265
#define COMPLEX_1j              torch::complex(torch::zeros({1}), torch::ones({1}))

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
    int decimation_factor;
    torch::Tensor lowpassFilterCoefficientsForDecimation;   // 

    // imaging related 
    float range_resolution;             // theoretical range-resolution = $\frac{c}{2B}$
    float azimuthal_resolution;         // theoretical azimuth-resolution = $\frac{\lambda}{(N-1)*inter-element-distance}$
    float range_cell_size;              // the range-cell quanta we're choosing for efficiency trade-off
    float azimuth_cell_size;            // the azimuth quanta we're choosing
    torch::Tensor mulFFTMatrix;         // the matrix containing the delays for each-element as a slot
    torch::Tensor azimuth_centers;      // tensor containing the azimuth centeres
    torch::Tensor range_centers;        // tensor containing the range-centers 
    int frame_size;                     // the frame-size corresponding to a range cell in a decimated signal matrix
    torch::Tensor matchFilter;          // torch tensor containing the match-filter
    
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

    /* =========================================================================
    Aim: Init 
    ------------------------------------------------------------------------- */ 
    void init(TransmitterClass* transmitterObj){
        
        // calculating range-related parameters
        this->range_resolution      = 1500/(2 * transmitterObj->fc);
        this->range_cell_size       = 40 * this->range_resolution;

        // status printing
        if(DEBUG_ULA) std::cout<<"\t\t ULAClass::init():: this->range_resolution = "  <<  this->range_resolution  <<  std::endl;
        if(DEBUG_ULA) std::cout<<"\t\t ULAClass::init():: this->range_cell_size  = "  <<  this->range_cell_size   <<  std::endl;

        // calculating azimuth-related parameters
        this->azimuthal_resolution      = (1500/transmitterObj->fc)/((this->num_sensors-1)*this->inter_element_spacing); 
        this->azimuth_cell_size         = 2 * this->azimuthal_resolution;

        // creating and storing the match-filter
        this->nfdc_CreateMatchFilter(transmitterObj);
    }

    // Create match-filter
    void nfdc_CreateMatchFilter(TransmitterClass* transmitterObj){

        // creating matrix for basebanding the signal
        torch::Tensor basebanding_vector = \
            torch::linspace(0, \
                            transmitterObj->Signal.numel() - 1, \
                            transmitterObj->Signal.numel()).reshape(transmitterObj->Signal.sizes());
        basebanding_vector = \
            torch::exp(COMPLEX_1j * 2 * PI * transmitterObj->fc * basebanding_vector);

        // multiplying the signal with the basebanding vector
        torch::Tensor match_filter = \
            torch::mul(transmitterObj->Signal, basebanding_vector);

        // low-pass filtering
        fConvolve1D(match_filter, this->lowpassFilterCoefficientsForDecimation);

        // creating sampling-indices
        int decimation_factor = std::floor((static_cast<float>(this->sampling_frequency)/2) /        \
                                            (static_cast<float>(transmitterObj->bandwidth)/2));
        int final_num_samples = std::ceil(static_cast<float>(match_filter.numel())/static_cast<float>(decimation_factor));
        torch::Tensor sampling_indices = \
            torch::linspace(1, \
                            (final_num_samples-1) * decimation_factor, 
                            final_num_samples).to(torch::kInt) - torch::tensor({1}).to(torch::kInt);
        
        // sampling the signal
        match_filter = match_filter.index({sampling_indices});
        
        // taking conjugate and flipping the signal
        match_filter = torch::flipud(   match_filter);
        match_filter = torch::conj(     match_filter);

        // storing the match-filter to the class member
        this->matchFilter = match_filter;
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

    // build sensor-coordinates based on location
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

    // signal simulation for the current sensor-array
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
        if(DEBUG_ULA) {
            std::cout<<"\t\t\t> this->signalMatrix.shape (after signal sim) = "; 
            fPrintTensorSize(this->signalMatrix);
        }

        return;
    }

    // decimating the obtained signal
    void nfdc_decimateSignal(TransmitterClass* transmitterObj){
        
        // creating the matrix for frequency-shifting
        torch::Tensor integerArray  = torch::linspace(0, \
                                                      this->signalMatrix.size(0)-1, \
                                                      this->signalMatrix.size(0)).reshape({this->signalMatrix.size(0), 1}); 
        integerArray                = torch::tile(integerArray, {1, this->num_sensors});
        integerArray                = torch::exp(COMPLEX_1j * transmitterObj->fc * integerArray);

        // storing original number of samples
        int original_signalMatrix_numsamples = this->signalMatrix.size(0);

        // printing
        std::cout << "this->signalMatrix.shape    = "<< this->signalMatrix.sizes().vec()   << std::endl;
        std::cout << "integerArray.shape          = "<< integerArray.sizes().vec()         << std::endl;

        // producing frequency-shifting
        this->signalMatrix          = torch::mul(this->signalMatrix, integerArray);

        // low-pass filter
        torch::Tensor lowpassfilter_impulseresponse = \
            this->lowpassFilterCoefficientsForDecimation.reshape({this->lowpassFilterCoefficientsForDecimation.numel(), 1});
        lowpassfilter_impulseresponse = torch::tile(lowpassfilter_impulseresponse, \
                                      {1, this->signalMatrix.size(1)}); 

        // Convolving
        fConvolveColumns(this->signalMatrix, lowpassfilter_impulseresponse); 

        // Cutting down the extra-samples from convolution
        this->signalMatrix = \
            this->signalMatrix.index({torch::indexing::Slice(0, original_signalMatrix_numsamples), \
                                      torch::indexing::Slice()});

        // building parameters for downsampling
        int decimation_factor           = std::floor(this->sampling_frequency/transmitterObj->bandwidth);
        this->decimation_factor         = decimation_factor;
        int numsamples_after_decimation = std::floor(this->signalMatrix.size(0)/decimation_factor);

        // building the samples which will be subsetted
        torch::Tensor samplingIndices = \
            torch::linspace(0, \
                            numsamples_after_decimation * decimation_factor - 1, \
                            numsamples_after_decimation).to(torch::kInt);
        
        // downsampling the low-pass filtered signal
        this->signalMatrix = \
            this->signalMatrix.index({samplingIndices, \
                                      torch::indexing::Slice()});


    }

    /* =========================================================================
    Aim: Match-filtering
    ------------------------------------------------------------------------- */
    void nfdc_matchFilterDecimatedSignal(){
        // Creating a 2D marix out of the signal
        torch::Tensor matchFilter2DMatrix = \
            this->matchFilter.reshape({this->matchFilter.numel(), 1});
        matchFilter2DMatrix = torch::tile(matchFilter2DMatrix, {1, this->num_sensors});

        // 2D convolving to produce the match-filtering
        // std::cout<<"\t\t ULAClass::nfdc_matchFilterDecimatedSignal: this->signalMatrix.shape = "<<this->signalMatrix.sizes().vec()<<std::endl;
        fConvolveColumns(this->signalMatrix, matchFilter2DMatrix);

    }

    /* =========================================================================
    Aim: precompute delay-matrices
    ------------------------------------------------------------------------- */ 
    void nfdc_precomputeDelayMatrices(TransmitterClass* transmitterObj){
        
        // calculating range-related parameters
        int number_of_range_cells       = std::ceil(((this->recording_period * 1500)/2)/this->range_cell_size); 
        int number_of_azimuths_to_image = std::ceil(transmitterObj->azimuthal_beamwidth / this->azimuth_cell_size); 

        // status printing
        if(DEBUG_ULA) std::cout << "\t\t\t ULAClass: number_of_range_cells = "       << number_of_range_cells       <<  std::endl;
        if(DEBUG_ULA) std::cout << "\t\t\t ULAClass: number_of_azimuths_to_image = " << number_of_azimuths_to_image <<  std::endl;

        // find the centers of the range-cells. 
        torch::Tensor range_centers = \
            torch::linspace(this->azimuth_cell_size/2, \
                            (number_of_range_cells - 0.5)*this->azimuth_cell_size, \
                            number_of_range_cells);
        this->range_centers = range_centers;
        if(DEBUG_ULA) std::cout<<"range_centers.sizes().vec() = "<<range_centers.sizes().vec()<<std::endl;

        // finding the centers of azimuth-cells
        torch::Tensor azimuth_centers = \
            torch::linspace(this->range_cell_size/2, \
                            (number_of_azimuths_to_image - 0.5) * this->range_cell_size, \
                            number_of_azimuths_to_image);
        this->azimuth_centers = azimuth_centers;
        if(DEBUG_ULA) std::cout<<"azimuth_centers.sizes().vec() = "<<azimuth_centers.sizes().vec()<<std::endl;

        // finding the mesh values
        auto range_azimuth_meshgrid = \
            torch::meshgrid({range_centers, azimuth_centers}, "ij");
        torch::Tensor range_grid    = range_azimuth_meshgrid[0];    // the columns are range_centers
        torch::Tensor azimuth_grid  = range_azimuth_meshgrid[1];    // the rows are azimuth-centers
        
        // printing
        if(DEBUG_ULA) std::cout <<  "range_grid.sizes().vec()   = " <<  range_grid.sizes().vec()    <<  std::endl;
        if(DEBUG_ULA) std::cout <<  "azimuth_grid.sizes().vec() = " <<  azimuth_grid.sizes().vec()  <<  std::endl;

        // going from 2D to 3D
        range_grid = torch::tile(range_grid.reshape({range_grid.size(0), \
                                                     range_grid.size(1), \
                                                     1}), \
                                 {1,1,this->num_sensors});
        azimuth_grid = torch::tile(azimuth_grid.reshape({azimuth_grid.size(0), \
                                                         azimuth_grid.size(1), \
                                                         1}), \
                                          {1, 1, this->num_sensors});

        // printing 
        if(DEBUG_ULA) std::cout <<  "\t range_grid.sizes().vec()   = " <<  range_grid.sizes().vec()    <<  std::endl;
        if(DEBUG_ULA) std::cout <<  "\t azimuth_grid.sizes().vec() = " <<  azimuth_grid.sizes().vec()  <<  std::endl;

        // creating x_m tensor
        torch::Tensor sensorCoordinatesSlot = \
            this->inter_element_spacing * \
            torch::linspace(0, \
                            this->num_sensors - 1,\
                            this->num_sensors).reshape({1,1,this->num_sensors}).to(torch::kFloat);
        sensorCoordinatesSlot = \
            torch::tile(sensorCoordinatesSlot, \
                        {range_grid.size(0),\
                         range_grid.size(1),
                         1});
        if(DEBUG_ULA) std::cout <<  "\t sensorCoordinatesSlot.sizes().vec() = " <<  sensorCoordinatesSlot.sizes().vec()  <<  std::endl;

        // calculating distances
        torch::Tensor distanceMatrix =                                      \
            torch::square(range_grid - sensorCoordinatesSlot) +             \
            torch::mul((2 * sensorCoordinatesSlot),                         \
                       torch::mul(range_grid,                               \
                                  1 - torch::cos(azimuth_grid * PI/180)));
        distanceMatrix = \
            torch::sqrt(distanceMatrix);

        // finding the time taken
        torch::Tensor timeMatrix    = distanceMatrix/1500;

        // finding the delay to be given 
        auto bruh390                = torch::max(timeMatrix, 2, true);
        torch::Tensor max_delay     = std::get<0>(bruh390);
        torch::Tensor delayMatrix   = max_delay - timeMatrix;

        // // now that we have the delay entries, we need to create the matrix that does it
        int decimation_factor = \
            std::floor(this->sampling_frequency/transmitterObj->bandwidth);
        this->decimation_factor = decimation_factor;
        
        // calculating frame-size
        int frame_size = static_cast<float>(std::ceil((2 * this->range_cell_size / 1500)* this->sampling_frequency /decimation_factor));
        
        // creating the multiplication matrix
        torch::Tensor mulFFTMatrix = \
            torch::linspace(0, \
                            (int)(frame_size)-1, \
                            (int)(frame_size)).reshape({1, \
                                                 (int)(frame_size), \
                                                 1}).to(torch::kFloat);         // creating an array 1,...,frame_size of shape [1,frame_size, 1];
        mulFFTMatrix = \
            torch::div(mulFFTMatrix, \
                       torch::tensor(frame_size).to(torch::kFloat));            // dividing by N
        mulFFTMatrix = \
            mulFFTMatrix * 2 * PI * -1 * COMPLEX_1j;                            // creating tenosr values for -1j * 2pi * k/N
        mulFFTMatrix = \
            torch::tile(mulFFTMatrix, \
                        {number_of_range_cells * number_of_azimuths_to_image, \
                         1, \
                         this->num_sensors});                                   // creating the larger tensor for it


        // populating the matrix
        for(int azimuth_index = 0; azimuth_index<number_of_azimuths_to_image; ++azimuth_index){
            for(int range_index = 0; range_index < number_of_range_cells; ++range_index){
                // finding the delays for sensors
                torch::Tensor currentSensorDelays = \
                    delayMatrix.index({range_index, \
                                       azimuth_index, \
                                       torch::indexing::Slice()});
                // reshaping it to the target size
                currentSensorDelays = \
                    currentSensorDelays.reshape({1, \
                                                 1, \
                                                 this->num_sensors});
                // tiling across the plane
                currentSensorDelays = \
                    torch::tile(currentSensorDelays, \
                                {1, frame_size, 1});
                // multiplying across the appropriate plane
                int index_to_place_at = \
                    azimuth_index * number_of_range_cells + \
                    range_index;
                mulFFTMatrix.index_put_({index_to_place_at, \
                                        torch::indexing::Slice(),
                                        torch::indexing::Slice()}, \
                                        currentSensorDelays);
            }
        }

        // std::cout<<"\t\t\t mulFFTMatrix.sizes().vec() = "<<mulFFTMatrix.sizes().vec()<<std::endl;


        // storing the mulFFTMatrix
        this->mulFFTMatrix  = mulFFTMatrix;
            


    }

    // beamforming the signal
    void nfdc_beamforming(TransmitterClass* transmitterObj){        
        
        // ensuring the signal matrix is in the shape we want
        if(this->signalMatrix.size(1) != this->num_sensors) 
            throw std::runtime_error("The second dimension doesn't correspond to the number of sensors \n");

        
        // calculating frame-size from range-cell size
        int frame_size = 
            std::ceil((2 * static_cast<float>(this->range_cell_size)/1500) *           \
                        (static_cast<float>(this->sampling_frequency)/static_cast<float>(this->decimation_factor)));
        this->frame_size = frame_size;

        
        // adding the batch-dimension
        /* This is to accomodate a particular property of torch library.
        In torch, the first dimension is always the batch-related dimension.
        So in order to use the function torch::bmm(), we need to ensure that the first dimension is that of shape. */
        this->signalMatrix = \
            this->signalMatrix.reshape({1, \
                                        this->signalMatrix.size(0), \
                                        this->signalMatrix.size(1)}); 
        

        // zero-padding to ensure correctness
        int ideal_length                              = std::ceil(this->range_centers.numel() * frame_size);
        int num_zeros_to_pad_signal_along_dimension_0 = ideal_length - this->signalMatrix.size(1);
        torch::Tensor zero_tensor                     = torch::zeros({this->signalMatrix.size(0),                       \
                                                                        num_zeros_to_pad_signal_along_dimension_0,      \
                                                                        this->num_sensors}).to(torch::kFloat);
        this->signalMatrix                            = torch::cat({this->signalMatrix,                                 \
                                                                    zero_tensor}, 1);
        

        // breaking the signal into frames
        fBuffer2D(this->signalMatrix, frame_size);
        

        // tiling it to ensure that it works for all range-angle combinations
        int number_of_azimuths_to_image = this->azimuth_centers.numel(); 
        this->signalMatrix = \
            torch::tile(this->signalMatrix, \
                        {number_of_azimuths_to_image, 1, 1});

        
        // element-wise multiplying the signals
        this->signalMatrix = torch::mul(this->signalMatrix, this->mulFFTMatrix);
        this->signalMatrix = torch::sum(this->signalMatrix, 2, true);
        // this->signalMatrix = torch::sum(this->signalMatrix, 2, false);

        
        // printing
        std::cout<< "this->signalMatrix.sizes().vec() = "   << this->signalMatrix.sizes().vec() << std::endl;
        // std::cout<< "this->signalMatrix.sizes().vec() = "   << this->signalMatrix.sizes().vec() << std::endl;

        
        


        // creating a range-angle mesh for this

        // find the different angles for which we're beamforming. 

        // find the delays for the different range-angle combinations

        // splitting the signals into the different range-cells

        // loop for beamforming all of em 
    }





















};