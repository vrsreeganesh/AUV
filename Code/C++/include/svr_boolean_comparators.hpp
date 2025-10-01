#pragma once
/*==============================================================================
------------------------------------------------------------------------------*/ 
template <typename T, typename U>
auto operator<(const    std::vector<T>&     input_vector,
               const    U                   scalar)
{
    // creating canvas
    auto    canvas      {std::vector<bool>(input_vector.size())};

    // transforming
    std::transform(input_vector.begin(), input_vector.end(),
                   canvas.begin(),
                   [&scalar](const auto& argx){
                        return argx < static_cast<T>(scalar);
                   });

    // returning 
    return std::move(canvas);
}
/*==============================================================================
------------------------------------------------------------------------------*/ 
template <typename T, typename U>
auto operator<=(const    std::vector<T>&     input_vector,
                const    U                   scalar)
{
    // creating canvas
    auto    canvas      {std::vector<bool>(input_vector.size())};

    // transforming
    std::transform(input_vector.begin(), input_vector.end(),
                   canvas.begin(),
                   [&scalar](const auto& argx){
                        return argx <= static_cast<T>(scalar);
                   });

    // returning 
    return std::move(canvas);
}
// =============================================================================
template <typename T, typename U>
auto operator>(const    std::vector<T>&     input_vector,
               const    U                   scalar)
{
    // creating canvas
    auto    canvas      {std::vector<bool>(input_vector.size())};

    // transforming
    std::transform(input_vector.begin(), input_vector.end(),
                   canvas.begin(),
                   [&scalar](const auto& argx){
                        return argx > static_cast<T>(scalar);
                   });

    // returning 
    return std::move(canvas);
}
/*==============================================================================
------------------------------------------------------------------------------*/ 
template <typename T, typename U>
auto operator>=(const    std::vector<T>&     input_vector,
                const    U                   scalar)
{
    // creating canvas
    auto    canvas      {std::vector<bool>(input_vector.size())};

    // transforming
    std::transform(input_vector.begin(), input_vector.end(),
                   canvas.begin(),
                   [&scalar](const auto& argx){
                        return argx >= static_cast<T>(scalar);
                   });

    // returning 
    return std::move(canvas);
}