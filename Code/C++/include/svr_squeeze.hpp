namespace   svr {
    template    <typename   T>
    auto    squeeze(const   std::vector<std::vector<T>>&    input_matrix)
    {
        // fetching dimensions
        const   auto&   num_rows_matrix     {input_matrix.size()};
        const   auto&   num_cols_matrix     {input_matrix[0].size()};

        // check if any dimension is 1
        if (num_rows_matrix == 0 || num_cols_matrix == 0)
            std::cerr << "at least one dimension should be 1";

        auto    final_length    {std::max(num_rows_matrix, num_cols_matrix)};

        // creating canvas
        auto    canvas          {std::vector<T>(final_length)};

        // building canvas
        if (num_rows_matrix == 1)
        {
            // filling canvas
            std::copy(input_matrix[0].begin(), input_matrix[0].end(),
                      canvas.begin());
        }
        else if(num_cols_matrix == 1)
        {
            // filling canvas
            std::transform(input_matrix.begin(),    input_matrix.end(),
                           canvas.begin(),
                           [](const auto& argx){
                                return argx[0];
                           });
        }

        // returning
        return std::move(canvas);
    }
}