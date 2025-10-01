#pragma once
/*==============================================================================
maximum along dimension 1
------------------------------------------------------------------------------*/ 
template <std::size_t axis, typename T>
auto    max(const   std::vector<std::vector<T>>&     input_matrix) -> std::enable_if_t<axis == 1, std::vector<std::vector<T>> >
{
    // setting up canvas
    auto    canvas      {std::vector<std::vector<T>>(input_matrix.size(),std::vector<T>(1))};

    // filling up the canvas
    for(auto row = 0; row < input_matrix.size(); ++row)
        canvas[row][0] = *(std::max_element(input_matrix[row].begin(), input_matrix[row].end()));

    // returning
    return std::move(canvas);
}