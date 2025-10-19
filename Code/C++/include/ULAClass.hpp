// // bringing in the header files
// #include <cstdint>
// #include <iostream>
// #include <ostream>
// #include <stdexcept>
// #include <torch/torch.h>
// #include <omp.h>            // the openMP


// // class definitions
// #include "ScattererClass.h"
// #include "TransmitterClass.h"

// // bringing in the functions
// #include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Functions/fPrintTensorSize.cpp"
// #include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Functions/fConvolveColumns.cpp"
// #include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Functions/fBuffer2D.cpp"
// #include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Functions/fConvolve1D.cpp"
// #include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Functions/fCart2Sph.cpp"

// #pragma once

// // hash defines
// #ifndef PRINTSPACE
//     #define PRINTSPACE      std::cout<<"\n\n\n\n\n\n\n\n"<<std::endl;
// #endif
// #ifndef PRINTSMALLLINE
//     #define PRINTSMALLLINE  std::cout<<"------------------------------------------------------------------------------------"<<std::endl;
// #endif
// #ifndef PRINTLINE
//     #define PRINTLINE       std::cout<<"===================================================================================="<<std::endl;
// #endif
// #ifndef PRINTDOTS
//     #define PRINTDOTS       std::cout<<"...................................................................................."<<std::endl;
// #endif


// #ifndef DEVICE
//     // #define DEVICE          torch::kMPS
//     #define DEVICE          torch::kCPU
// #endif

// #define PI              3.14159265
// #define COMPLEX_1j              torch::complex(torch::zeros({1}), torch::ones({1}))

// // #define DEBUG_ULA   true
// #define DEBUG_ULA   false



// class ULAClass{
// public:
//     // intrinsic parameters
//     int num_sensors;                    // number of sensors
//     float inter_element_spacing;        // space between sensors
//     torch::Tensor coordinates;          // coordinates of each sensor
//     float sampling_frequency;           // sampling frequency of the sensors
//     float recording_period;             // recording period of the ULA
//     torch::Tensor location;             // location of first coordinate

//     // derived stuff
//     torch::Tensor sensorDirection; 
//     torch::Tensor signalMatrix;

//     // decimation-related
//     int decimation_factor;                                  // the new decimation factor
//     float post_decimation_sampling_frequency;               // the new sampling frequency 
//     torch::Tensor lowpassFilterCoefficientsForDecimation;   // 

//     // imaging related 
//     float range_resolution;             // theoretical range-resolution = $\frac{c}{2B}$
//     float azimuthal_resolution;         // theoretical azimuth-resolution = $\frac{\lambda}{(N-1)*inter-element-distance}$
//     float range_cell_size;              // the range-cell quanta we're choosing for efficiency trade-off
//     float azimuth_cell_size;            // the azimuth quanta we're choosing
//     torch::Tensor mulFFTMatrix;         // the matrix containing the delays for each-element as a slot
//     torch::Tensor azimuth_centers;      // tensor containing the azimuth centeres
//     torch::Tensor range_centers;        // tensor containing the range-centers 
//     int frame_size;                     // the frame-size corresponding to a range cell in a decimated signal matrix
//     torch::Tensor matchFilter;          // torch tensor containing the match-filter
//     int num_buffer_zeros_per_frame;     // number of zeros we're adding per frame to ensure no-rotation
//     torch::Tensor beamformedImage;      // the beamformed image
//     torch::Tensor cartesianImage; 

//     // artificial acoustic-image related
//     torch::Tensor currentArtificalAcousticImage; // the acoustic image directly produced
    
//     // constructor
//     ULAClass(int numsensors                 = 32,
//              float inter_element_spacing    = 1e-3,
//              torch::Tensor coordinates      = torch::zeros({3, 2}),
//              float sampling_frequency       = 48e3,
//              float recording_period         = 1,
//              torch::Tensor location         = torch::zeros({3,1}),
//              torch::Tensor signalMatrix     = torch::zeros({1, 32}), 
//              torch::Tensor lowpassFilterCoefficientsForDecimation = torch::zeros({1,10})):
//              num_sensors(numsensors),
//              inter_element_spacing(inter_element_spacing),
//              coordinates(coordinates),
//              sampling_frequency(sampling_frequency),
//              recording_period(recording_period),
//              location(location),
//              signalMatrix(signalMatrix),
//              lowpassFilterCoefficientsForDecimation(lowpassFilterCoefficientsForDecimation){
//                 // calculating ULA direction
//                 torch::Tensor sensorDirection = coordinates.slice(1, 0, 1) - coordinates.slice(1, 1, 2);

//                 // normalizing 
//                 float normvalue = torch::linalg_norm(sensorDirection, 2, 0, true, torch::kFloat).item<float>();


//                 if (normvalue != 0){
//                     sensorDirection = sensorDirection / normvalue;
//                 }
                
//                 // copying direction 
//                 this->sensorDirection = sensorDirection.to(DATATYPE);
//         }

//     // overrinding printing
//     friend std::ostream& operator<<(std::ostream& os, ULAClass& ula){
//         os<<"\t number of sensors    : "<<ula.num_sensors           <<std::endl;
//         os<<"\t inter-element spacing: "<<ula.inter_element_spacing <<std::endl;
//         os<<"\t sensor-direction "      <<torch::transpose(ula.sensorDirection, 0, 1)<<std::endl;
//         PRINTSMALLLINE
//         return os;
//     }

//     /* =========================================================================
//     Aim: Init 
//     ------------------------------------------------------------------------- */ 
//     void init(TransmitterClass* transmitterObj){
        
//         // calculating range-related parameters
//         this->range_resolution      = 1500/(2 * transmitterObj->fc);
//         this->range_cell_size       = 40 * this->range_resolution;
//         if (DEBUG_ULA) std::cout << "\t ULACLASS::init: line 136" << std::endl;

//         // status printing
//         if (DEBUG_ULA) {
//             std::cout << "\t\t ULAClass::init():: this->range_resolution = "    \
//                       <<  this->range_resolution                                \
//                       <<  std::endl;
//             std::cout << "\t\t ULAClass::init():: this->range_cell_size  = "    \
//                       <<  this->range_cell_size                                 \
//                       <<  std::endl;
//         }
//         if (DEBUG_ULA) std::cout << "\t ULACLASS::init: line 147" << std::endl;

//         // calculating azimuth-related parameters
//         this->azimuthal_resolution =                                            \
//             (1500/transmitterObj->fc)                                           \
//             /((this->num_sensors-1)*this->inter_element_spacing); 
//         this->azimuth_cell_size         = 2 * this->azimuthal_resolution;
//         if (DEBUG_ULA) std::cout << "\t ULACLASS::init: line 154" << std::endl;

//         // creating and storing the match-filter
//         this->nfdc_CreateMatchFilter(transmitterObj);
//         if (DEBUG_ULA) std::cout << "\t ULACLASS::init: line 158" << std::endl;
//     }1

//     // Create match-filter
//     void nfdc_CreateMatchFilter(TransmitterClass* transmitterObj){

//         // creating matrix for basebanding the signal
//         torch::Tensor basebanding_vector =                  \
//             torch::linspace(                                \
//                 0,                                          \
//                 transmitterObj->Signal.numel()-1,           \
//                 transmitterObj->Signal.numel()              \
//                 ).reshape(transmitterObj->Signal.sizes());
//         basebanding_vector =                                    \
//             torch::exp(                                         \
//                 -1 * COMPLEX_1j * 2 * PI                             \
//                 * (transmitterObj->fc/this->sampling_frequency) \
//                 * basebanding_vector);
//         if (DEBUG_ULA) std::cout << "\t\t ULAClass::nfdc_createMatchFilter: line 176" << std::endl;

//         // multiplying the signal with the basebanding vector
//         torch::Tensor match_filter =                        \
//             torch::mul(transmitterObj->Signal,              \
//                        basebanding_vector);
//         if (DEBUG_ULA) std::cout << "\t\t ULAClass::nfdc_createMatchFilter: line 182" << std::endl;

//         // low-pass filtering to get the baseband signal
//         fConvolve1D(match_filter, this->lowpassFilterCoefficientsForDecimation);
//         if (DEBUG_ULA) std::cout << "\t\t ULAClass::nfdc_createMatchFilter: line 186" << std::endl;

