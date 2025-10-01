#pragma once
namespace   svr {
    /*==============================================================================
    tiling a vector
    ------------------------------------------------------------------------------*/ 
    template <typename T>
    auto tile(const     std::vector<T>&         input_vector,
              const     std::vector<size_t>&    mul_dimensions){

        // creating canvas
        const   std::size_t&   num_rows    {1 * mul_dimensions[0]};
        const   std::size_t&   num_cols    {input_vector.size() * mul_dimensions[1]};
        auto    canvas  {std::vector<std::vector<T>>(
            num_rows,
            std::vector<T>(num_cols, 0)
        )};

        // writing
        std::size_t     source_row;
        std::size_t     source_col;

        for(std::size_t row = 0; row < num_rows; ++row){
            for(std::size_t col = 0; col < num_cols; ++col){
                source_row  =   row % 1;
                source_col  =   col % input_vector.size();
                canvas[row][col]  = input_vector[source_col];
            }
        }

        // returning
        return std::move(canvas);
    }
    /*==============================================================================
    tiling a matrix
    ------------------------------------------------------------------------------*/ 
    template <typename T>
    auto tile(const     std::vector<std::vector<T>>&    input_matrix,
              const     std::vector<size_t>&            mul_dimensions){

        // creating canvas
        const   std::size_t&   num_rows    {input_matrix.size()     * mul_dimensions[0]};
        const   std::size_t&   num_cols    {input_matrix[0].size()  * mul_dimensions[1]};
        auto    canvas  {std::vector<std::vector<T>>(
            num_rows,
            std::vector<T>(num_cols, 0)
        )};

        // writing
        std::size_t     source_row;
        std::size_t     source_col;

        for(std::size_t row = 0; row < num_rows; ++row){
            for(std::size_t col = 0; col < num_cols; ++col){
                source_row  =   row % input_matrix.size();
                source_col  =   col % input_matrix[0].size();
                canvas[row][col]  = input_matrix[source_row][source_col];
            }
        }

        // returning
        return std::move(canvas);
    }
}