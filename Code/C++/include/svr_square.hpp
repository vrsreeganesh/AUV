#pragma once
namespace   svr {
    /*==========================================================================
    Element-wise squaring vector
    --------------------------------------------------------------------------*/ 
    template    <typename T,
                 typename = std::enable_if_t<std::is_arithmetic_v<T>>
                >
    auto    square(const    std::vector<T>&   input_vector)
    {
        // creating canvas
        auto    canvas      {std::vector<T>(input_vector.size())};

        // peforming calculations
        std::transform(input_vector.begin(), input_vector.end(),
                       canvas.begin(),
                        [](const auto& argx){
                                return argx * argx;
                        });
        
        // moving it back
        return std::move(canvas);
    }
    /*==========================================================================
    Element-wise squaring vector (in-place)
    --------------------------------------------------------------------------*/ 
    template    <typename T,
                 typename = std::enable_if_t<std::is_arithmetic_v<T>>
                >
    void    square_inplace(std::vector<T>&   input_vector)
    {
        // performing operations
        std::transform(input_vector.begin(), input_vector.end(),
                       input_vector.begin(),
                       [](auto& argx){
                            return argx * argx;
                       });
    }
    /*==========================================================================
    ELement-wise squaring a matrix
    --------------------------------------------------------------------------*/ 
    template <typename T>
    auto    square(const   std::vector<std::vector<T>>&    input_matrix)
    {
        // fetching dimensions
        const   auto&   num_rows    {input_matrix.size()};
        const   auto&   num_cols    {input_matrix[0].size()};

        // creating canvas
        auto    canvas      {std::vector<std::vector<T>>(
            num_rows,
            std::vector<T>(num_cols, 0)
        )};

        // going through each row
        #pragma omp parallel for
        for(auto row = 0; row < num_rows; ++row)
            std::transform(input_matrix[row].begin(),   input_matrix[row].end(),
                           canvas[row].begin(),
                           [](const auto& argx){
                                return argx * argx;
                           });

        // returning
        return std::move(canvas);
    }
    /*==========================================================================
    Squaring for scalars
    --------------------------------------------------------------------------*/ 
    template <typename T>
    auto square(const   T&  scalar)     {return scalar * scalar;}
}