//         // creating sampling-indices
//         int decimation_factor = \
//             std::floor((static_cast<float>(this->sampling_frequency)/2)       \
//                         /(static_cast<float>(transmitterObj->bandwidth)/2));
//         int final_num_samples = \
//             std::ceil(static_cast<float>(match_filter.numel())/static_cast<float>(decimation_factor));
//         torch::Tensor sampling_indices = \
//             torch::linspace(1, \
//                             (final_num_samples-1) * decimation_factor, 
//                             final_num_samples).to(torch::kInt) - torch::tensor({1}).to(torch::kInt);
//         if (DEBUG_ULA) std::cout << "ULAClass::nfdc_createMatchFilter: line 197" << std::endl;

//         // sampling the signal
//         match_filter = match_filter.index({sampling_indices});
        
//         // taking conjugate and flipping the signal
//         match_filter = torch::flipud(   match_filter);
//         match_filter = torch::conj(     match_filter);

//         // storing the match-filter to the class member
//         this->matchFilter = match_filter;
//     } 

//     // overloading the "=" operator
//     ULAClass& operator=(const ULAClass& other){
//         // checking if copying to the same object
//         if(this == &other){
//             return *this;
//         }

//         // copying everything
//         this->num_sensors           = other.num_sensors;
//         this->inter_element_spacing = other.inter_element_spacing;
//         this->coordinates           = other.coordinates.clone();
//         this->sampling_frequency    = other.sampling_frequency;
//         this->recording_period      = other.recording_period;
//         this->sensorDirection       = other.sensorDirection.clone(); 
        
//         // new additions
//         // this->location              = other.location;
//         this->lowpassFilterCoefficientsForDecimation = other.lowpassFilterCoefficientsForDecimation;
//         // this->sensorDirection       = other.sensorDirection.clone();
//         // this->signalMatrix          = other.signalMatrix.clone();


//         // returning
//         return *this;
//     }

//     // build sensor-coordinates based on location
//     void buildCoordinatesBasedOnLocation(){

//         // length-normalize the sensor-direction
//         this->sensorDirection = torch::div(this->sensorDirection, torch::linalg_norm(this->sensorDirection, \
//                                                                            2, 0, true, \
//                                                                            DATATYPE));
//         if(DEBUG_ULA) std::cout<<"\t ULAClass: line 105 \n";

//         // multiply with inter-element distance
//         this->sensorDirection = this->sensorDirection * this->inter_element_spacing;
//         this->sensorDirection = this->sensorDirection.reshape({this->sensorDirection.numel(), 1});
//         if(DEBUG_ULA) std::cout<<"\t ULAClass: line 110 \n";

//         // create integer-array
//         // torch::Tensor integer_array = torch::linspace(0, \
//         //                                               this->num_sensors-1, \
//         //                                               this->num_sensors).reshape({1, this->num_sensors}).to(DATATYPE);
//         torch::Tensor integer_array = torch::linspace(0,                                            \
//                                                       this->num_sensors-1,                          \
//                                                       this->num_sensors).reshape({1,                \
//                                                                                   this->num_sensors});
//         std::cout<<"integer_array = "; fPrintTensorSize(integer_array);
//         if(DEBUG_ULA) std::cout<<"\t ULAClass: line 116 \n";    

//         // 
//         torch::Tensor test = torch::mul(torch::tile(integer_array, {3, 1}).to(DATATYPE), \
//                                        torch::tile(this->sensorDirection, {1, this->num_sensors}).to(DATATYPE));
//         this->coordinates = this->location + test; 
//         if(DEBUG_ULA) std::cout<<"\t ULAClass: line 120 \n";
    
//     }

//     // signal simulation for the current sensor-array
//     void nfdc_simulateSignal(ScattererClass* scatterers,
//                              TransmitterClass* transmitterObj){

//         // creating signal matrix
//         int numsamples      = std::ceil((this->sampling_frequency * this->recording_period));
//         this->signalMatrix  = torch::zeros({numsamples, this->num_sensors}).to(DATATYPE);


//         // getting shape of coordinates
//         std::vector<int64_t> scatterers_coordinates_shape = \
//             scatterers->coordinates.sizes().vec();
        
//         // making a slot out of the coordinates
//         torch::Tensor slottedCoordinates =                                      \
//             torch::permute(scatterers->coordinates.reshape({
//                 scatterers_coordinates_shape[0],                                \
//                 scatterers_coordinates_shape[1],                                \
//                 1}                                                              \
//                 ), {2, 1, 0}).reshape({
//                     1,                                                          \
//                     (int)(scatterers->coordinates.numel()/3),                   \
//                     3});

        
//         // repeating along the y-direction number of sensor times. 
//         slottedCoordinates = 
//             torch::tile(slottedCoordinates,                                     \
//                         {this->num_sensors, 1, 1});
//         std::vector<int64_t> slottedCoordinates_shape =                         \
//             slottedCoordinates.sizes().vec();

        
//         // finding the shape of the sensor-coordinates
//         std::vector<int64_t> sensor_coordinates_shape = \
//             this->coordinates.sizes().vec();

//         // creating a slot tensor out of the sensor-coordinates
//         torch::Tensor slottedSensors =                                          \
//             torch::permute(this->coordinates.reshape({
//                 sensor_coordinates_shape[0],                                    \
//                 sensor_coordinates_shape[1],                                    \
//                 1}), {2, 1, 0}).reshape({(int)(this->coordinates.numel()/3),    \
//                                         1,                                      \
//                                         3});

        
//         // repeating slices along the x-coordinates
//         slottedSensors  =                                                       \
//             torch::tile(slottedSensors,                                         \
//                         {1, slottedCoordinates_shape[1], 1});

//         // slotting the coordinate of the transmitter and duplicating along dimensions [0] and [1]
//         torch::Tensor slotted_location =                                        \
//             torch::permute(this->location.reshape({3, 1, 1}),                   \
//                            {2, 1, 0}).reshape({1,1,3});
//         slotted_location =                                                      \
//             torch::tile(slotted_location, {slottedCoordinates_shape[0],         \
//                                            slottedCoordinates_shape[1],         \
//                                            1});



//         // subtracting to find the relative distances
//         torch::Tensor distBetweenScatterersAndSensors =                         \
//             torch::linalg_norm(slottedCoordinates - slottedSensors,             \
//                                2, 2, true, torch::kFloat).to(DATATYPE);

//         // substracting distance between relative fields
//         torch::Tensor distBetweenScatterersAndTransmitter =                     \
//             torch::linalg_norm(slottedCoordinates - slotted_location,           \
//                                2, 2, true, torch::kFloat).to(DATATYPE);

//         // adding up the distances
//         torch::Tensor distOfFlight      = \
//             distBetweenScatterersAndSensors + distBetweenScatterersAndTransmitter;
//         torch::Tensor timeOfFlight      = distOfFlight/1500;
//         torch::Tensor samplesOfFlight   = \
//             torch::floor(timeOfFlight.squeeze() \
//             * this->sampling_frequency);



//         // Adding pulses 
//         #pragma omp parallel for
//         for(int sensor_index = 0; sensor_index < this->num_sensors; ++sensor_index){
//             for(int scatter_index = 0; scatter_index < samplesOfFlight[0].numel(); ++scatter_index){
                
//                 // getting the sample where the current scatter's contribution must be placed. 
//                 int where_to_place =                        \
//                     samplesOfFlight.index({sensor_index,    \
//                                            scatter_index    \
//                                            }).item<int>();

//                 // checking whether that point is out of bounds
//                 if(where_to_place >= numsamples) continue;

//                 // placing a reflectivity-scaled impulse in there
//                 this->signalMatrix.index_put_({where_to_place, sensor_index},                       \
//                                               this->signalMatrix.index({where_to_place,             \
//                                                                         sensor_index}) +            \
//                                                 scatterers->reflectivity.index({0, \
//                                                                                 scatter_index}));
//             }
//         }



//         // // Adding pulses 
//         // for(int sensor_index = 0; sensor_index < this->num_sensors; ++sensor_index){

//         //     // indices associated with current index
//         //     torch::Tensor tensor_containing_placing_indices = \
//         //         samplesOfFlight[sensor_index].to(torch::kInt);

//         //     // calculating histogram
//         //     auto uniqueOutputs = at::_unique(tensor_containing_placing_indices, false, true);
//         //     torch::Tensor bruh = std::get<1>(uniqueOutputs);
//         //     torch::Tensor uniqueValues = std::get<0>(uniqueOutputs).to(torch::kInt);
//         //     torch::Tensor uniqueCounts = torch::bincount(bruh).to(torch::kInt);

//         //     // placing values according to histogram
//         //     this->signalMatrix.index_put_({uniqueValues.to(torch::kLong), sensor_index}, \
//         //                                   uniqueCounts.to(DATATYPE));

