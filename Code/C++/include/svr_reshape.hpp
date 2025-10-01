#pragma once

/*==============================================================================
reshaping a matrix into another matrix
------------------------------------------------------------------------------*/ 
template <std::size_t M, std::size_t N, typename T>
auto reshape(const  std::vector<std::vector<T>>&     input_matrix){

    // verifying size stuff
    if (M*N != input_matrix.size() * input_matrix[0].size())
        std::cerr << "Dimensions are quite different\n";
    
    // creating canvas
    auto    canvas      {std::vector<std::vector<T>>(
        M, std::vector<T>(N, (T)0)
    )};

    // writing to canvas
    size_t    tid             {0};
    size_t    target_row      {0};
    size_t    target_col      {0};
    for(auto row = 0; row<input_matrix.size(); ++row){
        for(auto col = 0; col < input_matrix[0].size(); ++col){
            tid             =   row * input_matrix[0].size() + col;
            target_row      =   tid/N;
            target_col      =   tid%N;
            canvas[target_row][target_col]      =   input_matrix[row][col];
        }
    }
        
    // moving it back
    return std::move(canvas);
}
/*==============================================================================
reshaping a matrix into a vector
------------------------------------------------------------------------------*/ 
template<std::size_t M, typename T>
auto reshape(const std::vector<std::vector<T>>& input_matrix){
    
    // checking element-count validity
    if (M != input_matrix.size() * input_matrix[0].size())
        std::cerr << "Number of elements differ\n";

    // creating canvas
    auto    canvas      {std::vector<T>(M, 0)};

    // filling canvas
    for(auto row = 0; row < input_matrix.size(); ++row)
        for(auto col = 0; col < input_matrix[0].size(); ++col)
            canvas[row * input_matrix.size() + col] = input_matrix[row][col];

    // moving it back
    return std::move(canvas);
}
/*==============================================================================
Matrix to matrix
------------------------------------------------------------------------------*/ 
template<typename T>
auto reshape(const std::vector<std::vector<T>>& input_matrix,
             const std::size_t                  M,
             const std::size_t                  N){
    
    // checking element-count validity
    if ( M * N != input_matrix.size() * input_matrix[0].size())
        std::cerr << "Number of elements differ\n";

    // creating canvas
    auto    canvas      {std::vector<std::vector<T>>(
        M, std::vector<T>(N, (T)0)
    )};

    // writing to canvas
    size_t    tid             {0};
    size_t    target_row      {0};
    size_t    target_col      {0};
    for(auto row = 0; row<input_matrix.size(); ++row){
        for(auto col = 0; col < input_matrix[0].size(); ++col){
            tid             =   row * input_matrix[0].size() + col;
            target_row      =   tid/N;
            target_col      =   tid%N;
            canvas[target_row][target_col]      =   input_matrix[row][col];
        }
    }

    // moving it back
    return std::move(canvas);
}
/*==============================================================================
converting a matrix into a vector
------------------------------------------------------------------------------*/ 
template<typename T>
auto reshape(const std::vector<std::vector<T>>& input_matrix,
             const size_t                       M){
    
    // checking element-count validity
    if (M != input_matrix.size() * input_matrix[0].size())
        std::cerr << "Number of elements differ\n";

    // creating canvas
    auto    canvas      {std::vector<T>(M, 0)};

    // filling canvas
    for(auto row = 0; row < input_matrix.size(); ++row)
        for(auto col = 0; col < input_matrix[0].size(); ++col)
            canvas[row * input_matrix.size() + col] = input_matrix[row][col];

    // moving it back
    return std::move(canvas);
}
