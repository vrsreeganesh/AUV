#pragma once
/*==============================================================================
y = scalar * vector
------------------------------------------------------------------------------*/ 
template <typename  T>
auto    operator*(
    const     T                   scalar,
    const     std::vector<T>&     input_vector
)
{
    // creating canvas
    auto    canvas      {input_vector};
    // performing operation
    std::for_each(canvas.begin(),   canvas.end(),
                 [&scalar](auto& argx){argx = argx * scalar;});
    // returning
    return std::move(canvas);
}
/*==============================================================================
y = scalar * vector
------------------------------------------------------------------------------*/ 
template <
    typename T1, 
    typename T2,
    typename = std::enable_if_t<
        !std::is_same_v<std::decay_t<T1>, std::vector<T2> > && 
        std::is_arithmetic_v<T1>
    >
>
auto operator*(const    T1                  scalar, 
               const    vector<T2>&         input_vector)
{
    // fetching final-type 
    using T3 = decltype(std::declval<T1>() * std::declval<T2>());
    
    // creating canvas
    auto    canvas      {std::vector<T3>(input_vector.size())};
    
    // multiplying
    std::transform(
        input_vector.begin(),    input_vector.end(),
        canvas.begin(),
        [&scalar](auto& argx){
            return static_cast<T3>(scalar) * static_cast<T3>(argx);
        });

    // returning
    return std::move(canvas);
}
/*==============================================================================
y = scalar * vecotor (subset init)
------------------------------------------------------------------------------*/
template    <
    typename T,
    typename = std::enable_if_t<
        std::is_floating_point_v<T>
    >
>
auto    operator*(
    const   std::complex<T>     scalar,
    const   std::vector<T>&     input_vector
)
{
    // creating canvas
    auto    canvas      {std::vector<std::complex<T>>(
        input_vector.size()
    )};

    // copying to canvas
    std::transform(
        input_vector.begin(),   input_vector.end(),
        canvas.begin(),
        [&scalar](const    auto&   argx){
            return scalar * static_cast<std::complex<T>>(argx);
        }
    );

    // moving it back
    return std::move(canvas);
}
/*==============================================================================
y = vector * scalar
------------------------------------------------------------------------------*/ 
template <typename T>
auto    operator*(const     std::vector<T>&     input_vector,
                  const     T                   scalar)
{
    // creating canvas
    auto    canvas      {input_vector};
    // multiplying
    std::for_each(canvas.begin(), canvas.end(),
                  [&scalar](auto& argx){
                    argx = argx * scalar;
                  });
    // returning
    return std::move(canvas);
}
/*==============================================================================
y = vector * vector
------------------------------------------------------------------------------*/ 
template <typename T>
auto operator*(const std::vector<T>&    input_vector_A,
               const std::vector<T>&    input_vector_B)
{
    // throwing error: size-desparity
    if (input_vector_A.size() != input_vector_B.size())     {std::cerr << "operator*: size disparity \n";}

    // creating canvas
    auto    canvas      {input_vector_A};

    // element-wise multiplying
    std::transform(input_vector_B.begin(), input_vector_B.end(),
                   canvas.begin(),
                   canvas.begin(),
                   [](const auto& argx, const auto& argy){
                        return argx * argy;
                   });

    // moving it back
    return std::move(canvas);
}
/*==============================================================================
y = vecotr * vector
------------------------------------------------------------------------------*/ 
template <
    typename T1, 
    typename T2,
    typename    =   std::enable_if_t<
        std::is_arithmetic_v<T1>    &&
        std::is_arithmetic_v<T2>
    >
>
auto    operator*(const     std::vector<T1>&    input_vector_A,
                  const     std::vector<T2>&    input_vector_B)
{

    // checking size disparity
    if (input_vector_A.size() != input_vector_B.size())
        std::cerr << "operator*: error, size-disparity \n";

    // figuring out resulting data type
    using   T3    = decltype(std::declval<T1>()   *   std::declval<T2>());

    // creating canvas
    auto    canvas      {std::vector<T3>(input_vector_A.size())};

    // performing multiplications
    std::transform(input_vector_A.begin(),  input_vector_A.end(),
                   input_vector_B.begin(),
                   canvas.begin(),
                   [](const     auto&       argx,
                      const     auto&       argy){
                        return static_cast<T3>(argx)    *   static_cast<T3>(argy);
                   });
    
    // returning
    return std::move(canvas);

}
/*==============================================================================
y = vector * complex_vector
------------------------------------------------------------------------------*/ 
template    <
    typename    T, 
    typename    =   std::enable_if_t<
        std::is_floating_point_v<T>
    >
