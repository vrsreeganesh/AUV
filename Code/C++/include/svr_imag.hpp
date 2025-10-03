#pragma once
namespace   svr {

    /*==========================================================================
    For type-deductions
    --------------------------------------------------------------------------*/ 
    template    <typename T>
    struct  imag_result_type;  

    template <> struct  imag_result_type<std::complex<double>>{
        using type = double;
    };
    template <> struct  imag_result_type<std::complex<float>>{
        using type = float;
    };
    template <> struct imag_result_type<double> {
        using type  = double;
    };
    template <> struct imag_result_type<float>{
        using type = float;
    };

    template    <typename T>
    using   imag_result_t   = typename imag_result_type<T>::type;

    /*==========================================================================
    --------------------------------------------------------------------------*/ 
    template    <typename   T>
    auto    imag(const  std::vector<T>&     input_vector)
    {
        // figure out base-type
        using TCanvas   = imag_result_t<T>;

        // creating canvas
        auto    canvas      {std::vector<TCanvas>(
            input_vector.size()
        )};

        // storing values
        std::transform(input_vector.begin(),    input_vector.end(),
                        canvas.begin(),
                        [](const    auto&    argx){
                            return std::imag(argx);
                        });

        // returning
        return std::move(canvas);
    }
}