//         // }

//         // Creating matrix out of transmitted signal
//         torch::Tensor signalTensorAsArgument =              \
//             transmitterObj->Signal.reshape({
//                 transmitterObj->Signal.numel(),             \
//                 1});
//         signalTensorAsArgument =                            \
//             torch::tile(signalTensorAsArgument,             \
//                         {1, this->signalMatrix.size(1)});



//         // convolving the pulse-matrix with the signal matrix
//         fConvolveColumns(this->signalMatrix,        \
//                          signalTensorAsArgument);


//         // trimming the convolved signal since the signal matrix length remains the same
//         this->signalMatrix = \
//             this->signalMatrix.index({
//                 torch::indexing::Slice(0, numsamples), \
//                 torch::indexing::Slice()});


//         // returning
//         return;
//     }

//     /* =========================================================================
//     Aim: Decimating basebanded-received signal
//     ------------------------------------------------------------------------- */ 
//     void nfdc_decimateSignal(TransmitterClass* transmitterObj){
        
//         // creating the matrix for frequency-shifting
//         torch::Tensor integerArray  = torch::linspace(0, \
//                                                       this->signalMatrix.size(0)-1, \
//                                                       this->signalMatrix.size(0)).reshape({this->signalMatrix.size(0), 1}); 
//         integerArray                = torch::tile(integerArray, {1, this->num_sensors});
//         integerArray                = torch::exp(COMPLEX_1j * transmitterObj->fc * integerArray);

//         // storing original number of samples
//         int original_signalMatrix_numsamples = this->signalMatrix.size(0);

//         // producing frequency-shifting
//         this->signalMatrix          = torch::mul(this->signalMatrix, integerArray);

//         // low-pass filter
//         torch::Tensor lowpassfilter_impulseresponse =                           \
//             this->lowpassFilterCoefficientsForDecimation.reshape(               \
//                 {this->lowpassFilterCoefficientsForDecimation.numel(),          \
//                 1});
//         lowpassfilter_impulseresponse =                                         \
//             torch::tile(lowpassfilter_impulseresponse,                          \
//                         {1, this->signalMatrix.size(1)}); 

//         // low-pass filtering the signal
//         fConvolveColumns(this->signalMatrix, 
//                         lowpassfilter_impulseresponse); 

//         // Cutting down the extra-samples from convolution
//         this->signalMatrix = \
//             this->signalMatrix.index({torch::indexing::Slice(0, original_signalMatrix_numsamples), \
//                                       torch::indexing::Slice()});

//         // // Cutting off samples in the front. 
//         // int cutoffpoint = lowpassfilter_impulseresponse.size(0) - 1;
//         // this->signalMatrix =                                                    \
//         //     this->signalMatrix.index({                                          \
//         //         torch::indexing::Slice(cutoffpoint,                             \
//         //                                torch::indexing::None),                  \
//         //         torch::indexing::Slice()                                        \
//         //     });

//         // building parameters for downsampling
//         int decimation_factor           = std::floor(this->sampling_frequency/transmitterObj->bandwidth);
//         this->decimation_factor         = decimation_factor;
//         this->post_decimation_sampling_frequency =                              \
//             this->sampling_frequency / this->decimation_factor;
//         int numsamples_after_decimation = std::floor(this->signalMatrix.size(0)/decimation_factor);

//         // building the samples which will be subsetted
//         torch::Tensor samplingIndices = \
//             torch::linspace(0, \
//                             numsamples_after_decimation * decimation_factor - 1, \
//                             numsamples_after_decimation).to(torch::kInt);
        
//         // downsampling the low-pass filtered signal
//         this->signalMatrix = \
//             this->signalMatrix.index({samplingIndices, \
//                                       torch::indexing::Slice()});

//         // returning
//         return;
//     }

//     /* =========================================================================
//     Aim: Match-filtering
//     ------------------------------------------------------------------------- */
//     void nfdc_matchFilterDecimatedSignal(){
        
//         // Creating a 2D matrix out of the signal
//         torch::Tensor matchFilter2DMatrix = \
//             this->matchFilter.reshape({this->matchFilter.numel(), 1});
//         matchFilter2DMatrix = \
//             torch::tile(matchFilter2DMatrix, \
//                         {1, this->num_sensors});

        
//         // 2D convolving to produce the match-filtering
//         fConvolveColumns(this->signalMatrix,    \
//                         matchFilter2DMatrix);

        
//         // Trimming the signal to contain just the signals that make sense to us
//         int startingpoint   = matchFilter2DMatrix.size(0) - 1;
//         this->signalMatrix  =                                   \
//             this->signalMatrix.index({                          \
//                 torch::indexing::Slice(startingpoint,           \
//                                        torch::indexing::None),  \
//                 torch::indexing::Slice()});

//         // // trimming the two ends of the signal
//         // int startingpoint   = matchFilter2DMatrix.size(0) - 1;
//         // int endingpoint     = this->signalMatrix.size(0)    \
//         //                       - matchFilter2DMatrix.size(0) \
//         //                       + 1;
//         // this->signalMatrix  =                                   \
//         //     this->signalMatrix.index({                          \
//         //         torch::indexing::Slice(startingpoint,           \
//         //                                endingpoint),  \
//         //         torch::indexing::Slice()});
        

//     }

//     /* =========================================================================
//     Aim: precompute delay-matrices
//     ------------------------------------------------------------------------- */ 
//     void nfdc_precomputeDelayMatrices(TransmitterClass* transmitterObj){
        
//         // calculating range-related parameters
//         int number_of_range_cells       = \
//             std::ceil(((this->recording_period * 1500)/2)/this->range_cell_size); 
//         int number_of_azimuths_to_image = \
//             std::ceil(transmitterObj->azimuthal_beamwidth / this->azimuth_cell_size); 
        
//         // creating centers of range-cell centers
//         torch::Tensor range_centers = \
//             this->range_cell_size * \
//             torch::arange(0, number_of_range_cells) \
//             + this->range_cell_size/2;
//         this->range_centers = range_centers;
        
//         // creating discretized azimuth-centers
//         torch::Tensor azimuth_centers =                     \
//             this->azimuth_cell_size *                       \
//             torch::arange(0, number_of_azimuths_to_image)   \
//             + this->azimuth_cell_size/2;
//         this->azimuth_centers = azimuth_centers;
//         this->azimuth_centers = this->azimuth_centers - torch::mean(this->azimuth_centers);

//         // finding the mesh values
//         auto range_azimuth_meshgrid = \
//             torch::meshgrid({range_centers, \
//                              azimuth_centers}, "ij");
//         torch::Tensor range_grid    = range_azimuth_meshgrid[0];    // the columns are range_centers
//         torch::Tensor azimuth_grid  = range_azimuth_meshgrid[1];    // the rows are azimuth-centers
        
//         // going from 2D to 3D
//         range_grid = \
//             torch::tile(range_grid.reshape({range_grid.size(0), \
//                                             range_grid.size(1), \
//                                             1}), {1,1,this->num_sensors});
//         azimuth_grid = \
//             torch::tile(azimuth_grid.reshape({azimuth_grid.size(0), \
//                                               azimuth_grid.size(1), \
//                                               1}), {1, 1, this->num_sensors});

//         // creating x_m tensor
//         torch::Tensor sensorCoordinatesSlot = \
//             this->inter_element_spacing * \
//             torch::arange(0, this->num_sensors).reshape({
//                 1, 1, this->num_sensors
//             }).to(DATATYPE);

//         sensorCoordinatesSlot = \
//             torch::tile(sensorCoordinatesSlot, \
//                         {range_grid.size(0),\
//                          range_grid.size(1),
//                          1});

//         // calculating distances
//         torch::Tensor distanceMatrix =                                      \
//             torch::square(range_grid - sensorCoordinatesSlot) +             \
//             torch::mul((2 * sensorCoordinatesSlot),                         \
//                        torch::mul(range_grid,                               \
//                                   1 - torch::cos(azimuth_grid * PI/180)));
//         distanceMatrix = torch::sqrt(distanceMatrix);

//         // finding the time taken
//         torch::Tensor timeMatrix    = distanceMatrix/1500;
//         torch::Tensor sampleMatrix  = timeMatrix * this->sampling_frequency;

//         // finding the delay to be given 
//         auto bruh390                = torch::max(sampleMatrix, 2, true);
//         torch::Tensor max_delay     = std::get<0>(bruh390);
//         torch::Tensor delayMatrix   = max_delay - sampleMatrix;

