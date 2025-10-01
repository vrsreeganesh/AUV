#pragma once
namespace   svr {
    /*==============================================================================
    For type-deductions
    ------------------------------------------------------------------------------*/ 
    template <typename T>
    struct  fft_result_type;

    // specializations
    template <> struct  fft_result_type<double>{
        using  type = std::complex<double>;
    };  
    template <> struct fft_result_type<std::complex<double>>{
        using type = std::complex<double>;
    };  
    template <> struct  fft_result_type<float>{
        using  type = std::complex<float>;
    };
    template <> struct fft_result_type<std::complex<float>>{
        using type = std::complex<float>;
    };

    template <typename  T>
    using   fft_result_t    = typename fft_result_type<T>::type;

    /*==============================================================================
    y = fft(x, nfft)
        > calculating n-point dft where n-value is explicit
    ------------------------------------------------------------------------------*/ 
    template<typename T>
    auto fft(const      std::vector<T>&     input_vector, 
             const      size_t              nfft)
    {
        // throwing an error
        if (nfft < input_vector.size())     {std::cerr << "size-mistmatch\n";}
        if (nfft <= 0)                      {std::cerr << "size-mistmatch\n";}

        // fetching data-type
        using   RType   = fft_result_t<T>;
        using   baseType    = std::conditional_t<std::is_same_v<T, std::complex<double>>,
                                                 double,
                                                 T>;

        // canvas instantiation
        std::vector<RType>  canvas(nfft);
        auto    nfft_sqrt       {static_cast<RType>(std::sqrt(nfft))};
        auto    finaloutput     {std::vector<RType>(nfft, 0)};

        // calculating index by index
        for(int frequency_index = 0; frequency_index<nfft; ++frequency_index){
            RType   accumulate_value;
            for(int signal_index = 0; signal_index < input_vector.size(); ++signal_index){
                accumulate_value += \
                    static_cast<RType>(input_vector[signal_index]) * \
                    static_cast<RType>(std::exp(-1.00 * std::numbers::pi * \
                                                (static_cast<baseType>(frequency_index)/static_cast<baseType>(nfft)) * \
                                                static_cast<baseType>(signal_index)));
            }
            finaloutput[frequency_index]    = accumulate_value / nfft_sqrt;
        }

        // returning
        return std::move(finaloutput);
    }

    /*==============================================================================
    y = ifft(x, nfft)
    ------------------------------------------------------------------------------*/ 
    template<typename T>
    auto ifft(const      std::vector<T>&     input_vector)
    {
        // fetching data-type
        using   RType   = fft_result_t<T>;
        using   baseType    = std::conditional_t<std::is_same_v<T, std::complex<double>>,
                                                 double,
                                                 T>;
        
        //  setup
        auto    nfft        {input_vector.size()};

        // canvas instantiation
        std::vector<RType>  canvas(nfft);
        auto    nfft_sqrt       {static_cast<RType>(std::sqrt(nfft))};
        auto    finaloutput     {std::vector<RType>(nfft, 0)};

        // calculating index by index
        for(int frequency_index = 0; frequency_index<nfft; ++frequency_index){
            RType   accumulate_value;
            for(int signal_index = 0; signal_index < input_vector.size(); ++signal_index){
                accumulate_value += \
                    static_cast<RType>(input_vector[signal_index]) * \
                    static_cast<RType>(std::exp(1.00 * std::numbers::pi * \
                                                (static_cast<baseType>(frequency_index)/static_cast<baseType>(nfft)) * \
                                                static_cast<baseType>(signal_index)));
            }
            finaloutput[frequency_index]    = accumulate_value  /   nfft_sqrt;
        }

        // returning
        return std::move(finaloutput);
    }
}