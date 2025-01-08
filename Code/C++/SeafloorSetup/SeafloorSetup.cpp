/*
========================================
Aim: Setup sea floor
========================================
*/ 
#include <torch/torch.h>
#include <iostream>
#include <fstream>
#include <unordered_map>


// function to print tensor size
void print_tensor_size(const torch::Tensor& inputTensor) {
    // Printing size
    std::cout << "Tensor size: ";
    for (const auto& size : inputTensor.sizes()) {
        std::cout << size << " ";
    }
    std::cout << std::endl;
}

int main() {

  // setting up flat sea-floor
  double del_steps  = 1.0;
  double x_steps    = del_steps;
  double y_steps    = del_steps;

  // floor bounds
  double x_begin  = 0.0; 
  double x_end    = 4.0;
  double y_begin  = 0.0;
  double y_end    = 4.0;

  // setting up coordinates
  torch::Tensor x_flat_coordinates = torch::arange(x_begin, 
                                                   x_end + x_steps, 
                                                   x_steps);
  torch::Tensor y_flat_coordinates = torch::arange(y_begin, 
                                                   y_end + y_steps, 
                                                   y_steps);
  auto grids  = torch::meshgrid({x_flat_coordinates, y_flat_coordinates}, "xy");
  torch::Tensor x_temp = grids[0];
  torch::Tensor y_temp = grids[1];
  y_temp = torch::flip(y_temp, {0});    // flipping the tensor

  // flattening the tensor
  x_temp  = x_temp.flatten().unsqueeze(0);
  y_temp  = y_temp.flatten().unsqueeze(0);

  // concatenating dimensions
  torch::Tensor floor_coordinates_2D  = torch::cat({x_temp, y_temp}, 0);

  // setting up 3D coordinates
  torch::Tensor floor_coordinates_3D  = torch::cat({floor_coordinates_2D,
                                                    torch::zeros({1, floor_coordinates_2D.size(1)})},
                                                    0);

  // setting up scatter strengths
  torch::Tensor floor_scatterers_reflectivity = torch::normal(10.0,
                                                              1.0,
                                                              {1, floor_coordinates_3D.size(1)});

  // // instantiating an unordered map
  // std::unordered_map<std::string, torch::Tensor> floor_scatterers;
  // floor_scatterers["coordinates"] = floor_coordinates_3D;
  // floor_scatterers["reflectivity"] = floor_scatterers_reflectivity;

  // printing
  std::cout<<"=================================================================="<<std::endl;
  std::cout<<"floor_coordinates_3D \n"<<floor_coordinates_3D<<std::endl;
  std::cout<<"floor_scatterers_reflectivity \n"<<floor_scatterers_reflectivity<<std::endl;


  // saving directly
  torch::save(floor_coordinates_3D, 
              "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/floor_coordinates_3D.pt");
  torch::save(floor_scatterers_reflectivity, 
              "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/floor_scatterers_reflectivity.pt");
  std::cout<<"Saving torch::tensors"<<std::endl;

  // loading directly
  torch::Tensor floor_coordinates_3D_loaded;
  torch::Tensor floor_scatterers_reflectivity_loaded;

  torch::load(floor_coordinates_3D_loaded,
              "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/floor_coordinates_3D.pt");
  torch::load(floor_scatterers_reflectivity_loaded, 
              "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Assets/floor_scatterers_reflectivity.pt");
  std::cout<<"Loaded torch::tensors"<<std::endl;

  // returning
  return 0;
}