//         // now that we have the delay entries, we need to create the matrix that does it
//         int decimation_factor = \
//             std::floor(static_cast<float>(this->sampling_frequency)/transmitterObj->bandwidth);
//         this->decimation_factor = decimation_factor;

        
//         // calculating frame-size
//         int frame_size = \
//             std::ceil(static_cast<float>((2 * this->range_cell_size / 1500) * \
//                         static_cast<float>(this->sampling_frequency)/decimation_factor));
//         this->frame_size = frame_size;

//         // // calculating the buffer-zeros to add 
//         // int num_buffer_zeros_per_frame = \
//         //     static_cast<float>(this->num_sensors - 1) * \
//         //     static_cast<float>(this->inter_element_spacing) * \
//         //     this->sampling_frequency /1500;

//         int num_buffer_zeros_per_frame =                    \
//             std::ceil((this->num_sensors - 1) *             \
//                        this->inter_element_spacing *        \
//                        this->sampling_frequency             \
//                        / (1500 * this->decimation_factor));

//         // storing to class member
//         this->num_buffer_zeros_per_frame = \
//             num_buffer_zeros_per_frame;
        
//         // calculating the total frame-size
//         int total_frame_size = \
//             this->frame_size + this->num_buffer_zeros_per_frame;

//         // creating the multiplication matrix
//         torch::Tensor mulFFTMatrix = \
//             torch::linspace(0, \
//                             total_frame_size-1, \
//                             total_frame_size).reshape({1, \
//                                                        total_frame_size, \
//                                                        1}).to(DATATYPE); // creating an array 1,...,frame_size of shape [1,frame_size, 1];
//         mulFFTMatrix = \
//             torch::div(mulFFTMatrix, \
//                        torch::tensor(total_frame_size).to(DATATYPE)); // dividing by N
//         mulFFTMatrix = mulFFTMatrix * 2 * PI * -1 * COMPLEX_1j; // creating tenosr values for -1j * 2pi * k/N
//         mulFFTMatrix = \
//             torch::tile(mulFFTMatrix, \
//                         {number_of_range_cells * number_of_azimuths_to_image, \
//                          1, \
//                          this->num_sensors}); // creating the larger tensor for it


//         // populating the matrix
//         for(int azimuth_index = 0; \
//             azimuth_index<number_of_azimuths_to_image; \
//             ++azimuth_index){
//             for(int range_index = 0; \
//                 range_index < number_of_range_cells; \
//                 ++range_index){
//                 // finding the delays for sensors
//                 torch::Tensor currentSensorDelays = \
//                     delayMatrix.index({range_index, \
//                                        azimuth_index, \
//                                        torch::indexing::Slice()});
//                 // reshaping it to the target size
//                 currentSensorDelays = \
//                     currentSensorDelays.reshape({1, \
//                                                  1, \
//                                                  this->num_sensors});
//                 // tiling across the plane
//                 currentSensorDelays = \
//                     torch::tile(currentSensorDelays, \
//                                 {1, total_frame_size, 1});
//                 // multiplying across the appropriate plane
//                 int index_to_place_at = \
//                     azimuth_index * number_of_range_cells + \
//                     range_index;
//                 mulFFTMatrix.index_put_({index_to_place_at, \
//                                         torch::indexing::Slice(),
//                                         torch::indexing::Slice()}, \
//                                         currentSensorDelays);
//             }
//         }

//         // storing the mulFFTMatrix
//         this->mulFFTMatrix  = mulFFTMatrix;
//     }

//     /* =========================================================================
//     Aim: Beamforming the signal
//     ------------------------------------------------------------------------- */ 
//     void nfdc_beamforming(TransmitterClass* transmitterObj){        
        
//         // ensuring the signal matrix is in the shape we want
//         if(this->signalMatrix.size(1) != this->num_sensors) 
//             throw std::runtime_error("The second dimension doesn't correspond to the number of sensors \n");
        
//         // adding the batch-dimension
//         this->signalMatrix = \
//             this->signalMatrix.reshape({                    \
//                 1,                                          \
//                 this->signalMatrix.size(0),                 \
//                 this->signalMatrix.size(1)}); 

        
//         // zero-padding to ensure correctness
//         int ideal_length = \
//             std::ceil(this->range_centers.numel() * this->frame_size);
//         int num_zeros_to_pad_signal_along_dimension_0 = \
//             ideal_length - this->signalMatrix.size(1);


//         // printing 
//         if (DEBUG_ULA) PRINTSMALLLINE
//         if (DEBUG_ULA) std::cout<<"\t\t ULAClass::nfdc_beamforming | this->range_centers.numel()               = "<<this->range_centers.numel()     <<std::endl;
//         if (DEBUG_ULA) std::cout<<"\t\t ULAClass::nfdc_beamforming | this->frame_size                          = "<<this->frame_size                 <<std::endl;
//         if (DEBUG_ULA) std::cout<<"\t\t ULAClass::nfdc_beamforming | ideal_length                              = "<<ideal_length                     <<std::endl;
//         if (DEBUG_ULA) std::cout<<"\t\t ULAClass::nfdc_beamforming | this->signalMatrix.size(1)                = "<<this->signalMatrix.size(1)       <<std::endl;
//         if (DEBUG_ULA) std::cout<<"\t\t ULAClass::nfdc_beamforming | num_zeros_to_pad_signal_along_dimension_0 = "<<num_zeros_to_pad_signal_along_dimension_0       <<std::endl; 
//         if (DEBUG_ULA) PRINTSPACE

//         // appending or slicing based on the requirements 
//         if (num_zeros_to_pad_signal_along_dimension_0 <= 0) {
            
//             // sending out a warning that slicing is going on
//             if (DEBUG_ULA) std::cerr <<"\t\t ULAClass::nfdc_beamforming | Please note that the signal matrix has been sliced. This could lead to loss of information"<<std::endl;

//             // slicing the signal matrix
//             if (DEBUG_ULA) PRINTSPACE
//             if (DEBUG_ULA) std::cout<<"\t\t ULAClass::nfdc_beamforming | this->signalMatrix.shape (before slicing) = "<< this->signalMatrix.sizes().vec() <<std::endl;
//             this->signalMatrix = \
//                 this->signalMatrix.index({torch::indexing::Slice(), \
//                                           torch::indexing::Slice(0, ideal_length), \
//                                           torch::indexing::Slice()});
//             if (DEBUG_ULA) std::cout<<"\t\t ULAClass::nfdc_beamforming | this->signalMatrix.shape (after slicing) = "<< this->signalMatrix.sizes().vec() <<std::endl;
//             if (DEBUG_ULA) PRINTSPACE

//         }
//         else {
//             // creating a zero-filled tensor to append to signal matrix
//             torch::Tensor zero_tensor   =                                   \
//                 torch::zeros({this->signalMatrix.size(0),                   \
//                               num_zeros_to_pad_signal_along_dimension_0,    \
//                               this->num_sensors}).to(DATATYPE);

//             // appending to signal matrix
//             this->signalMatrix          =                                   \
//                 torch::cat({this->signalMatrix, zero_tensor}, 1);
//         }

//         // breaking the signal into frames
//         fBuffer2D(this->signalMatrix, frame_size);


//         // add some zeros to the end of frames to accomodate delaying of signals. 
//         torch::Tensor zero_filled_tensor =                      \
//             torch::zeros({this->signalMatrix.size(0),           \
//                           this->num_buffer_zeros_per_frame,     \
//                           this->num_sensors}).to(DATATYPE);
//         this->signalMatrix =                                    \
//             torch::cat({this->signalMatrix,                     \
//                         zero_filled_tensor}, 1);


//         // tiling it to ensure that it works for all range-angle combinations
//         int number_of_azimuths_to_image = this->azimuth_centers.numel(); 
//         this->signalMatrix = \
//             torch::tile(this->signalMatrix, \
//                         {number_of_azimuths_to_image, 1, 1});

//         // element-wise multiplying the signals to delay each of the frame accordingly 
//         this->signalMatrix = torch::mul(this->signalMatrix, \
//                                         this->mulFFTMatrix);

