#pragma once
/*==============================================================================
y = vector - scalar
------------------------------------------------------------------------------*/
template <typename T>
auto operator-(const std::vector<T>&    a, 
               const T                  scalar){
    std::vector<T> temp(a.size());
    std::transform(a.begin(),
                   a.end(),
                   temp.begin(),
                   [scalar](T x){return (x - scalar);});
    return std::move(temp);
}
/*==============================================================================
y = vector - vector
------------------------------------------------------------------------------*/
template <typename T>
auto operator-(const    std::vector<T>&     input_vector_A,
               const    std::vector<T>&     input_vector_B)
{
    // throwing error
    if (input_vector_A.size() != input_vector_B.size())
        std::cerr << "operator-(vector, vector): size disparity\n";

    // creating canvas 
    const   auto&   num_cols    {input_vector_A.size()};
    auto            canvas      {std::vector<T>()};

    // peforming operations
    std::transform(input_vector_A.begin(),  input_vector_A.begin(),
                   input_vector_B.begin(),
                   canvas.begin(),
                    [](const auto& argx, const auto& argy){
                        return argx - argy;
                    });

    // return
    return std::move(canvas);
}
/*==============================================================================
y = matrix - matrix
------------------------------------------------------------------------------*/
template <typename T>
auto operator-(const    std::vector<std::vector<T>>&     input_matrix_A,
               const    std::vector<std::vector<T>>&     input_matrix_B)
{
    // fetching dimensions
    const   auto&   num_rows_A  {input_matrix_A.size()};
    const   auto&   num_cols_A  {input_matrix_A[0].size()};
    const   auto&   num_rows_B  {input_matrix_B.size()};
    const   auto&   num_cols_B  {input_matrix_B[0].size()};

    // creating canvas
    auto    canvas      {std::vector<std::vector<T>>()};

    // if both matrices are of equal dimensions
    if (num_rows_A == num_rows_B && num_cols_A == num_cols_B)
    {
        // copying one to the canvas
        canvas  = input_matrix_A;

        // subtracting
        for(auto row = 0; row < num_rows_B; ++row)
            std::transform(canvas[row].begin(), canvas[row].end(),
                           input_matrix_B[row].begin(),
                           canvas[row].begin(),
                           [](auto& argx, const auto& argy){
                                return argx - argy;
                           });
    }
    // column broadcasting (case 1)
    else if(num_rows_A == num_rows_B && num_cols_B == 1)
    {
        // copying canvas
        canvas  =   input_matrix_A;

        // substracting
        for(auto row = 0; row < num_rows_A; ++row){
            std::transform(canvas[row].begin(), canvas[row].end(),
                           canvas[row].begin(),
                           [&input_matrix_B,
                            &row](auto& argx){
                                return argx - input_matrix_B[row][0];
                           });
        }
    }
    else{
        std::cerr << "operator-: not implemented for this case \n";
    }

    // returning 
    return std::move(canvas);    
}


