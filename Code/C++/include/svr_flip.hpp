#pragma once
namespace   svr {
    /*==============================================================================
    Mirror image of a vector
    ------------------------------------------------------------------------------*/ 
    template <typename T>
    auto    fliplr(const    std::vector<T>&     input_vector)
    {
        // creating canvas
        auto    canvas      {input_vector};

        // rewriting
        std::reverse(canvas.begin(), canvas.end());

        // returning
        return std::move(canvas);
    }
}