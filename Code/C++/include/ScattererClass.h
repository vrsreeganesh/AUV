// header-files
#include <iostream>
#include <ostream>
#include <torch/torch.h>

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
    
    // public variables
    torch::Tensor coordinates;  // tensor holding coordinates [3, x]
    torch::Tensor reflectivity; // tensor holding reflectivity [1, x]
        
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

    // copy constructor from a pointer
    ScattererClass(ScattererClass* scatterers){

        // copying the values 
        this->coordinates = scatterers->coordinates;
        this->reflectivity = scatterers->reflectivity;
    }
    
};