//         // summing up the signals
//         // this->signalMatrix = torch::sum(this->signalMatrix, \
//         //                                 2,                  \
//         //                                 true);
//         this->signalMatrix = torch::sum(this->signalMatrix, \
//                                         2,                  \
//                                         false);

        
//         // printing some stuff
//         if (DEBUG_ULA) std::cout<<"\t\t ULAClass::nfdc_beamforming: this->azimuth_centers.numel()      = "<<this->azimuth_centers.numel()  <<std::endl; 
//         if (DEBUG_ULA) std::cout<<"\t\t ULAClass::nfdc_beamforming: this->range_centers.numel()        = "<<this->range_centers.numel()    <<std::endl;
//         if (DEBUG_ULA) std::cout<<"\t\t ULAClass::nfdc_beamforming: total number                       = "<<this->range_centers.numel() * this->azimuth_centers.numel()   <<std::endl;
//         if (DEBUG_ULA) std::cout<<"\t\t ULAClass::nfdc_beamforming: this->signalMatrix.sizes().vec()   = "<<this->signalMatrix.sizes().vec()    <<std::endl;

//         // creating a tensor to store the final image
//         torch::Tensor finalImage = \
//             torch::zeros({this->frame_size * this->range_centers.numel(), \
//                           this->azimuth_centers.numel()}).to(torch::kComplexFloat);


//         // creating a loop to assign values
//         for(int range_index = 0; range_index < this->range_centers.numel(); ++range_index){
//             for(int angle_index = 0; angle_index < this->azimuth_centers.numel(); ++angle_index){

//                 // getting row index
//                 int rowindex = \
//                     angle_index * this->range_centers.numel() \
//                     + range_index;

//                 // getting the strip to store
//                 torch::Tensor strip = \
//                     this->signalMatrix.index({rowindex, \
//                                               torch::indexing::Slice()});

//                 // taking just the first few values
//                 strip = strip.index({torch::indexing::Slice(0, this->frame_size)});

//                 // placing the strips on the image
//                 finalImage.index_put_({\
//                     torch::indexing::Slice((range_index)*this->frame_size, \
//                                             (range_index+1)*this->frame_size), \
//                                            angle_index}, \
//                                            strip);

//             }
//         }

//         // saving the image
//         this->beamformedImage = finalImage;



//         // converting image from polar to cartesian
//         nfdc_PolarToCartesian();


//     }

//     /* =========================================================================
//     Aim: Converting Polar Image to Cartesian
//     ............................................................................
//     Note:
//         > For now, we're assuming that the r value is one. 
//     ------------------------------------------------------------------------- */ 
//     void nfdc_PolarToCartesian(){


//         // deciding image dimensions
//         int num_pixels_width    = 128;
//         int num_pixels_height   = 128;


//         // creating query points
//         torch::Tensor max_right   =                                             \
//             torch::cos(                                                         \
//                 torch::max(                                                     \
//                     this->azimuth_centers                                       \
//                     - torch::mean(this->azimuth_centers)                        \
//                     + torch::tensor({90}).to(DATATYPE))                    \
//                 * PI/180);
//         torch::Tensor max_left    =                                             \
//             torch::cos(                                                         \
//                 torch::min(this->azimuth_centers                                \
//                            - torch::mean(this->azimuth_centers)                 \
//                            + torch::tensor({90}).to(DATATYPE))             \
//                 * PI/180);
//         torch::Tensor max_top     = torch::tensor({1});
//         torch::Tensor max_bottom  = torch::min(this->range_centers);
        

//         // creating query points along the x-dimension
//         torch::Tensor query_x =                             \
//             torch::linspace(                                \
//                 max_left,                                   \
//                 max_right,                                  \
//                 num_pixels_width                            \
//                 ).to(DATATYPE);
        
//         torch::Tensor query_y =                             \
//             torch::linspace(                                \
//                 max_bottom.item<float>(),                   \
//                 max_top.item<float>(),                      \
//                 num_pixels_height                           \
//                 ).to(DATATYPE);
        

//         // converting original coordinates to their corresponding cartesian
//         float delta_r = 1/static_cast<float>(this->beamformedImage.size(0));
//         float delta_azimuth =                               \
//             torch::abs(                                     \
//                 this->azimuth_centers.index({1})            \
//                 - this->azimuth_centers.index({0})          \
//                 ).item<float>();
        


//         // getting query points
//         torch::Tensor range_values = \
//             torch::linspace(                                \
//                 delta_r,                                    \
//                 this->beamformedImage.size(0) * delta_r,    \
//                 this->beamformedImage.size(0)               \
//                 ).to(DATATYPE);
//         range_values = \
//             range_values.reshape({range_values.numel(), 1});
//         range_values = \
//             torch::tile(range_values, \
//                         {1, this->azimuth_centers.numel()});
        

//         // getting angle-values
//         torch::Tensor angle_values =                        \
//             this->azimuth_centers                           \
//             - torch::mean(this->azimuth_centers)            \
//             + torch::tensor({90});
//         angle_values =                                      \
//             torch::tile(                                    \
//                 angle_values,                               \
//                 {this->beamformedImage.size(0), 1});


//         // converting to cartesian original points
//         torch::Tensor query_original_x = \
//             range_values * torch::cos(angle_values * PI/180);
//         torch::Tensor query_original_y = \
//             range_values * torch::sin(angle_values * PI/180);
        


//         // converting points to vector 2D format
//         torch::Tensor query_source =                                            \
//             torch::cat({                                                        \
//                 query_original_x.reshape({1, query_original_x.numel()}),        \
//                 query_original_y.reshape({1, query_original_y.numel()})},       \
//                 0);


//         // converting reflectivity to corresponding 2D format
//         torch::Tensor reflectivity_vectors = \
//             this->beamformedImage.reshape({1, this->beamformedImage.numel()});


//         // creating image
//         torch::Tensor cartesianImageLocal =                 \
//             torch::zeros(                                   \
//                 {num_pixels_height,                         \
//                  num_pixels_width}                          \
//                  ).to(torch::kComplexFloat);

//         /*
//         Next Aim: start interpolating the points on the uniform grid. 
//         */ 
//         #pragma omp parallel for
//         for(int x_index = 0; x_index < query_x.numel(); ++x_index){
//             // if(DEBUG_ULA) std::cout << "\t\t\t x_index = " << x_index << " ";
//             #pragma omp parallel for
//             for(int y_index = 0; y_index < query_y.numel(); ++y_index){
//                 // if(DEBUG_ULA) if(y_index%16 == 0) std::cout<<".";

//                 // getting current values
//                 torch::Tensor current_x = query_x.index({x_index}).reshape({1, 1});
//                 torch::Tensor current_y = query_y.index({y_index}).reshape({1, 1});

                


//                 // getting the query value
//                 torch::Tensor query_vector = torch::cat({current_x, current_y}, 0);


//                 // copying the query source
//                 torch::Tensor query_source_relative = query_source;
//                 query_source_relative = query_source_relative - query_vector;


//                 // subsetting using absolute values and masks 
//                 float threshold = delta_r * 10;
//                 // PRINTDOTS
//                 auto mask_row = \
//                     torch::abs(query_source_relative[0]) <= threshold;
//                 auto mask_col = \
//                     torch::abs(query_source_relative[1]) <= threshold;
//                 auto mask_together = torch::mul(mask_row, mask_col);


                

//                 // calculating number of points in threshold neighbourhood
//                 int num_points_in_threshold_neighbourhood = \
//                     torch::sum(mask_together).item<int>(); 
//                 if (num_points_in_threshold_neighbourhood == 0){
//                     continue;
//                 }
                


//                 // subsetting points in neighbourhood
//                 torch::Tensor PointsInNeighbourhood =       \
//                     query_source_relative.index({
//                         torch::indexing::Slice(),           \
//                         mask_together});
//                 torch::Tensor ReflectivitiesInNeighbourhood = \
//                     reflectivity_vectors.index({torch::indexing::Slice(), mask_together});


//                 // finding the distance between the points
//                 torch::Tensor relativeDistances = \
//                     torch::linalg_norm(PointsInNeighbourhood, \
//                                        2, 0, true, \
//                                        torch::kFloat).to(DATATYPE);


//                 // calculating weighing factor
//                 torch::Tensor weighingFactor =                                  \
//                     torch::nn::functional::softmax(                             \
//                         torch::max(relativeDistances)- relativeDistances,       \
//                         torch::nn::functional::SoftmaxFuncOptions(1));


//                 // combining intensities using distances
//                 torch::Tensor finalIntensity = \
//                     torch::sum(
//                         torch::mul(weighingFactor, \
//                                    ReflectivitiesInNeighbourhood));

//                 // assigning values
//                 cartesianImageLocal.index_put_({x_index, y_index}, finalIntensity);

//             }
//             // std::cout<<std::endl;
//         }

//         // saving to member function
//         this->cartesianImage = cartesianImageLocal;

//     }

