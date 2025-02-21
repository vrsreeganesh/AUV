
// including headerfiles
#include <stdexcept>
#include <torch/torch.h>

#pragma once

// adding hills as sea-floor
void fCreateHills(torch::Tensor& mean_vector,               \
                  torch::Tensor& dimension_vector,          \
                  torch::Tensor& points2D){
    
    // creating hills
    int num_hills = mean_vector[0].numel();

    // the final heights of the points
    torch::Tensor finalHeights = \
        torch::zeros({1, points2D[0].numel()}).to(DATATYPE);

    // creating hills one by one
    for(int hill_index = 0; hill_index < num_hills; ++hill_index){

        // changing origin
        torch::Tensor normalizedx =                             \
            (PI/2) *                                            \
            (points2D[0] - mean_vector[0].index({hill_index}))  \
            /dimension_vector[0].index({hill_index});
        torch::Tensor normalizedy =                             \
            (PI/2) *                                            \
            (points2D[1] - mean_vector[1].index({hill_index}))  \
            /dimension_vector[1].index({hill_index});

        // building height values
        torch::Tensor heightx =                                 \
            torch::mul(torch::cos(normalizedx),                 \
                       torch::exp(torch::abs(normalizedx)/10));
        torch::Tensor heighty = \
            torch::mul(torch::cos(normalizedy),                 \
                       torch::exp(torch::abs(normalizedy)/10));
        torch::Tensor heights = \
            dimension_vector[2].index({hill_index}) * heightx * heighty;

        // boolean mask for indices outside hill
        auto maskx_piby2        = (normalizedx > PI/2);
        auto maskx_minuspiby2   = (normalizedx < -PI/2);
        auto masky_piby2        = (normalizedy > PI/2);
        auto masky_minuspiby2   = (normalizedy < -PI/2);
        
        // assigning zeros to points outside the hill positive cycle
        heights.index_put_({maskx_piby2},       0);
        heights.index_put_({maskx_minuspiby2},  0);
        heights.index_put_({masky_piby2},       0);
        heights.index_put_({masky_minuspiby2},  0);

        // maxing hill-amplitudes for now
        torch::Tensor appendedHeights = \
            torch::cat({heights.reshape({1, heights.numel()}), finalHeights}, 0);
        
        // finalHeights = std::get<0>(torch::max(appendedHeights, 0, true));
        finalHeights = torch::sum(appendedHeights, 0, true);
    }

    // appending to the final points
    points2D = torch::cat({points2D, finalHeights}, 0);
}