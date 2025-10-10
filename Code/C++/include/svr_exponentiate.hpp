#pragma once

namespace   svr     {
    /*==============================================================================
    y = exp(vector)
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
    /*==============================================================================
    y = exp(matrix)
    ------------------------------------------------------------------------------*/ 
    template    <
        typename    sourceType,
        typename    destinationType,
        typename    =   std::enable_if_t<
            std::is_arithmetic_v<sourceType>
        >
    >
    auto    exp(
        const   std::vector<std::vector<sourceType>>     input_matrix
    )
    {
        // fetching dimensions
        const   auto&   num_rows    {input_matrix.size()};
        const   auto&   num_cols    {input_matrix[0].size()};

        // creating canvas
        auto    canvas      {std::vector<std::vector<destinationType>>(
            num_rows,
            std::vector<destinationType>(num_cols)
        )};

        // writing to each entry
        for(auto row = 0; row < num_rows; ++row)
            std::transform(
                input_matrix[row].begin(),  input_matrix[row].end(),
                canvas[row].begin(),
                [](const    auto&   argx){
                    return std::exp(argx);
                }
            );

        // returning
        return std::move(canvas);
    }
    /*==========================================================================
    Aim: Exponentiating complex matrices with general floating types
    --------------------------------------------------------------------------*/ 
    template    <
        typename    T,
        typename    =   std::enable_if_t<
            std::is_floating_point_v<T>
        >
    >
    auto    exp(
        const   std::vector<std::vector<std::complex<T>>>   input_matrix
    )
    {
        // fetching dimensions
        const   auto&   num_rows    {input_matrix.size()};
        const   auto&   num_cols    {input_matrix[0].size()};

        // creating canvas
        auto    canvas      {std::vector<std::vector<std::complex<T>>>(
            num_rows,
            std::vector<std::complex<T>>(num_cols)
        )};

        // writing to each entry
        for(auto row = 0; row < num_rows; ++row)
            std::transform(
                input_matrix[row].begin(),  input_matrix[row].end(),
                canvas[row].begin(),
                [](const    auto&   argx){
                    return std::exp(argx);
                }
            );

        // returning
        return std::move(canvas);
    }

}   