//     /* =========================================================================
//     Aim: create acoustic image directly
//     ------------------------------------------------------------------------- */ 
//     void nfdc_createAcousticImage(ScattererClass* scatterers, \
//                                   TransmitterClass* transmitterObj){

//         // first we ensure that the scattersers are in our frame of reference
//         scatterers->coordinates = scatterers->coordinates - this->location;
        
//         // finding the spherical coordinates of the scatterers
//         torch::Tensor scatterers_spherical = fCart2Sph(scatterers->coordinates);

//         // note that its not precisely projection. its rotation. So the original lengths must be maintained. but thats easy since the operation of putting th eelevation to be zero works just fine. 
//         scatterers_spherical.index_put_({1, torch::indexing::Slice()}, 0);

//         // converting the points back to cartesian
//         torch::Tensor scatterers_acoustic_cartesian = fSph2Cart(scatterers_spherical);

//         // removing the z-dimension
//         scatterers_acoustic_cartesian = \
//             scatterers_acoustic_cartesian.index({torch::indexing::Slice(0, 2, 1), \
//                                                 torch::indexing::Slice()});

//         // deciding image dimensions
//         int num_pixels_x = 512;
//         int num_pixels_y = 512;
//         torch::Tensor acousticImage =                       \
//             torch::zeros({num_pixels_x,                     \
//                           num_pixels_y}).to(DATATYPE);

//         // finding the max and min values
//         torch::Tensor min_x     = torch::min(scatterers_acoustic_cartesian[0]);
//         torch::Tensor max_x     = torch::max(scatterers_acoustic_cartesian[0]);
//         torch::Tensor min_y     = torch::min(scatterers_acoustic_cartesian[1]);
//         torch::Tensor max_y     = torch::max(scatterers_acoustic_cartesian[1]);

//         // creating query grids
//         torch::Tensor query_x = torch::linspace(0, 1, num_pixels_x);
//         torch::Tensor query_y = torch::linspace(0, 1, num_pixels_y);

//         // scaling it up to image max-point spread
//         query_x             = min_x + (max_x - min_x) * query_x;
//         query_y             = min_y + (max_y - min_y) * query_y;
//         float delta_queryx  = (query_x[1] - query_x[0]).item<float>();
//         float delta_queryy  = (query_y[1] - query_y[0]).item<float>();

//         // creating a mesh-grid
//         auto queryMeshGrid = torch::meshgrid({query_x, query_y}, "ij");
//         query_x = queryMeshGrid[0].reshape({1, queryMeshGrid[0].numel()});
//         query_y = queryMeshGrid[1].reshape({1, queryMeshGrid[1].numel()});;
//         torch::Tensor queryMatrix = torch::cat({query_x, query_y}, 0);

//         // printing shapes
//         if(DEBUG_ULA) PRINTSMALLLINE
//         if(DEBUG_ULA) std::cout<<"\t\t ULAClass::nfdc_createAcousticImage: query_x.shape      = "<<query_x.sizes().vec()<<std::endl;
//         if(DEBUG_ULA) std::cout<<"\t\t ULAClass::nfdc_createAcousticImage: query_y.shape      = "<<query_y.sizes().vec()<<std::endl;
//         if(DEBUG_ULA) std::cout<<"\t\t ULAClass::nfdc_createAcousticImage: queryMatrix.shape  = "<<queryMatrix.sizes().vec()<<std::endl;

//         // setting up threshold values
//         float threshold_value =         \
//             std::min(delta_queryx,      \
//                      delta_queryy); if(DEBUG_ULA) std::cout<<"\t\t\t ULAClass::nfdc_createAcousticImage: line 711"<<std::endl;

//         // putting a loop through the whole thing
//         for(int i = 0; i<queryMatrix[0].numel(); ++i){
//             // for each element in the query matrix
//             if(DEBUG_ULA) std::cout<<"\t\t\t ULAClass::nfdc_createAcousticImage: line 716"<<std::endl;

//             // calculating relative position of all the points
//             torch::Tensor relativeCoordinates = \
//                 scatterers_acoustic_cartesian - \
//                 queryMatrix.index({torch::indexing::Slice(), i}).reshape({2, 1}); if(DEBUG_ULA) std::cout<<"\t\t\t ULAClass::nfdc_createAcousticImage: line 720"<<std::endl;

//             // calculating distances between all the points and the query point
//             torch::Tensor relativeDistances = \
//                 torch::linalg_norm(relativeCoordinates, \
//                                    1, 0, true, \
//                                    DATATYPE);if(DEBUG_ULA) std::cout<<"\t\t\t ULAClass::nfdc_createAcousticImage: line 727"<<std::endl;
//             // finding points that are within the threshold
//             torch::Tensor conditionMeetingPoints = \
//                 relativeDistances.squeeze() <= threshold_value;if(DEBUG_ULA) std::cout<<"\t\t\t ULAClass::nfdc_createAcousticImage: line 729"<<std::endl;

//             // subsetting the points in the neighbourhood
//             if(torch::sum(conditionMeetingPoints).item<float>() == 0){

//                 // continuing implementation if there are no points in the neighbourhood
//                 continue; if(DEBUG_ULA) std::cout<<"\t\t\t ULAClass::nfdc_createAcousticImage: line 735"<<std::endl;
//             }
//             else{
//                 // creating mask for points in the neighbourhood
//                 auto mask = (conditionMeetingPoints == 1);if(DEBUG_ULA) std::cout<<"\t\t\t ULAClass::nfdc_createAcousticImage: line 739"<<std::endl;

//                 // subsetting relative distances in the neighbourhood
//                 torch::Tensor distanceInTheNeighbourhood = \
//                     relativeDistances.index({torch::indexing::Slice(), mask});if(DEBUG_ULA) std::cout<<"\t\t\t ULAClass::nfdc_createAcousticImage: line 743"<<std::endl;

//                 // subsetting reflectivity of points in the neighbourhood
//                 torch::Tensor reflectivityInTheNeighbourhood = \
//                     scatterers->reflectivity.index({torch::indexing::Slice(), mask});if(DEBUG_ULA) std::cout<<"\t\t\t ULAClass::nfdc_createAcousticImage: line 747"<<std::endl;

//                 // assigning intensity as a function of distance and reflectivity
//                 torch::Tensor reflectivityAssignment =                          \
//                     torch::mul(torch::exp(-distanceInTheNeighbourhood),         \
//                                reflectivityInTheNeighbourhood);if(DEBUG_ULA) std::cout<<"\t\t\t ULAClass::nfdc_createAcousticImage: line 752"<<std::endl;
//                 reflectivityAssignment  = \
//                     torch::sum(reflectivityAssignment);if(DEBUG_ULA) std::cout<<"\t\t\t ULAClass::nfdc_createAcousticImage: line 754"<<std::endl;

//                 // assigning this value to the image pixel intensity
//                 int pixel_position_x = i%num_pixels_x;
//                 int pixel_position_y = std::floor(i/num_pixels_x);
//                 acousticImage.index_put_({pixel_position_x, \
//                                           pixel_position_y}, \
//                                          reflectivityAssignment.item<float>());if(DEBUG_ULA) std::cout<<"\t\t\t ULAClass::nfdc_createAcousticImage: line 761"<<std::endl;
//             }

//         }

//         // storing the acoustic-image to the member
//         this->currentArtificalAcousticImage = acousticImage;

//         // // saving the torch::tensor
//         // torch::save(acousticImage, \
//         //             "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/Assets/acoustic_image.pt");

        

//         // // bringing it back to the original coordinates
//         // scatterers->coordinates = scatterers->coordinates + this->location;
//     }



// };


template <
    svr::PureFloatingPointType          T,
    svr::FFT_SourceDestination_Type     sourceType,
    svr::FFT_SourceDestination_Type     destinationType,
    svr::PureComplexFloatingType        T_PureComplex
>
class ULAClass
{
public:
    // intrinsic parameters
    std::size_t                     num_sensors;                                // number of sensors
    T                               inter_element_spacing;                      // space between sensors
    std::vector<std::vector<T>>     coordinates;                                // coordinates of each sensor
    T                               sampling_frequency;                         // sampling frequency of the sensors
    T                               recording_period;                           // recording period of the ULA
    std::vector<T>                  location;                                   // location of first coordinate

    // derived 
    std::vector<T>                  sensor_direction;
    std::vector<std::vector<T>>     signal_matrix;
    std::size_t                     num_samples;

