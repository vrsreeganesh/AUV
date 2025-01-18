/* =====================================
Aim: Setup sea floor
======================================*/ 
#include <torch/torch.h>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/include/ScattererClass.h"

int main() {


  // sea-floor bounds
  int along_track_length  = 100; 
  int across_track_length = 100;

  // scatter-intensity
  int along_track_scatter_density = 10;
  int across_track_scatter_density = 10;

  // setting up coordinates
  auto xpoints = torch::linspace(0, \
                                 across_track_length, \
                                 across_track_length * across_track_scatter_density);
  auto ypoints = torch::linspace(0, \
                                 along_track_length,  \
                                 along_track_length * along_track_scatter_density);

  // creating mesh
  auto mesh_grid = torch::meshgrid({xpoints, ypoints}, "ij");
  auto X = mesh_grid[0]; auto Y = mesh_grid[1];
  X = torch::reshape(X, {1, X.numel()});
  Y = torch::reshape(Y, {1, Y.numel()});
  torch::Tensor Z = torch::zeros({1, Y.numel()});

  // setting up floor coordinates
  torch::Tensor floorScatter_coordinates = torch::cat({X, Y, Z}, 0);
  torch::Tensor floorScatter_reflectivity = torch::ones({3, Y.numel()});

  // initializing object of class
  ScattererClass SeafloorScatter(floorScatter_coordinates,
                                 floorScatter_reflectivity);

  // printing 
  std::cout<<"SeafloorScatter = "<<std::endl;
  std::cout<<SeafloorScatter;

  // returning
  return 0;
}