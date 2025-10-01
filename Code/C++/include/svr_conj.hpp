#pragma once
namespace   svr {
    /*==============================================================================
    y = svr::conj(vector);
    ------------------------------------------------------------------------------*/ 
    template    <typename T>
    auto    conj(const  std::vector<T>&    input_vector)
    {
        // creating canvas
        auto    canvas      {std::vector<T>(input_vector.size())};

        // calculating conjugates
        std::for_each(canvas.begin(), canvas.end(),
                      [](auto& argx){argx = std::conj(argx);});

        // returning
        return std::move(canvas);
    }
}