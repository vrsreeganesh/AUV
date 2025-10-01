#pragma once
/*==============================================================================
y = cos(input_vector)
------------------------------------------------------------------------------*/ 
template <typename T>
auto cos(const  std::vector<T>&     input_vector)
{
    // created canvas
    auto    canvas      {input_vector};

    // calling the function
    std::transform(input_vector.begin(), input_vector.end(),
                   canvas.begin(),
                   [](auto& argx){return std::cos(argx);});

    // returning the output
    return std::move(canvas);
}
/*==============================================================================
y = cosd(input_vector)
------------------------------------------------------------------------------*/ 
template <typename T>
auto cosd(const std::vector<T>     input_vector)
{
    // created canvas
    auto    canvas      {input_vector};

    // calling the function
    std::transform(input_vector.begin(),
                   input_vector.end(),
                   input_vector.begin(),
                   [](const auto& argx){return std::cos(argx * 180.00/std::numbers::pi);});

    // returning the output
    return std::move(canvas);
}