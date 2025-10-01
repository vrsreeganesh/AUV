#pragma once
/*==============================================================================
y = vector + vector
------------------------------------------------------------------------------*/ 
template <typename T>
std::vector<T> operator+(const std::vector<T>& a, 
                         const std::vector<T>& b) 
{
    // Identify which is bigger
    const auto& big = (a.size() > b.size()) ? a : b;
    const auto& small = (a.size() > b.size()) ? b : a;

    std::vector<T> result = big;  // copy the bigger one

    // Add elements from the smaller one
    for (size_t i = 0; i < small.size(); ++i) {
        result[i] += small[i];
    }

    return result;
}
/*==============================================================================
------------------------------------------------------------------------------*/ 
// y = vector + vector
template <typename T>
std::vector<T>& operator+=(std::vector<T>& a, 
                           const std::vector<T>& b) {
    
    const auto& small = (a.size() < b.size()) ? a : b;
    const auto& big   = (a.size() < b.size()) ? b : a;

    // If b is bigger, resize 'a' to match
    if (a.size() < b.size())                        {a.resize(b.size());}

    // Add elements
    for (size_t i = 0; i < small.size(); ++i)       {a[i] += b[i];}

    // returning elements
    return a;
}
// =============================================================================
// y = matrix + matrix
template <typename T>
std::vector<std::vector<T>> operator+(const std::vector<std::vector<T>>& a,
                                      const std::vector<std::vector<T>>& b)
{
    // fetching dimensions
    const   auto&   num_rows_A      {a.size()};
    const   auto&   num_cols_A      {a[0].size()};
    const   auto&   num_rows_B      {b.size()};
    const   auto&   num_cols_B      {b[0].size()};

    // choosing the three different metrics
    if (num_rows_A != num_rows_B && num_cols_A != num_cols_B){
        cout << format("a.dimensions = [{},{}], b.shape = [{},{}]\n",
                        num_rows_A, num_cols_A, 
                        num_rows_B, num_cols_B);
        std::cerr << "dimensions don't match\n";
    }

    // creating canvas
    auto    canvas      {std::vector<std::vector<T>>(
        std::max(num_rows_A, num_rows_B),
        std::vector<T>(std::max(num_cols_A, num_cols_B), (T)0.00)
    )};

    // performing addition
    if (num_rows_A == num_rows_B && num_cols_A == num_cols_B){
        for(auto row = 0; row < num_rows_A; ++row){
            std::transform(a[row].begin(), a[row].end(),
                           b[row].begin(),
                           canvas[row].begin(),
                           std::plus<T>());
        }
    }
    else if(num_rows_A == num_rows_B){
        
        // if number of columsn are different, check if one of the cols are one
        const   auto    min_num_cols    {std::min(num_cols_A, num_cols_B)};
        if (min_num_cols != 1)  {std::cerr<< "Operator+: unable to broadcast\n";}
        const   auto    max_num_cols    {std::max(num_cols_A, num_cols_B)};

        // using references to tag em differently
        const   auto&   big_matrix      {num_cols_A > num_cols_B ? a : b};
        const   auto&   small_matrix    {num_cols_A < num_cols_B ? a : b};

        // Adding to canvas
        for(auto row = 0; row < canvas.size(); ++row){
            std::transform(big_matrix[row].begin(), big_matrix[row].end(),
                           canvas[row].begin(),
                           [&small_matrix, 
                            &row](const auto& argx){
                                return argx + small_matrix[row][0];
                           });
        }
    }
    else if(num_cols_A == num_cols_B){

        // check if the smallest column-number is one
        const   auto    min_num_rows    {std::min(num_rows_A, num_rows_B)};
        if(min_num_rows != 1)       {std::cerr << "Operator+ : unable to broadcast\n";}
        const   auto    max_num_rows    {std::max(num_rows_A, num_rows_B)};

        // using references to differentiate the two matrices
        const   auto&   big_matrix      {num_rows_A > num_rows_B ? a : b};
        const   auto&   small_matrix    {num_rows_A < num_rows_B ? a : b};

        // adding to canvas
        for(auto row = 0; row < canvas.size(); ++row){
            std::transform(big_matrix[row].begin(), big_matrix[row].end(),
                           small_matrix[0].begin(),
                           canvas[row].begin(),
                           [](const auto& argx, const auto& argy){
                            return argx + argy;
                           });
        }
    }
    else {
        std::cerr << "operator+: yet to be implemented \n";
    }

    // returning
    return std::move(canvas);
}
/*==============================================================================
y = vector + scalar
------------------------------------------------------------------------------*/ 
template <typename T>
auto operator+(const    std::vector<T>&     input_vector,
               const    T                   scalar)
{
    // creating canvas
    auto    canvas      {input_vector};

    // adding scalar to the canvas
    std::transform(canvas.begin(),  canvas.end(),
                   canvas.begin(),
                   [&scalar](auto& argx){return argx + scalar;});

    // returning canvas
    return std::move(canvas);
}
/*==============================================================================
y = scalar + vector
------------------------------------------------------------------------------*/ 
template <typename T>
auto operator+(const    T                   scalar,
               const    std::vector<T>&     input_vector)
{
    // creating canvas
    auto    canvas      {input_vector};

    // adding scalar to the canvas
    std::transform(canvas.begin(),  canvas.end(),
                   canvas.begin(),
                   [&scalar](auto& argx){return argx + scalar;});

    // returning canvas
    return std::move(canvas);
}
