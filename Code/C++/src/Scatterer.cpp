#include <iostream>

// class definition
class Scatterer{
private:
    // private members 
    torch::Tensor coordinates;
    torch::Tensor reflectivity;

public:
  // constructor
  Scatterer(torch::Tensor coordinates = torch::empty({0}),
            torch::Tensor reflectivity = torch::empty({0}))
            : coordinates(coordinates), reflectivity(reflectivity) {}

  // Function to print the coordinates and reflectivity
  void print() const {
      std::cout << "Coordinates: " << coordinates << std::endl;
      std::cout << "Reflectivity: " << reflectivity << std::endl;
  }

  // Overload the << operator for printing Scatterer objects
  friend std::ostream& operator<<(std::ostream& os, const Scatterer& scatterer) {
      os << "Coordinates:\n" << scatterer.coordinates << "\n";
      os << "Reflectivity:\n" << scatterer.reflectivity << "\n";
      return os;
  }

  // // Method to save the Scatterer object using torch::save
  // void save(const std::string& filename) {
  //   // Use torch::save to save a map containing the tensors
  //   std::unordered_map<std::string, torch::Tensor> state_dict;
  //   state_dict["coordinates"] = coordinates;
  //   state_dict["reflectivity"] = reflectivity;

  //   torch::save(state_dict, filename);
  //   std::cout << "Saved to " << filename << std::endl;
  // }

  // // Static method to load a Scatterer object using torch::load
  // static Scatterer load(const std::string& filename) {
  //   // Load the state_dict from file
  //   std::unordered_map<std::string, torch::Tensor> state_dict;
  //   torch::load(state_dict, filename);

  //   // Retrieve the tensors from the state_dict
  //   torch::Tensor coordinates = state_dict["coordinates"];
  //   torch::Tensor reflectivity = state_dict["reflectivity"];

  //   return Scatterer(coordinates, reflectivity);
  // }

};
