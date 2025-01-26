#include <torch/torch.h>

// function to column normalize a tensor
torch::Tensor fColumnNormalize(torch::Tensor inputTensor){
  // finding norm
  torch::Tensor inputTensorNorm = torch::linalg::norm(inputTensor,    //A
                                                      2,              // order
                                                      0,              // dimension
                                                      true,           // keep-dims
                                                      torch::kFloat); // d-type       

  // dividing the input by the norm
  torch::Tensor outputTensor = inputTensor/inputTensorNorm;

  // returning result tensor
  return outputTensor;
}