    // decimation related
    int                             decimation_factor;                              // the new decimation factor
    T                               post_decimation_sampling_frequency;             // the new sampling frequency 
    std::vector<T>                  lowpass_filter_coefficients_for_decimation;     // filter-coefficients for filtering

    // imaging related
    T   range_resolution;               // theoretical range-resolution = $\frac{c}{2B}$
    T   azimuthal_resolution;           // theoretical azimuth-resolution = $\frac{\lambda}{(N-1)*inter-element-distance}$
    T   range_cell_size;                // the range-cell quanta we're choosing for efficiency trade-off
    T   azimuth_cell_size;              // the azimuth quanta we're choosing
    std::vector<T>    azimuth_centers;  // tensor containing the azimuth centeres
    std::vector<T>    range_centers;    // tensor containing the range-centers 
    int  frame_size;                    // the frame-size corresponding to a range cell in a decimated signal matrix
    
    std::vector<std::vector<complex<T>>>    mulFFTMatrix;   // the matrix containing the delays for each-element as a slot
    std::vector<complex<T>>                 matchFilter;    // torch tensor containing the match-filter
    int    num_buffer_zeros_per_frame;                      // number of zeros we're adding per frame to ensure no-rotation
    std::vector<std::vector<T>>     beamformedImage;         // the beamformed image
    std::vector<std::vector<T>>     cartesianImage;          // the cartesian version of beamformed image

    // Decimating Related
    std::vector<std::complex<T>>    basebanding_signal;

    // Artificial acoustic-image related
    std::vector<std::vector<T>>     currentArtificialAcousticImage;    // acoustic image directly produced


    // Basic Constructor
    ULAClass()  = default;

    // constructor
    ULAClass(const  int     num_sensors_arg,
             const  auto    inter_element_spacing_arg,
             const  auto&   coordinates_arg,
             const  auto&   sampling_frequency_arg,
             const  auto&   recording_period_arg,
             const  auto&   location_arg,
             const  auto&   signalMatrix_arg,
             const  auto&   lowpass_filter_coefficients_for_decimation_arg):
                num_sensors(num_sensors_arg),
                inter_element_spacing(inter_element_spacing_arg),
                coordinates(std::move(coordinates_arg)),
                sampling_frequency(sampling_frequency_arg),
                recording_period(recording_period_arg),
                location(std::move(location_arg)),
                signal_matrix(std::move(signalMatrix_arg)),
                lowpass_filter_coefficients_for_decimation(std::move(lowpass_filter_coefficients_for_decimation_arg))
    {
        
        // calculating ULA direction
        sensor_direction   = std::vector<T>{coordinates[1][0] - coordinates[0][0],
                                            coordinates[1][1] - coordinates[0][1],
                                            coordinates[1][2] - coordinates[0][2]};

        // normalizing
        auto    norm_value_temp     {std::norm(std::inner_product(sensor_direction.begin(), 
                                                                  sensor_direction.end(),
                                                                  sensor_direction.begin(),
                                                                  0.00))};

        // dividing
        if (norm_value_temp != 0)   {sensor_direction   =   sensor_direction / norm_value_temp;}

    }

    // // deleting copy constructor/assignment
    // ULAClass<T>(const  ULAClass<T>&   other)                    = delete;
    // ULAClass<T>&   operator=(const  ULAClass<T>&   other)       = delete;
    ULAClass<T, sourceType, destinationType, T_PureComplex>(
        ULAClass<T, sourceType, destinationType, T_PureComplex>&&    other
    )                         = delete;
    ULAClass<T, sourceType, destinationType, T_PureComplex>&    operator=(
        const ULAClass<T, sourceType, destinationType, T_PureComplex>& other
    )         = default;

    // member-functions
    void    buildCoordinatesBasedOnLocation();
    void    buildCoordinatesBasedOnLocation(const  std::vector<T>&  new_location);
    void    init(
        const TransmitterClass<T>&                                          transmitterObj,
        svr::FFTPlanUniformPoolHandle<  T_PureComplex, T_PureComplex>&      fph_match_filter,
        svr::IFFTPlanUniformPoolHandle< T_PureComplex, T_PureComplex>&      ifph_match_filter
    );
    void    nfdc_CreateMatchFilter(
        const TransmitterClass<T>&                                              transmitterObj,
        svr::FFTPlanUniformPoolHandle<  T_PureComplex, T_PureComplex>&          fph_match_filter,
        svr::IFFTPlanUniformPoolHandle< T_PureComplex, T_PureComplex>&          ifph_match_filter
    );
    void    simulate_signals(
        const ScattererClass<T>&                                                seafloor,
        const std::vector<std::size_t>                                          scatterer_indices,
        const TransmitterClass<T>&                                              transmitter,
        svr::FFTPlanUniformPoolHandle<T, std::complex<T>>&                      fft_pool_handle,
        svr::IFFTPlanUniformPoolHandle<std::complex<T>, T>&                     ifft_pool_handle
    );
    void    build_sensor_coordinates_from_location();
    // void    decimate_signal();
    // void    decimate_signal(
    //     const  TransmitterClass<T>&   transmitter
    // );
    // void    decimate_signal(
    //     const  TransmitterClass<T>&                                                 transmitter,
    //     svr::FFTPlanUniformPoolHandle<  sourceType,         destinationType >&      fft_pool_handle,
    //     svr::IFFTPlanUniformPoolHandle< destinationType,    sourceType>&            ifft_pool_handle
    // );
    // void    decimate_signal(
    //     const  TransmitterClass<T>&                                                 transmitter,
    //     svr::FFTPlanUniformPoolHandle<      sourceType,         destinationType>&   fft_pool_handle,
    //     svr::IFFTPlanUniformPoolHandle<     destinationType,    sourceType>&        ifft_pool_handle);
    void    decimate_signal(
        const  TransmitterClass<T>&                                                 transmitter);
};
/* =============================================================================
Aim: Build Coordinates Based On Location
------------------------------------------------------------------------------*/
template <
    svr::PureFloatingPointType          T,
    svr::FFT_SourceDestination_Type     sourceType,
    svr::FFT_SourceDestination_Type     destinationType,
    svr::PureComplexFloatingType        T_PureComplex
>
void    ULAClass<T, sourceType, destinationType, T_PureComplex>::buildCoordinatesBasedOnLocation(){
    // length-normalizing sensor-direction
    this->sensor_direction      =   this->sensor_direction / norm(this->sensor_direction);

    // multiply with inter-element distance
    auto    inter_element_vector    {this->sensor_direction * \
                                     this->inter_element_spacing};
    
    // create integer array (verified)
    auto    integer_array       {svr::linspace<T>(
        static_cast<T>(0), 
        static_cast<T>(this->num_sensors-1),
        this->num_sensors
    )};

    auto    test    {svr::tile(integer_array,   {3,1})  *  \
                     svr::tile(transpose(inter_element_vector),   
                               {1, static_cast<std::size_t>(this->num_sensors)})};
    
    // translating coordinates
    this->coordinates   =  transpose( this->location)  +  test;}

/*==============================================================================
------------------------------------------------------------------------------*/ 
template <
    svr::PureFloatingPointType          T,
    svr::FFT_SourceDestination_Type     sourceType,
    svr::FFT_SourceDestination_Type     destinationType,
    svr::PureComplexFloatingType        T_PureComplex
>
void    ULAClass<T, sourceType, destinationType, T_PureComplex>::buildCoordinatesBasedOnLocation(
    const  std::vector<T>&  new_location
){
    // updating location
    this->location  = new_location;

    // calling the update-coordinates-function
    this->buildCoordinatesBasedOnLocation();}
/* =============================================================================
Aim: Init 
------------------------------------------------------------------------------*/
template <
    svr::PureFloatingPointType          T,
    svr::FFT_SourceDestination_Type     sourceType,
    svr::FFT_SourceDestination_Type     destinationType,
    svr::PureComplexFloatingType        T_PureComplex