>
auto    operator*(
    const   std::vector<T>&                 input_vector_A,
    const   std::vector<std::complex<T>>&   input_vector_B
)
{
    // checking size issue
    if (input_vector_A.size() != input_vector_B.size())
        throw std::runtime_error(
            "FILE: svr_operator_star.hpp | FUNCTION: operator* | REPORT: error disparity in the two input-vectors"
        );

    // creating canvas
    auto    canvas      {std::vector<std::complex<T>>(  input_vector_A.size()   )};

    // filling up the canvas
    std::transform(
        input_vector_B.begin(), input_vector_B.end(),
        input_vector_A.begin(),
        canvas.begin(),
        [](const    auto&   argx,   const   auto&   argy){
            return argx + static_cast<std::complex<T>>(argy);
        }
    );

    // moving it back
    return std::move(canvas);
}
/*==============================================================================
y = complex_vector * vector
------------------------------------------------------------------------------*/ 
template    <
    typename    T,
    typename    =   std::enable_if_t<
        std::is_floating_point_v<T>
    >
>
auto    operator*(
    const   std::vector<std::complex<T>>&   input_vector_A,
    const   std::vector<T>&                 input_vector_B
)
{
    // enforcing size
    if (input_vector_A.size() != input_vector_B.size())
        throw std::runtime_error(
            "FILE: svr_operator_star.hpp | FUNCTION: operator* overload"
        );

    // creating canvas
    auto    canvas      {std::vector<std::complex<T>>(input_vector_A.size())};
    
    // filling values
    std::transform(
        input_vector_A.begin(), input_vector_A.end(),
        input_vector_B.begin(),
        canvas.begin(),
        [](const    auto&   argx, const auto&   argy){
            return argx * static_cast<std::complex<T>>(argy);
        }
    );

    // returning 
    return std::move(canvas);
}
/*==============================================================================
y = complex-vector * complex-vector
------------------------------------------------------------------------------*/
template    <
    typename    T,
    typename    =   std::enable_if_t<
        std::is_floating_point_v<T>
    >
