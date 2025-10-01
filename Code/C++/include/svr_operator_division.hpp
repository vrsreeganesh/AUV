#pragma once
/*==============================================================================
element-wise division with scalars
------------------------------------------------------------------------------*/ 
template <typename T>
auto operator/(const    std::vector<T>&     input_vector,
               const    T&                  input_scalar)
{
    // creating canvas
    auto    canvas      {input_vector};

    // filling canvas
    std::transform(canvas.begin(), canvas.end(),
                   canvas.begin(),
                   [&input_scalar](const auto& argx){
                        return static_cast<double>(argx) / static_cast<double>(input_scalar);
                   });

    // returning value
    return std::move(canvas);
}
/*==============================================================================
element-wise division with scalars
------------------------------------------------------------------------------*/ 
template <typename T>
auto operator/=(const    std::vector<T>&     input_vector,
                const    T&                  input_scalar)
{
    // creating canvas
    auto    canvas      {input_vector};

    // filling canvas
    std::transform(canvas.begin(), canvas.end(),
                   canvas.begin(),
                   [&input_scalar](const auto& argx){
                        return static_cast<double>(argx) / static_cast<double>(input_scalar);
                   });

    // returning value
    return std::move(canvas);
}
/*==============================================================================
element-wise with matrix
------------------------------------------------------------------------------*/ 
template    <typename   T>
auto    operator/(const std::vector<std::vector<T>>&    input_matrix,
                  const T                               scalar)
{
    // fetching matrix-dimensions
    const   auto&   num_rows_matrix     {input_matrix.size()};
    const   auto&   num_cols_matrix     {input_matrix[0].size()};

    // creating canvas
    auto    canvas      {std::vector<std::vector<T>>(
        num_rows_matrix,
        std::vector<T>(num_cols_matrix)
    )};

    // dividing with values
    for(auto    row = 0; row < num_rows_matrix; ++row){
        std::transform(input_matrix[row].begin(),   input_matrix[row].end(),
                       canvas[row].begin(),
                       [&scalar](const  auto& argx){
                            return argx/scalar;
                       });
    }

    // returning values
    return std::move(canvas);
}
