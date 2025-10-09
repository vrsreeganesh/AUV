#pragma once
/*==============================================================================
calculating norm for vector
------------------------------------------------------------------------------*/ 
template <typename T>
auto norm(const     std::vector<T>&     input_vector)
{
    return std::sqrt(
        std::inner_product(
            input_vector.begin(), input_vector.end(),
            input_vector.begin(),
            (T)0
        )
    );
}
/*==============================================================================
Calculating norm of a complex-vector
------------------------------------------------------------------------------*/ 
template    <>
auto    norm(const  std::vector<std::complex<double>>&      input_vector)
{
    return std::sqrt(
        std::inner_product(
            input_vector.begin(),    input_vector.end(),
            input_vector.begin(),
            static_cast<double>(0),
            std::plus<double>(),
            [](const    auto&   argx,
               const    auto&   argy){
                return static_cast<double>(
                    (argx * std::conj(argy)).real()
                );
            }
        )
    );

}
/*==============================================================================
------------------------------------------------------------------------------*/ 
template <typename T>
auto    norm(const  std::vector<std::vector<T>>&    input_matrix,
             const  std::size_t                     dim)
{
    // creating canvas
    auto    canvas      {std::vector<std::vector<T>>()};
    const   auto&   num_rows_matrix     {input_matrix.size()};
    const   auto&   num_cols_matrix     {input_matrix[0].size()};

    // along dim 0
    if(dim == 0)
    {
        // allocate canvas
        canvas  =   std::vector<std::vector<T>>(
            1,
            std::vector<T>(input_matrix[0].size())
        );

        // performing norm
        auto  accumulate_vector     {std::vector<T>(input_matrix[0].size())};

        // going through each row
        for(auto    row = 0; row < num_rows_matrix; ++row)
        {
            std::transform(input_matrix[row].begin(),   input_matrix[row].end(),
                           accumulate_vector.begin(),
                           accumulate_vector.begin(),
                           [](const auto&   argx, auto& argy){
                                return argx*argx + argy;
                           });
        }

        // calculating element-wise square root
        std::for_each(accumulate_vector.begin(), accumulate_vector.end(),
                      [](auto&  argx){
                            argx = std::sqrt(argx);
                      });

        // moving to the canvas
        canvas[0]   = std::move(accumulate_vector);
    }
    else if (dim == 1)
    {
        // allocating space in the canvas
        canvas      = std::vector<std::vector<T>>(
            input_matrix[0].size(),
            std::vector<T>(1, 0)
        );

        // going through each column
        for(auto row = 0; row < num_cols_matrix; ++row){
            canvas[row][0]  = norm(input_matrix[row]);
        }

    }
    else
    {
        std::cerr << "norm(matrix, dim): dimension operation not defined \n";
    }

    // returning
    return std::move(canvas);
}



/*
Templates to create
    -   matrix and norm-axis
    -   axis instantiated std::vector<T>
*/ 