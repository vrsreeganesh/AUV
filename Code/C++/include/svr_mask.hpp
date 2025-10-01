#pragma once
namespace   svr {
    /*==========================================================================
    y = input_vector[mask == 1]
    --------------------------------------------------------------------------*/
    template <typename T,
              typename = std::enable_if_t< std::is_arithmetic_v<T>                  ||
                                           std::is_same_v<T, std::complex<double>>  ||
                                           std::is_same_v<T, std::complex<float>>
                                         >
             >
    auto mask(const     std::vector<T>&     input_vector,
              const     std::vector<bool>&  mask_vector)
    {
        // checking dimensionality issues
        if (input_vector.size() != mask_vector.size())
            std::cerr << "mask(vector, mask): incompatible size \n";
        
        // creating canas
        auto    num_trues   {std::count(mask_vector.begin(),
                                        mask_vector.end(),
                                        true)};
        auto    canvas      {std::vector<T>(num_trues)};

        // copying values
        auto    destination_index   {0};
        for(auto i = 0; i <input_vector.size(); ++i)
            if (mask_vector[i] == true)
                canvas[destination_index++]    = input_vector[i];

        // returning output
        return std::move(canvas);       
    }
    /*==========================================================================
    --------------------------------------------------------------------------*/
    template <typename T>
    auto mask(const     std::vector<std::vector<T>>&        input_matrix,
              const     std::vector<bool>                   mask_vector)
    {
        // fetching dimensions
        const   auto&   num_rows_matrix     {input_matrix.size()};
        const   auto&   num_cols_matrix     {input_matrix[0].size()};
        const   auto&   num_cols_vector          {mask_vector.size()};

        // error-checking
        if (num_cols_matrix != num_cols_vector)
            std::cerr << "mask(matrix, bool-vector): size disparity";

        // building canvas
        auto    num_trues   {std::count(mask_vector.begin(),
                                        mask_vector.end(),
                                        true)};
        auto    canvas      {std::vector<std::vector<T>>(
            num_rows_matrix,
            std::vector<T>(num_cols_vector, 0)
        )};

        // writing values
        #pragma omp parallel for
        for(auto row = 0; row < num_rows_matrix; ++row){
            auto    destination_index   {0};
            for(auto col = 0; col < num_cols_vector; ++col)
                if(mask_vector[col] == true)
                    canvas[row][destination_index++]    = input_matrix[row][col];
        }

        // returning
        return std::move(canvas);
    }
    /*==========================================================================
    Fetch Indices corresponding to mask true's
    --------------------------------------------------------------------------*/
    auto mask_indices(const std::vector<bool>&  mask_vector)
    {
        // creating canvas
        auto    num_trues   {std::count(mask_vector.begin(), mask_vector.end(),
                                        true)};
        auto    canvas      {std::vector<std::size_t>(num_trues)};

        // building canvas
        auto    destination_index   {0};
        for(auto i = 0; i < mask_vector.size(); ++i)
            if (mask_vector[i] == true)
                canvas[destination_index++] = i;

        // returning
        return std::move(canvas);
    }
}