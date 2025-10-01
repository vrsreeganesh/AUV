#pragma once
namespace   svr {
    /*==============================================================================
    y = tan-inverse(input_vector_A/input_vector_B)
    ------------------------------------------------------------------------------*/ 
    template <typename T>
    auto atan2(const    std::vector<T>      input_vector_A,
               const    std::vector<T>      input_vector_B)
    {
        // throw error
        if (input_vector_A.size() != input_vector_B.size())
            std::cerr << "atan2: size disparity\n";

        // create canvas
        auto    canvas      {std::vector<T>(input_vector_A.size(), 0)};

        // performing element-wise atan2 calculation
        std::transform(input_vector_A.begin(), input_vector_A.end(),
                       input_vector_B.begin(),
                       canvas.begin(),
                       [](const     auto&   arg_a,
                          const     auto&   arg_b){
                            
                            return std::atan2(arg_a, arg_b);
                       });

        // moving things back
        return std::move(canvas);
    }
    /*==============================================================================
    y = tan-inverse(a/b)
    ------------------------------------------------------------------------------*/ 
    template <typename T>
    auto atan2(T    scalar_A,
               T    scalar_B)
    {
        return std::atan2(scalar_A, scalar_B);
    }
}
