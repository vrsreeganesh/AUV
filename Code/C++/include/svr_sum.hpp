#pragma once
/*==============================================================================
------------------------------------------------------------------------------*/ 
template <std::size_t axis, typename T>
auto sum(const  std::vector<T>&    input_vector) -> std::enable_if_t<axis == 0, std::vector<T>>
{
    // returning the input as is
    return input_vector;
}
/*==============================================================================
------------------------------------------------------------------------------*/ 
template <std::size_t axis, typename T>
auto sum(const  std::vector<std::vector<T>>&    input_matrix) -> std::enable_if_t<axis == 0, std::vector<T>>
{
    // creating canvas
    auto    canvas      {std::vector<T>(input_matrix[0].size(),     0)};

    // filling up the canvas
    for(auto row = 0; row < input_matrix.size(); ++row)
        std::transform(input_matrix[row].begin(), input_matrix[row].end(),
                       canvas.begin(),
                       canvas.begin(),
                       [](auto& argx, auto& argy){return argx + argy;});

    // returning
    return std::move(canvas);

}
/*==============================================================================
------------------------------------------------------------------------------*/ 
template <std::size_t axis, typename T>
auto sum(const  std::vector<std::vector<T>>&    input_matrix) -> std::enable_if_t<axis == 1, std::vector<std::vector<T>>>
{
    // creating canvas
    auto canvas     {std::vector<std::vector<T>>(input_matrix.size(), 
                                                 std::vector<T>(1, 0.00))};

    // filling up the canvas
    for(auto row = 0; row < input_matrix.size(); ++row)
        canvas[row][0]     = std::accumulate(input_matrix[row].begin(),
                                             input_matrix[row].end(), 
                                             static_cast<T>(0));

    // returning
    return std::move(canvas);

}
/*==============================================================================
------------------------------------------------------------------------------*/ 
template <std::size_t axis, typename T>
auto sum(const  std::vector<T>&     input_vector_A,
         const  std::vector<T>&     input_vector_B) -> std::enable_if_t<axis == 0, std::vector<T> >
{
    // setup
    const   auto&   num_cols_A      {input_vector_A.size()};
    const   auto&   num_cols_B      {input_vector_B.size()};

    // throwing errors 
    if (num_cols_A != num_cols_B)   {std::cerr << "sum: size disparity\n";}

    // creating canvas
    auto    canvas  {input_vector_A};

    // summing up
    std::transform(input_vector_B.begin(), input_vector_B.end(),
                   canvas.begin(),
                   canvas.begin(),
                   std::plus<T>());

    // returning
    return std::move(canvas);
}