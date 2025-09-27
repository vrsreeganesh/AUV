// =============================================================================
// Aim: substracting scalar from a vector
template <typename T>
std::vector<T> operator-(const std::vector<T>& a, const T scalar){
    std::vector<T> temp(a.size());
    std::transform(a.begin(),
                   a.end(),
                   temp.begin(),
                   [scalar](T x){return (x - scalar);});
    return temp;
}
// =============================================================================
template <typename T>
auto operator-(const    std::vector<std::vector<T>>&     input_matrix_A,
               const    std::vector<std::vector<T>>&     input_matrix_B)
{
    // throwing error in case of dimension differences
    if (input_matrix_A.size() != input_matrix_B.size() || 
        input_matrix_A[0].size() != input_matrix_B[0].size())
        std::cerr << "operator- dimension mismatch\n";

    // setting up canvas
    auto    canvas      {std::vector<std::vector<T>>(
        input_matrix_A.size(),
        std::vector<T>(input_matrix_A[0].size())
    )};

    // subtracting values
    for(auto row = 0; row < input_matrix_B.size(); ++row)
        std::transform(input_matrix_A[row].begin(), input_matrix_A[row].end(),
                       input_matrix_B[row].begin(),
                       canvas[row].begin(),
                       [](const auto& x, const auto& y){
                            return x - y;
                       });

    // returning 
    return std::move(canvas);
    
}