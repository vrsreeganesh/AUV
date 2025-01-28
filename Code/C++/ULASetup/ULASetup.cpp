/* =====================================
Aim: Setup sea floor
NOAA: 50 to 100 KHz is the transmission frequency
we'll create our LFM with 50 to 70KHz
======================================*/ 

#ifndef DEVICE
    #define DEVICE          torch::kMPS
    // #define DEVICE          torch::kCPU
#endif



// =========================================================
void ULASetup(ULAClass* ula_1, 
              ULAClass* ula_2) {

    // setting up ula
    int num_sensors             = 64;                           // number of sensors
    float sampling_frequency    = 160e3;                        // sampling frequency
    float inter_element_spacing = 1500/(2*sampling_frequency);  // space between samples
    float recording_period      = 1;                            // sampling-period

    // building the direction for the sensors
    torch::Tensor ULA_direction = torch::tensor({0,1,0}).reshape({3,1}).to(torch::kFloat).to(DEVICE);
    ULA_direction       = ULA_direction/torch::linalg_norm(ULA_direction, 2, 0, true, torch::kFloat).to(DEVICE);
    ULA_direction       = ULA_direction * inter_element_spacing;

    // building the coordinates for the sensors
    torch::Tensor ULA_coordinates = torch::mul(torch::linspace(0, num_sensors-1, num_sensors).to(DEVICE), \
                                               ULA_direction);

    // assigning values 
    ula_1->num_sensors            = num_sensors;              // assigning number of sensors
    ula_1->inter_element_spacing  = inter_element_spacing;    // assigning inter-element spacing
    ula_1->coordinates            = ULA_coordinates;          // assigning ULA coordinates
    ula_1->sampling_frequency     = sampling_frequency;       // assigning sampling frequencys
    ula_1->recording_period       = recording_period;         // assigning recording period
    ula_1->sensorDirection        = ULA_direction;            // ULA direction 

    ula_1->num_sensors            = num_sensors;              // assigning number of sensors
    ula_1->inter_element_spacing  = inter_element_spacing;    // assigning inter-element spacing
    ula_1->coordinates            = ULA_coordinates;          // assigning ULA coordinates
    ula_1->sampling_frequency     = sampling_frequency;       // assigning sampling frequencys
    ula_1->recording_period       = recording_period;         // assigning recording period
    ula_1->sensorDirection        = ULA_direction;            // ULA direction 

    // assigning values 
    ula_2->num_sensors            = num_sensors;              // assigning number of sensors
    ula_2->inter_element_spacing  = inter_element_spacing;    // assigning inter-element spacing
    ula_2->coordinates            = ULA_coordinates;          // assigning ULA coordinates
    ula_2->sampling_frequency     = sampling_frequency;       // assigning sampling frequencys
    ula_2->recording_period       = recording_period;         // assigning recording period
    ula_2->sensorDirection        = ULA_direction;            // ULA direction 

    ula_2->num_sensors            = num_sensors;              // assigning number of sensors
    ula_2->inter_element_spacing  = inter_element_spacing;    // assigning inter-element spacing
    ula_2->coordinates            = ULA_coordinates;          // assigning ULA coordinates
    ula_2->sampling_frequency     = sampling_frequency;       // assigning sampling frequencys
    ula_2->recording_period       = recording_period;         // assigning recording period
    ula_2->sensorDirection        = ULA_direction;            // ULA direction 
    
}