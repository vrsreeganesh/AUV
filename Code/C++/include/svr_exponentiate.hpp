#pragma once
/*==============================================================================
y = abs(vector)
------------------------------------------------------------------------------*/ 
template <typename T>
auto exp(const  std::vector<T>&     input_vector)
{
    // creating canvas
    auto    canvas      {input_vector};

    // transforming
    std::transform(canvas.begin(), canvas.end(),
                   canvas.begin(),
                   [](auto& argx){return std::exp(argx);});

    // returning
    return std::move(canvas);
}