/* =====================================
Aim: Setup sea floor
======================================*/ 
// #include <torch/torch.h>
// #include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/include/ULAClass.h"

// int main() {

//     // setting up ula
//     int num_sensors             = 4;    // number of sensors
//     float inter_element_spacing = 1e-3; // space between samples
//     float sampling_frequency    = 16e3; // sampling frequency
//     float recording_period      = 1;    // sampling-period

//     torch::Tensor ULA_direction = torch::tensor({0,1,0}).reshape({3,1}).to(torch::kFloat);
//     ULA_direction       = ULA_direction/torch::linalg_norm(ULA_direction, 2, 0, true, torch::kFloat);
//     ULA_direction       = ULA_direction * inter_element_spacing;
//     torch::Tensor ULA_coordinates = torch::mul(torch::linspace(0, num_sensors-1, num_sensors), ULA_direction);

//     // creating class object
//     ULAClass ula(num_sensors,
//                  inter_element_spacing,
//                  ULA_coordinates,
//                  sampling_frequency,
//                  recording_period);
    
//     // returning
//     return 0;
// }

// =========================================================
void ULASetup(ULAClass* ula) {

    // setting up ula
    int num_sensors             = 4;    // number of sensors
    float inter_element_spacing = 1e-3; // space between samples
    float sampling_frequency    = 16e3; // sampling frequency
    float recording_period      = 1;    // sampling-period

    torch::Tensor ULA_direction = torch::tensor({0,1,0}).reshape({3,1}).to(torch::kFloat);
    ULA_direction       = ULA_direction/torch::linalg_norm(ULA_direction, 2, 0, true, torch::kFloat);
    ULA_direction       = ULA_direction * inter_element_spacing;
    torch::Tensor ULA_coordinates = torch::mul(torch::linspace(0, num_sensors-1, num_sensors), ULA_direction);

    // assigning values 
    ula->num_sensors             = num_sensors;              // assigning number of sensors
    ula->inter_element_spacing   = inter_element_spacing;    // assigning inter-element spacing
    ula->coordinates             = ULA_coordinates;          // assigning ULA coordinates
    ula->sampling_frequency      = sampling_frequency;       // assigning sampling frequencys
    ula->recording_period        = recording_period;         // assigning recording period

    // ula.num_sensors             = num_sensors;              // assigning number of sensors
    // ula.inter_element_spacing   = inter_element_spacing;    // assigning inter-element spacing
    // ula.coordinates             = ULA_coordinates;          // assigning ULA coordinates
    // ula.sampling_frequency      = sampling_frequency;       // assigning sampling frequencys
    // ula.recording_period        = recording_period;         // assigning recording period
    
}