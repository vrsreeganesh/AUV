#pragma once
/*==============================================================================
Dependencies
------------------------------------------------------------------------------*/ 
#include <vector>       // for vectors
#include <algorithm>    // for std::transform

/*==============================================================================
y = abs(vector)
------------------------------------------------------------------------------*/ 
template <typename T>
auto abs(const  std::vector<T>&     input_vector)
{
    // creating canvas
    auto    canvas      {input_vector};

    // calculating abs
    std::transform(canvas.begin(),
                   canvas.end(),
                   canvas.begin(),
                   [](auto& argx){return std::abs(argx);});

    // returning 
    return std::move(canvas);
}
/*==============================================================================
y = abs(matrix)
------------------------------------------------------------------------------*/ 
template <typename T>
auto abs(const  std::vector<std::vector<T>>     input_matrix)
{
    // creating canvas
    auto    canvas      {input_matrix};

    // applying element-wise abs
    std::transform(input_matrix.begin(),
                   input_matrix.end(),
                   input_matrix.begin(),
                    [](auto& argx){return std::abs(argx);});

    // returning
    return std::move(canvas);
}