>
void    ULAClass<T, sourceType, destinationType, T_PureComplex>::init(
    const TransmitterClass<T>&                                          transmitterObj,
    svr::FFTPlanUniformPoolHandle<  T_PureComplex, T_PureComplex>&      fph_match_filter,
    svr::IFFTPlanUniformPoolHandle< T_PureComplex, T_PureComplex>&      ifph_match_filter
){    
    // calculating range-related parameters
    this->range_resolution      =   1500.00/(2 * transmitterObj.fc);
    this->range_cell_size       =   40  *   this->range_resolution;
    // calculating azimuth-related parameters
    this->azimuthal_resolution  =   (1500.00 / transmitterObj.fc)   /   \
                                    (this->num_sensors - 1)  *   (this->inter_element_spacing);
    this->azimuth_cell_size     =   2   *   this->azimuthal_resolution;


    // creating basebanding signal
    this->basebanding_signal    =   svr::linspace(
        static_cast<std::complex<T>>(   0),
        static_cast<std::complex<T>>(   this->num_samples-1),
        static_cast<std::size_t>(       this->num_samples)
    );
    // exponentiating signal to create final-basebanding matrix
    this->basebanding_signal  =   svr::exp(
        1i * 2 * std::numbers::pi * transmitterObj.f_low * \
        basebanding_signal / sampling_frequency
    );
    // asserting that the size of the signal is the same as that of the 
    if (this->basebanding_signal.size() != this->num_samples)
        throw std::runtime_error(
            "FILE: ULAClass.hpp | FUNCTION: ULAClass::init() | REPORT: basebanding_signal.size() != num_samples"
        );

        
    // creating and storing match-filter
    // this->nfdc_CreateMatchFilter(std::ref(transmitterObj));
    // this->nfdc_CreateMatchFilter(transmitterObj);
    this->nfdc_CreateMatchFilter(   transmitterObj, 
                                    fph_match_filter, 
                                    ifph_match_filter);

    // logging
    spdlog::info("Finished ULA Initialization");}
/* =============================================================================
Aim: Creating match-filter
------------------------------------------------------------------------------*/
template <
    svr::PureFloatingPointType          T,
    svr::FFT_SourceDestination_Type     sourceType,
    svr::FFT_SourceDestination_Type     destinationType,
    svr::PureComplexFloatingType        T_PureComplex
>
void    ULAClass<T, sourceType, destinationType, T_PureComplex>::nfdc_CreateMatchFilter(
    const TransmitterClass<T>&                                          transmitterObj,
    svr::FFTPlanUniformPoolHandle<  T_PureComplex, T_PureComplex>&      fph_match_filter,
    svr::IFFTPlanUniformPoolHandle< T_PureComplex, T_PureComplex>&      ifph_match_filter
){
    // creating matrix for basebanding signal
    auto    linspace00              {svr::linspace(
        0,
        transmitterObj.signal.size()-1,
        transmitterObj.signal.size()
    )};
    auto    basebanding_vector      {
        svr::exp(
            1.00 * 1i * 2.00 * std::numbers::pi                 * \
            (transmitterObj.f_low / this->sampling_frequency)   * \
            linspace00
        )
    };

    // multiplying signal with basebanding signal
    auto    match_filter        {   transmitterObj.signal * basebanding_vector   };
    basebanding_vector.clear();

    // low-pass filtering with baseband signal to obtain pure baseband signal
    spdlog::warn("Shift to convolution method plan-based method");
    match_filter    =   svr::conv1D(
        match_filter,
        svr::complex(   this->lowpass_filter_coefficients_for_decimation    )
        // this->lowpass_filter_coefficients_for_decimation
    );
    // match_filter    =   svr::conv1D_long_FFTPlanPool(
    //     match_filter,
    //     svr::complex(   this->lowpass_filter_coefficients_for_decimation    ),
    //     fph_match_filter,
    //     ifph_match_filter
    // );
    
    // creating sampling-indices
    int     decimation_factor   {static_cast<int>(std::floor(
        (static_cast<T>(this->sampling_frequency)/2.00) /   \
        (static_cast<T>(transmitterObj.bandwidth)/2.00))
    )};
    int     final_num_samples   {static_cast<int>(std::ceil(
        static_cast<T>(match_filter.size())/                \
        static_cast<T>(decimation_factor)
    ))};
    auto    sampling_indices    {
        svr::linspace(
            1,
            (final_num_samples - 1) * decimation_factor,
            final_num_samples
        )
    };

    // sampling the signal
    match_filter    =   svr::index(match_filter, sampling_indices);

    // taking conjugate and flipping the signal
    match_filter    =   svr::fliplr(    match_filter);
    match_filter    =   svr::conj(      match_filter);

    // storing the match-filter to the class member
    this->matchFilter   =   std::move(match_filter);

    // Logging
    spdlog::info("Finished match-filter creation");}
/*==============================================================================
Aim: Simulate signals received by uniform-linear-array
------------------------------------------------------------------------------*/ 
template <
    svr::PureFloatingPointType          T,
    svr::FFT_SourceDestination_Type     sourceType,
    svr::FFT_SourceDestination_Type     destinationType,
    svr::PureComplexFloatingType        T_PureComplex
>
void    ULAClass<T, sourceType, destinationType, T_PureComplex>::simulate_signals(
    const ScattererClass<T>&                                seafloor,
    const std::vector<std::size_t>                          scatterer_indices,
    const TransmitterClass<T>&                              transmitter,
    svr::FFTPlanUniformPoolHandle<T, std::complex<T>>&      fft_pool_handle,
    svr::IFFTPlanUniformPoolHandle<std::complex<T>, T>&     ifft_pool_handle
){
    // creating signal matrix
    auto    num_samples     {static_cast<std::size_t>(
        std::ceil(
            this->sampling_frequency * this->recording_period
        ))} ;
    this->signal_matrix     = svr::zeros<T>({this->num_sensors, num_samples});

    // placing dots for each sensor
    for(auto    sensor_index    =   0;
        sensor_index    < num_sensors;
        ++sensor_index)
    {
        // fetching sensor coordinates
        const   auto   curr_sensor_coordinate   {std::vector<T>({
            coordinates[0][sensor_index],
            coordinates[1][sensor_index],
            coordinates[2][sensor_index]
        })};

        // subsetting scatterers
        const   auto    subsetted_scatterers    {svr::index(seafloor.coordinates,
                                                            scatterer_indices,
                                                            0)};
        const   auto    subset_scatterer_return {svr::index(seafloor.reflectivity,
                                                            scatterer_indices)};
                                                            
        // calculating distance between sensor and scatterer
        auto    dist_from_sensor    {subsetted_scatterers - transpose(curr_sensor_coordinate)};
        dist_from_sensor        =   norm(dist_from_sensor, 0);

        // distance between transmitter and scatterer
        auto    dist_from_transmitter   {subsetted_scatterers - transpose(curr_sensor_coordinate)};
        dist_from_transmitter   =   norm(dist_from_transmitter, 0);
        
        // calculating time-of-flight
        auto   time_of_flight  {
            dist_from_sensor/transmitter.speed_of_sound     + \
            dist_from_transmitter/transmitter.speed_of_sound
        };
        auto&   indices_of_flight   {time_of_flight};
        indices_of_flight   = svr::floor(time_of_flight * this->sampling_frequency);

        // placing returns in the signal matrix
        edit_accumulate(this->signal_matrix[sensor_index],
                        svr::squeeze(indices_of_flight),
                        subset_scatterer_return);

    }

    // convolving input-signal with each row
    for(auto row = 0; row < this->signal_matrix.size(); ++row)
    {
        // peforming convolution
        this->signal_matrix[row] = svr::conv1D_long_FFTPlanPool(
            this->signal_matrix[row],
            transmitter.signal,
            fft_pool_handle,
            ifft_pool_handle
        );

        // only keeping first num-samples amount of samples
        this->signal_matrix[row].resize(this->num_samples);
    }

    // logging
    spdlog::info("Finished signal-simulation");}
/*==============================================================================
Decimating the recorded signal
    -> basebanding the signal
    -> lowpass-filtering
    -> downsample signals
------------------------------------------------------------------------------*/
template <
    svr::PureFloatingPointType          T,
    svr::FFT_SourceDestination_Type     sourceType,
    svr::FFT_SourceDestination_Type     destinationType,
    svr::PureComplexFloatingType        T_PureComplex
>
void    ULAClass<T, sourceType, destinationType, T_PureComplex>::decimate_signal(
    const  TransmitterClass<T>&                                                 transmitter
)
{
    // multiplying with signal to baseband signal
    auto    basebanded_signal_matrix   {
        this->signal_matrix * this->basebanding_signal
    };

    // // running low-pass filter
    // auto&   basebanded_lowpassfiltered_signal_matrix    {
    //     basebanded_signal_matrix
    // };

    // auto&   basebanded_lowpassfiltered_signal_matrix    {
    //     svr::conv1D_long_FFTPlanPool(
    //         basebanded_signal_matrix,
    //         this->lowpass_filter_coefficients_for_decimation,
    //         fft_pool_handle,
    //         ifft_pool_handle
    //     )
    // };



    


    // logging
    spdlog::warn("signal-decimation | incomplete");

}