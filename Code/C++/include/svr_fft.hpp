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

    /*==========================================================================
    y = fft(std::vector<double> nfft)   // specialization
    --------------------------------------------------------------------------*/
    #include <fftw3.h>          // for fft
    template <>
    auto fft(const  std::vector<double>&    input_vector, 
             const  std::size_t             nfft)
    {
        if (nfft < input_vector.size())
            throw std::runtime_error("nfft must be >= input_vector.size()");
        if (nfft <= 0)
            throw std::runtime_error("nfft must be > 0");

        // FFTW real-to-complex output
        std::vector<std::complex<double>> output(nfft);

        // Allocate input (double) and output (fftw_complex) arrays
        double* in          = reinterpret_cast<double*>(
            fftw_malloc(sizeof(double) * nfft)
        );
        fftw_complex* out   = reinterpret_cast<fftw_complex*>(
            fftw_malloc(sizeof(fftw_complex) * (nfft/2 + 1))
        );

        // Copy input and zero-pad if needed
        for (std::size_t i = 0; i < nfft; ++i) {
            in[i] = (i < input_vector.size()) ? input_vector[i] : 0.0;
        }

        // Create FFTW plan and execute
        fftw_plan plan  = fftw_plan_dft_r2c_1d(
            static_cast<int>(nfft), in, out, FFTW_ESTIMATE
        );
        fftw_execute(plan);

        // Copy FFTW output to std::vector<std::complex<double>>
        for (std::size_t i = 0; i < nfft/2 + 1; ++i) {
            output[i] = std::complex<double>(out[i][0], out[i][1]);
        }
        // Optional: fill remaining bins with zeros to match full nfft size
        for (std::size_t i = nfft/2 + 1; i < nfft; ++i) {
            output[i] = std::complex<double>(0.0, 0.0);
        }

        // Cleanup
        fftw_destroy_plan(plan);
        fftw_free(in);
        fftw_free(out);

        // filling up the other half of the output
        const   auto    halfpoint   {static_cast<std::size_t>(nfft/2)};
        std::transform(
            output.begin() + 1,          // first half (skip DC)
            output.begin() + halfpoint,  // end of first half
            output.rbegin(),         // start writing from last element backward (skip Nyquist)
            [](const auto& x) { return std::conj(x); }
        );

        // returning
        return std::move(output);
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

    /*==========================================================================
    x = ifft(std::vector<std::complex<double>> spectrum, nfft)
    --------------------------------------------------------------------------*/
    #include <fftw3.h>
    #include <vector>
    #include <complex>
    #include <stdexcept>

    auto ifft(const std::vector<std::complex<double>>& input_vector,
              const std::size_t                        nfft)
    {
        if (nfft <= 0)
            throw std::runtime_error("nfft must be > 0");
        if (input_vector.size() != nfft)
            throw std::runtime_error("input spectrum must be of size nfft");

        // Output: real-valued time-domain sequence
        std::vector<double> output(nfft);

        // Allocate FFTW input/output
        fftw_complex* in = reinterpret_cast<fftw_complex*>(
            fftw_malloc(sizeof(fftw_complex) * (nfft/2 + 1))
        );
        double* out = reinterpret_cast<double*>(
            fftw_malloc(sizeof(double) * nfft)
        );

        // Copy *only* the first nfft/2+1 bins (rest are redundant due to symmetry)
        for (std::size_t i = 0; i < nfft/2 + 1; ++i) {
            in[i][0] = input_vector[i].real();
            in[i][1] = input_vector[i].imag();
        }

        // Create inverse FFTW plan
        fftw_plan plan = fftw_plan_dft_c2r_1d(
            static_cast<int>(nfft), 
            in, 
            out, 
            FFTW_ESTIMATE
        );

        fftw_execute(plan);

        // Normalize by nfft (FFTW leaves IFFT unscaled)
        for (std::size_t i = 0; i < nfft; ++i) {
            output[i] = out[i] / static_cast<double>(nfft);
        }

        // Cleanup
        fftw_destroy_plan(plan);
        fftw_free(in);
        fftw_free(out);

        return output;
    }



}