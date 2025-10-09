#pragma once
namespace   svr {
    /*==========================================================================
    y = index(vector, mask)

    template    <
        typename T1,
        typename T2,
        typename = std::enable_if_t<
            (std::is_arithmetic_v<T1>                   || 
            std::is_same_v<T1, std::complex<float>  >   ||
            std::is_same_v<T1, std::complex<double> >)   &&
            std::is_integral_v<T2>
        >
    >
    --------------------------------------------------------------------------*/
    template    <typename T1,
                 typename T2,
                 typename = std::enable_if_t<std::is_arithmetic_v<T1>                   || 
                                             std::is_same_v<T1, std::complex<float> >   ||
                                             std::is_same_v<T1, std::complex<double> >
                                            >
                 >
    auto    index(const     std::vector<T1>&        input_vector,
                  const     std::vector<T2>&        indices_to_sample)
    {
        // creating canvas
        auto    canvas      {std::vector<T1>(indices_to_sample.size(), 0)};

        // copying the associated values
        for(int i = 0; i < indices_to_sample.size(); ++i){
            auto    source_index    {indices_to_sample[i]};
            if(source_index < input_vector.size()){
                canvas[i] = input_vector[source_index];
            }
            else{
                // cout << "warning: Some chosen samples are out of bounds. svr::index | source_index !< input_vector.size()\n";
            }
                
        }

        // returning
        return std::move(canvas);
    }
    /*==========================================================================
    y = index(matrix, mask, dim)
    --------------------------------------------------------------------------*/
    template <
        typename T1, 
        typename T2,
        typename    =   std::enable_if_t<
            (std::is_same_v<T1, double> ||  std::is_same_v<T1, float>)     &&
            (std::is_same_v<T2, int>    ||  std::is_same_v<T2, std::size_t>)
        >
    >
    auto index(const    std::vector<std::vector<T1>>&   input_matrix,
               const    std::vector<T2>&                indices_to_sample,
               const    std::size_t&                    dim)
    {
        // fetching dimensions
        const   auto&   num_rows_matrix     {input_matrix.size()};
        const   auto&   num_cols_matrix     {input_matrix[0].size()};

        // creating canvas
        auto    canvas  {std::vector<std::vector<T1>>()};
        
        // if indices are row-indices
        if (dim == 0){
            
            // initializing canvas
            canvas  = std::vector<std::vector<T1>>(
                num_rows_matrix,
                std::vector<T1>(indices_to_sample.size())
            );

            // filling the canvas
            auto destination_index  {0};
            std::for_each(
                indices_to_sample.begin(), indices_to_sample.end(),
                [&](const auto& col){
                for(auto row = 0; row < num_rows_matrix; ++row){
                    if (col <= input_matrix[0].size()){
                        canvas[row][destination_index]    =   input_matrix[row][col];
                    }
                }
                ++destination_index;
                });
        }
        else if(dim == 1){
            // initializing canvas
            canvas = std::vector<std::vector<T1>>(
                indices_to_sample.size(),
                std::vector<T1>(num_cols_matrix)
            );

            // filling the canvas
            #pragma omp parallel for
            for(auto row = 0; row < canvas.size(); ++row){
                auto    destination_col   {0};
                std::for_each(indices_to_sample.begin(),    indices_to_sample.end(),
                              [&row, 
                               &input_matrix, 
                               &destination_col,
                               &canvas](const auto& source_col){
                                    canvas[row][destination_col++] = input_matrix[row][source_col];
                              });
            }
        }
        else {
            std::cerr << "svr_index | this dim is not implemented \n";
        }

        // moving it back 
        return std::move(canvas);
    }
}   