>
auto    operator*(
    const   std::vector<std::complex<T>>    input_vector_A,
    const   std::vector<std::complex<T>>    input_vector_B
)
{
    // checking size
    if (input_vector_A.size() != input_vector_B.size())
        throw std::runtime_error(
            "FILE: svr_operator_star.hpp | FUNCTION: operator*(complex-vector, complex-vector)"
        );

    // creating canvas
    auto    canvas      {std::vector<std::complex<T>>(input_vector_A.size())};

    // filling canvas
    std::transform(
        input_vector_A.begin(),     input_vector_A.end(),
        input_vector_B.begin(),
        canvas.begin(),
        [](const    auto&   argx, const auto&   argy){
            return argx * argy;
        }
    );

    // returning values
    return std::move(canvas);
}
// scalar * matrix =============================================================
template <typename T>
auto operator*(const T                              scalar, 
               const std::vector<std::vector<T>>&   inputMatrix)
{
    std::vector<std::vector<T>> temp    {inputMatrix};
    for(int i = 0; i<inputMatrix.size(); ++i){
        std::transform(inputMatrix[i].begin(),
                       inputMatrix[i].end(),
                       temp[i].begin(),
                       [&scalar](T x){return scalar * x;});
    }
    return std::move(temp);
}
/*==============================================================================
y = matrix * scalar
------------------------------------------------------------------------------*/
template <typename T>
auto    operator*(const     std::vector<std::vector<T>>&    input_matrix,
                  const     T                               scalar)
{
    // fetching matrix dimensions
    const   auto&   num_rows_matrix     {input_matrix.size()};
    const   auto&   num_cols_matrix     {input_matrix[0].size()};

    // creating canvas
    auto    canvas      {std::vector<std::vector<T>>(
        num_rows_matrix,
        std::vector<T>(num_cols_matrix)
    )};

    // storing the values
    for(auto row = 0; row < num_rows_matrix; ++row)
        std::transform(input_matrix[row].begin(),   input_matrix[row].end(),
                       canvas[row].begin(),
                       [&scalar](const  auto&   argx){
                            return argx * scalar;
                       });

    // returning
    return std::move(canvas);
}
/*==============================================================================
y = matrix * matrix
------------------------------------------------------------------------------*/ 
template <typename T>
auto operator*(const std::vector<std::vector<T>>& A,
               const std::vector<std::vector<T>>& B) -> std::vector<std::vector<T>>
{
    // Case 1: element-wise multiplication
    if (A.size() == B.size() && A[0].size() == B[0].size()) {
        std::vector<std::vector<T>> C(A.size(), std::vector<T>(A[0].size()));
        for (std::size_t row = 0; row < A.size(); ++row) {
            std::transform(A[row].begin(), A[row].end(),
                           B[row].begin(),
                           C[row].begin(),
                           [](const auto& x, const auto& y){ return x * y; });
        }
        return C;
    }

    // Case 2: broadcast column vector
    else if (A.size() == B.size() && B[0].size() == 1) {
        std::vector<std::vector<T>> C(A.size(), std::vector<T>(A[0].size()));
        for (std::size_t row = 0; row < A.size(); ++row) {
            std::transform(A[row].begin(), A[row].end(),
                           C[row].begin(),
                           [&](const auto& x){ return x * B[row][0]; });
        }
        return C;
    }

    // case 3: when second matrix contains just one row
    // case 4: when first matrix is just one column
    // case 5: when second matrix  is just one column

    // Otherwise, invalid
    else {
        throw std::runtime_error("operator* dimension mismatch");
    }
}
/*==============================================================================
y = scalar * matrix
------------------------------------------------------------------------------*/ 
template <typename T1, typename T2>
auto operator*(const T1                             scalar, 
               const std::vector<std::vector<T2>>&  inputMatrix)
{
    std::vector<std::vector<T2>> temp    {inputMatrix};
    for(int i = 0; i<inputMatrix.size(); ++i){
        std::transform(inputMatrix[i].begin(),
                       inputMatrix[i].end(),
                       temp[i].begin(),
                       [&scalar](T2 x){return static_cast<T2>(scalar) * x;});
    }
    return temp;
}
/*==============================================================================
matrix-multiplication
------------------------------------------------------------------------------*/ 
template <typename T1, typename T2>
auto matmul(const std::vector<std::vector<T1>>& matA,
            const std::vector<std::vector<T2>>& matB)
{

    // throwing error
    if (matA[0].size() != matB.size())      {std::cerr << "dimension-mismatch \n";}

    // getting result-type
    using ResultType    = decltype(std::declval<T1>() * std::declval<T2>() + \
                                   std::declval<T1>() * std::declval<T2>() );

    // creating aliasses
    auto finalnumrows  {matA.size()};
    auto finalnumcols  {matB[0].size()};

    // creating placeholder
    auto rowcolproduct  = [&](auto rowA, auto colB){
        ResultType temp   {0};
        for(int i = 0; i < matA.size(); ++i)    {temp += static_cast<ResultType>(matA[rowA][i]) + static_cast<ResultType>(matB[i][colB]);}
        return temp;
    };

    // producing row-column combinations
    std::vector<std::vector<ResultType>> finaloutput(finalnumrows, std::vector<ResultType>(finalnumcols));
    for(int row = 0; row < finalnumrows; ++row){for(int col = 0; col < finalnumcols; ++col){finaloutput[row][col]   = rowcolproduct(row, col);}}

    // returning 
    return finaloutput;
}
/*==============================================================================
y = matrix * vector
------------------------------------------------------------------------------*/ 
template    <
    typename T,
    typename    =   std::enable_if_t<
        std::is_arithmetic_v<T>
    >
>
auto operator*(const    std::vector<std::vector<T>>&     input_matrix,
               const    std::vector<T>&                  input_vector)
{
    // fetching dimensions
    const   auto&   num_rows_matrix     {input_matrix.size()};
    const   auto&   num_cols_matrix     {input_matrix[0].size()};
    const   auto&   num_rows_vector     {1};
    const   auto&   num_cols_vector     {input_vector.size()};
    const   auto&   max_num_rows        {num_rows_matrix > num_rows_vector ?\
                                         num_rows_matrix : num_rows_vector};
    const   auto&   max_num_cols        {num_cols_matrix > num_cols_vector ?\
                                         num_cols_matrix : num_cols_vector};

    // creating canvas
    auto    canvas      {std::vector<std::vector<T>>(
        max_num_rows,
        std::vector<T>(max_num_cols, 0)
    )};

    // multiplying column matrix with row matrix
    if (num_cols_matrix == 1 && num_rows_vector == 1){

        // writing to canvas
        for(auto    row = 0; row < max_num_rows; ++row)
            for(auto    col = 0; col < max_num_cols; ++col)
                canvas[row][col]    =   input_matrix[row][0]    *   input_vector[col];
    }
    /*==========================================================================
    Multiplying each row with the input-vector
    --------------------------------------------------------------------------*/
    else if(
        num_cols_matrix == num_cols_vector  &&
        num_rows_vector == 1
    )
    {
        // writing to canvas
        for(auto row = 0; row < max_num_rows; ++row)
            std::transform(
                input_matrix[row].begin(),  input_matrix[row].end(),
                input_vector.begin(),
                canvas[row].begin(),
                [](const auto& argx, const auto& argy){return argx * argy;}
            );
    }
    else
    {
        std::cerr << "Operator*: [matrix, vector] | not implemented \n";
    }

    // returning
    return std::move(canvas);

}
/*==============================================================================
complex-matrix * vector
------------------------------------------------------------------------------*/
template <
    typename T,
    typename    =   std::enable_if_t<
        std::is_floating_point_v<T>
    >
