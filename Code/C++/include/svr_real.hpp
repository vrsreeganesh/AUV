#pragma once
namespace   svr {

    /*==========================================================================
    For type-deductions
    --------------------------------------------------------------------------*/ 
    template    <typename T>
    struct  real_result_type;

    template <> struct  real_result_type<std::complex<double>>{
        using type = double;
    };
    template <> struct  real_result_type<std::complex<float>>{
        using type = float;
    };
    template <> struct real_result_type<double> {
        using type  = double;
    };
    template <> struct real_result_type<float>{
        using type = float;
    };

    template    <typename T>
    using   real_result_t   = typename real_result_type<T>::type;

    /*==========================================================================
    Element-wise real() of a vector
    --------------------------------------------------------------------------*/ 
    template    <typename T>
    auto    real(const  std::vector<T>&   input_vector)
    {
        // figure out base-type
        using TCanvas   = real_result_t<T>;

        // creating canvas
        auto    canvas      {std::vector<TCanvas>(
            input_vector.size()
        )};

        // storing values
        std::transform(input_vector.begin(),    input_vector.end(),
                        canvas.begin(),
                        [](const    auto&    argx){
                            return std::real(argx);
                        });

        // returning
        return std::move(canvas);
    }
    // /*==========================================================================
    // Element-wise real() of a matrix
    // --------------------------------------------------------------------------*/ 
    // template    <typename   T>
    // auto    real(const  std::vector<std::vector<T>>&    input_matrix)
    // {
    //     // fetching dimensions
    //     const   auto&   num_rows_matrix     {input_matrix.size()};
    //     const   auto&   num_cols_matrix     {input_matrix[0].size()};

    //     // creating canvas
    //     auto    canvas      {std::vector<std::vector<T>>(
    //         num_rows_matrix,
    //         std::vector<T>(num_cols_matrix)
    //     )};

    //     // storing the values
    //     for(auto    row = 0; row < num_rows_matrix; ++row)
    //         std::transform(input_matrix[row].begin(),   input_matrix[row].end(),
    //                        canvas[row].begin(),
    //                        [](const auto&   argx){
    //                             return std::real(argx);
    //                        });

    //     // returning
    //     return std::move(canvas);
    // }
}