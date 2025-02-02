/* =====================================
Aim: Setup sea floor
NOAA: 50 to 100 KHz is the transmission frequency
we'll create our LFM with 50 to 70KHz
======================================*/ 


// Choosing device 
#ifndef DEVICE
    // #define DEVICE          torch::kMPS
    #define DEVICE          torch::kCPU
#endif




void ULASetup(ULAClass* ula_fls, 
              ULAClass* ula_port,
              ULAClass* ula_starboard) {

    // setting up ula
    int num_sensors             = 64;                           // number of sensors
    float sampling_frequency    = 160e3;                        // sampling frequency
    float inter_element_spacing = 1500/(2*sampling_frequency);  // space between samples
    float recording_period      = 0.1;                            // sampling-period

    // building the direction for the sensors
    torch::Tensor ULA_direction = torch::tensor({-1,0,0}).reshape({3,1}).to(torch::kFloat).to(DEVICE);
    ULA_direction               = ULA_direction/torch::linalg_norm(ULA_direction, 2, 0, true, torch::kFloat).to(DEVICE);
    ULA_direction               = ULA_direction * inter_element_spacing;

    // building the coordinates for the sensors
    torch::Tensor ULA_coordinates = torch::mul(torch::linspace(0, num_sensors-1, num_sensors).to(DEVICE), \
                                               ULA_direction);

    // assigning values 
    ula_fls->num_sensors            = num_sensors;              // assigning number of sensors
    ula_fls->inter_element_spacing  = inter_element_spacing;    // assigning inter-element spacing
    ula_fls->coordinates            = ULA_coordinates;          // assigning ULA coordinates
    ula_fls->sampling_frequency     = sampling_frequency;       // assigning sampling frequencys
    ula_fls->recording_period       = recording_period;         // assigning recording period
    ula_fls->sensorDirection        = ULA_direction;            // ULA direction 

    ula_fls->num_sensors            = num_sensors;              // assigning number of sensors
    ula_fls->inter_element_spacing  = inter_element_spacing;    // assigning inter-element spacing
    ula_fls->coordinates            = ULA_coordinates;          // assigning ULA coordinates
    ula_fls->sampling_frequency     = sampling_frequency;       // assigning sampling frequencys
    ula_fls->recording_period       = recording_period;         // assigning recording period
    ula_fls->sensorDirection        = ULA_direction;            // ULA direction 

    // assigning values 
    ula_port->num_sensors            = num_sensors;              // assigning number of sensors
    ula_port->inter_element_spacing  = inter_element_spacing;    // assigning inter-element spacing
    ula_port->coordinates            = ULA_coordinates;          // assigning ULA coordinates
    ula_port->sampling_frequency     = sampling_frequency;       // assigning sampling frequencys
    ula_port->recording_period       = recording_period;         // assigning recording period
    ula_port->sensorDirection        = ULA_direction;            // ULA direction 

    ula_port->num_sensors            = num_sensors;              // assigning number of sensors
    ula_port->inter_element_spacing  = inter_element_spacing;    // assigning inter-element spacing
    ula_port->coordinates            = ULA_coordinates;          // assigning ULA coordinates
    ula_port->sampling_frequency     = sampling_frequency;       // assigning sampling frequencys
    ula_port->recording_period       = recording_period;         // assigning recording period
    ula_port->sensorDirection        = ULA_direction;            // ULA direction 


    // assigning values 
    ula_starboard->num_sensors            = num_sensors;              // assigning number of sensors
    ula_starboard->inter_element_spacing  = inter_element_spacing;    // assigning inter-element spacing
    ula_starboard->coordinates            = ULA_coordinates;          // assigning ULA coordinates
    ula_starboard->sampling_frequency     = sampling_frequency;       // assigning sampling frequencys
    ula_starboard->recording_period       = recording_period;         // assigning recording period
    ula_starboard->sensorDirection        = ULA_direction;            // ULA direction 

    ula_starboard->num_sensors            = num_sensors;              // assigning number of sensors
    ula_starboard->inter_element_spacing  = inter_element_spacing;    // assigning inter-element spacing
    ula_starboard->coordinates            = ULA_coordinates;          // assigning ULA coordinates
    ula_starboard->sampling_frequency     = sampling_frequency;       // assigning sampling frequencys
    ula_starboard->recording_period       = recording_period;         // assigning recording period
    ula_starboard->sensorDirection        = ULA_direction;            // ULA direction 
    
}