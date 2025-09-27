namespace   svr {
    // =============================================================================
    // For type-deductions
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

    // // =========================================================================
    // // y = fft(x, nfft)
    // template<typename T>
    // auto fft(const      std::vector<T>&     input_vector, 
    //          const      size_t              nfft)
    // {

    //     svr::Timer timer("fft");

    //     // throwing an error
    //     if (nfft < input_vector.size())     {std::cerr << "size-mistmatch\n";}
    //     if (nfft <= 0)                      {std::cerr << "size-mistmatch\n";}

    //     // fetching data-type
    //     using   RType   = fft_result_t<T>;

    //     // canvas instantiation
    //     std::vector<RType>  canvas(nfft);

    //     // building time-only basis
    //     std::vector<RType>
    //     basiswithoutfrequency   {linspace(static_cast<RType>(0),
    //                                       static_cast<RType>(nfft-1),
    //                                       nfft)};
    //     auto    lambda_basiswithoutfrequency    = [&basiswithoutfrequency](RType& arg){
    //         return std::exp(-1.00 * 1i * 2.00 *                             \
    //                         std::numbers::pi * static_cast<RType>(arg) /    \
    //                         static_cast<RType>(basiswithoutfrequency.size()));
    //     };
    //     std::transform(basiswithoutfrequency.begin(),   basiswithoutfrequency.end(),
    //                    basiswithoutfrequency.begin(),
    //                    lambda_basiswithoutfrequency);

    //     // building basis vectors
    //     auto    bases_vectors    {std::vector<std::vector<RType>>()};
    //     for(auto i = 0; i < nfft; ++i){
    //         // making a copy of the bases-without-frequency
    //         auto    temp    {basiswithoutfrequency};
    //         // exponentiating basis with frequency
    //         std::transform(temp.begin(),    temp.end(),
    //                        temp.begin(),
    //                        [&i](auto& argx){return std::pow(argx, i);});
    //         // pushing to end of bases-vectors
    //         bases_vectors.push_back(std::move(temp));
    //     }

    //     // projecting input-array onto fourier bases
    //     auto    finaloutput     {std::vector<RType>(nfft, 0)};
    //     auto    nfft_sqrt       {static_cast<RType>(std::sqrt(nfft))};
    //     #pragma omp parallel for
    //     for(int i = 0; i < nfft; ++i){
    //         // projecting input-vector with 
    //         finaloutput[i]  =   std::inner_product(input_vector.begin(), input_vector.end(),
    //                                                 bases_vectors[i].begin(),
    //                                                 RType(0),
    //                                                 std::plus<RType>(),
    //                                                 [&nfft_sqrt](const auto& argx,
    //                                                              const auto& argy){
    //                                                     return static_cast<RType>(argx) * static_cast<RType>(argy) / nfft_sqrt;
    //                                                 });
    //     }
    //     // returning finaloutput
    //     return std::move(finaloutput);
    // }

    // =========================================================================
    // y = fft(x, nfft)
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












    // // =========================================================================
    // // y = ifft(x)
    // template<typename T>
    // auto ifft(const      std::vector<T>&     input_vector)
    // {
    //     svr::Timer timer00("ifft");

    //     // fetching nfft
    //     auto    nfft    {input_vector.size()};

    //     // fetching data-type
    //     using   RType   = fft_result_t<T>;

    //     // canvas instantiation
    //     std::vector<RType>  canvas(nfft);

    //     // building time-only basis
    //     std::vector<RType>
    //     basiswithoutfrequency   {linspace(static_cast<RType>(0),
    //                                       static_cast<RType>(nfft-1),
    //                                       nfft)};
    //     auto    lambda_basiswithoutfrequency    = [&basiswithoutfrequency](RType& arg){
    //         return std::exp(1.00 * 1i * 2.00 *                             \
    //                         std::numbers::pi * static_cast<RType>(arg) /    \
    //                         static_cast<RType>(basiswithoutfrequency.size()));
    //     };
    //     std::transform(basiswithoutfrequency.begin(),   basiswithoutfrequency.end(),
    //                    basiswithoutfrequency.begin(),
    //                    lambda_basiswithoutfrequency);

    //     // building basis vectors
    //     auto    bases_vectors    {std::vector<std::vector<RType>>()};
    //     for(auto i = 0; i < nfft; ++i){
    //         // making a copy of the bases-without-frequency
    //         auto    temp    {basiswithoutfrequency};
    //         // exponentiating basis with frequency
    //         std::transform(temp.begin(),    temp.end(),
    //                        temp.begin(),
    //                        [&i](auto& argx){return std::pow(argx, i);});
    //         // pushing to end of bases-vectors
    //         bases_vectors.push_back(std::move(temp));
    //     }

    //     // projecting input-array onto fourier bases
    //     auto    finaloutput     {std::vector<RType>(nfft, 0)};
    //     auto    nfft_sqrt       {static_cast<RType>(std::sqrt(nfft))};
    //     #pragma omp parallel for
    //     for(int i = 0; i < nfft; ++i){
    //         // projecting input-vector with 
    //         finaloutput[i]  =   std::inner_product(input_vector.begin(), input_vector.end(),
    //                                                bases_vectors[i].begin(),
    //                                                RType(0),
    //                                                std::plus<RType>(),
    //                                                [&nfft_sqrt](const auto& argx,
    //                                                             const auto& argy){
    //                                                     return static_cast<RType>(argx) * static_cast<RType>(argy) / nfft_sqrt;
    //                                                 });
    //     }
    //     // returning finaloutput
    //     return std::move(finaloutput);
    // }

    // =========================================================================
    // y = ifft(x, nfft)
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