>
auto    operator*(
    const   std::vector<std::vector<std::complex<T>>>&  input_matrix,
    const   std::vector<T>&                             input_vector
)
{
    // fetching dimensions
    const   auto    num_rows_matrix     {input_matrix.size()};
    const   auto    num_cols_matrix     {input_matrix[0].size()};
    const   auto    num_rows_vector     {static_cast<std::size_t>(1)};
    const   auto    num_cols_vector     {input_vector.size()};

    // throwing an error
    if (num_cols_matrix != num_cols_vector)
        throw std::runtime_error(
            "FILE: svr_operator_star.hpp | FUNCTION: operator*(complex-matrix, vector)"
        );

    // creating canvas
    auto    canvas      {std::vector<std::vector<std::complex<T>>>(
        num_rows_matrix,
        std::vector<std::complex<T>>(num_cols_matrix)
    )};

    // performing operations 
    for(auto row = 0; row < num_rows_matrix; ++row)
        std::transform(
            input_matrix[row].begin(),  input_matrix[row].end(),
            input_vector.begin(),
            canvas[row].begin(),
            [](const    auto&   argx,   const   auto&   argy){
                return argx * static_cast<std::complex<T>>(argy);
            }
        );
    
    // returning the final output
    return std::move(canvas);

}
/*==============================================================================
martix * complex-vector
------------------------------------------------------------------------------*/
template    <
    typename    T,
    typename    =   std::enable_if_t<
        std::is_floating_point_v<T>
    >
>
auto    operator*(
    const   std::vector<std::vector<T>>&    input_matrix,
    const   std::vector<std::complex<T>>&   input_vector
)
{
    // fetching dimensions
    const   auto    num_rows_matrix     {input_matrix.size()};
    const   auto    num_cols_matrix     {input_matrix[0].size()};
    const   auto    num_rows_vector     {static_cast<std::size_t>(1)};
    const   auto    num_cols_vector     {input_vector.size()};

    // fetching dimension mismatch
    if (num_cols_matrix != num_cols_vector)
        throw std::runtime_error(
            "FILE: svr_operator_star.hpp | FUNCTION: operator*(input-matrix, complex-vector)"
        );
    
    // creating canvas
    auto    canvas      {std::vector<std::vector<std::complex<T>>>(
        num_rows_matrix,
        std::vector<std::complex<T>>(
            num_cols_matrix
        )
    )};

    // filling the values
    for(auto row = 0; row < num_rows_matrix; ++row)
        std::transform(
            input_matrix[row].begin(),  input_matrix[row].end(),
            input_vector.begin(),
            canvas[row].begin(),
            [](const    auto&   argx,   const   auto&   argy){
                return static_cast<std::complex<T>>(argx)  *   argy;
            }
        );

    // returning final-output
    return std::move(canvas);
}
/*==============================================================================
scalar operators
------------------------------------------------------------------------------*/
auto operator*(const std::complex<double>   complexscalar, 
               const double                 doublescalar){
    return complexscalar * static_cast<std::complex<double>>(doublescalar);
}
auto operator*(const double                 doublescalar, 
               const std::complex<double>   complexscalar){
    return complexscalar * static_cast<std::complex<double>>(doublescalar);
}
auto operator*(const std::complex<double>   complexscalar, 
               const int                    scalar){
    return complexscalar * static_cast<std::complex<double>>(scalar);
}
auto operator*(const int                    scalar, 
               const std::complex<double>   complexscalar){
    return complexscalar * static_cast<std::complex<double>>(scalar);
}