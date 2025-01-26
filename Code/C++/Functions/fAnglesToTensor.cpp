#include <torch/torch.h>
// function: angles to vector
torch::Tensor fAnglesToTensor(float azimuthal_angle,
                              float elevation_angle)
{
  // calculating tensor
  torch::Tensor coordinateTensor = torch::tensor({cos(elevation_angle) * cos(azimuthal_angle),
                                                  cos(elevation_angle) * sin(azimuthal_angle),
                                                  sin(elevation_angle)}).view({3,1});

  // returning value
  return coordinateTensor;
}