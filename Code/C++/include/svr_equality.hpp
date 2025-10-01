#pragma once
/*==============================================================================
------------------------------------------------------------------------------*/ 
template <typename T, typename U>
auto operator==(const   std::vector<T>&     input_vector,
                const   U&                  scalar)
{
    // setting up canvas
    auto    canvas      {std::vector<bool>(input_vector.size())};

    // writing to canvas
    std::transform(input_vector.begin(), input_vector.end(),
                   canvas.begin(),
                   [&scalar](const auto& argx){
                        return argx == scalar;
                   });

    // returning
    return std::move(canvas);
}