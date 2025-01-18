// header-files
#include <iostream>
#include <ostream>

// function to print tensor size
void print_tensor_size(const torch::Tensor& inputTensor) {
    // Printing size
    std::cout << "[";
    for (const auto& size : inputTensor.sizes()) {
        std::cout << size << ",";
    }
    std::cout << "\b]" <<std::endl;
}


// Scatterer Class = Scatterer Class
// Scatterer Class = Scatterer Class
// Scatterer Class = Scatterer Class
// Scatterer Class = Scatterer Class
// Scatterer Class = Scatterer Class
class ScattererClass{
public:
    // public members
    torch::Tensor coordinates;
    torch::Tensor reflectivity;

    // constructor = constructor
    ScattererClass(torch::Tensor arg_coordinates    = torch::zeros({3,1}), 
                   torch::Tensor arg_reflectivity   = torch::zeros({3,1})):
                   coordinates(arg_coordinates),
                   reflectivity(arg_reflectivity) {}

    // overloading output
    friend std::ostream& operator<<(std::ostream& os, ScattererClass& scatterer){

        // printing coordinate shape
        os<<"\t> scatterer.coordinates.shape    = ";
        print_tensor_size(scatterer.coordinates);

        // printing reflectivity shape
        os<<"\t> scatterer.reflectivity.shape   = ";
        print_tensor_size(scatterer.reflectivity);

        // returning os
        return os;
    }
    
};