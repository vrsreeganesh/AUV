namespace   svr {
    /*==========================================================================
    element-wise flooring of a vector-contents
    --------------------------------------------------------------------------*/ 
    template    <typename   T>
    auto    floor(const     std::vector<T>&     input_vector)
    {
        // creating canvas
        auto    canvas      {std::vector<T>(input_vector.size())};

        // filling the canvas
        std::transform(input_vector.begin(),  input_vector.end(),
                       canvas.begin(),
                       [](const auto&   argx){
                            return std::floor(argx);
                       });

        // returning
        return std::move(canvas);
    }
    /*==========================================================================
    element-wise flooring of a vector-contents (in-place)
    --------------------------------------------------------------------------*/ 
    template    <typename   T>
    auto    floor_inplace(std::vector<T>&       input_vector)
    {
        // rewriting the contents
        std::transform(input_vector.begin(),    input_vector.end(),
                       input_vector.begin(),
                       [](auto& argx){
                            return std::floor(argx);
                       });
    }
    /*==========================================================================
    element-wise flooring of matrix-contents
    --------------------------------------------------------------------------*/ 
    template    <typename T>
    auto    floor(const std::vector<std::vector<T>>&    input_matrix)
    {
        // fetching dimensions
        const   auto&   num_rows_matrix     {input_matrix.size()};
        const   auto&   num_cols_matrix     {input_matrix[0].size()};

        // creating canvas
        auto    canvas      {std::vector<std::vector<T>>(
            num_rows_matrix,
            std::vector<T>(num_cols_matrix)
        )};

        // writing contents
        for(auto row = 0; row < num_rows_matrix; ++row)
            std::transform(input_matrix[row].begin(),   input_matrix[row].end(),
                           canvas[row].begin(),
                           [](const auto&   argx){
                                return std::floor(argx);
                           });

        // returning contents
        return std::move(canvas);

    }
    /*==========================================================================
    element-wise flooring of matrix-contents (in-place)   
    --------------------------------------------------------------------------*/
    template    <typename T>
    auto    floor_inplace(std::vector<std::vector<T>>&  input_matrix)
    {
        // performing operations
        for(auto row = 0; row < input_matrix.size(); ++row)
            std::transform(input_matrix[row].begin(),   input_matrix[row].end(),
                           input_matrix[row].begin(),
                           [](auto& argx){
                                return  std::floor(argx);
                           });
    }
}