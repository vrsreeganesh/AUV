#pragma once
/*==============================================================================
minimum along dimension 1
------------------------------------------------------------------------------*/ 
template <std::size_t axis, typename T>
auto    min(const   std::vector<std::vector<T>>& input_matrix) -> std::enable_if_t<axis == 1, std::vector<std::vector<T>> >
{
    // creating canvas
    auto    canvas      {std::vector<std::vector<T>>(input_matrix.size(),std::vector<T>(1))};

    // storing the values
    for(auto row = 0; row < input_matrix.size(); ++row) 
        canvas[row][0]    = *(std::min_element(input_matrix[row].begin(), input_matrix[row].end()));

    // returning the value
    return std::move(canvas);
}