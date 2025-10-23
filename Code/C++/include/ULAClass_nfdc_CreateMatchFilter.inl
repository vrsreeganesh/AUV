/* =============================================================================
Aim: Creating match-filter
------------------------------------------------------------------------------*/
template <
    svr::PureFloatingPointType          T,
    svr::FFT_SourceDestination_Type     sourceType,
    svr::FFT_SourceDestination_Type     destinationType,
    svr::PureComplexFloatingType        T_PureComplex
>
void    ULAClass<T, sourceType, destinationType, T_PureComplex>::nfdc_CreateMatchFilter(
    const TransmitterClass<T>&                                          transmitterObj,
    svr::FFTPlanUniformPoolHandle<  T_PureComplex, T_PureComplex>&      fph_match_filter,
    svr::IFFTPlanUniformPoolHandle< T_PureComplex, T_PureComplex>&      ifph_match_filter
){
    // creating matrix for basebanding signal
    auto    linspace00              {svr::linspace(
        static_cast<T>(0),
        static_cast<T>(transmitterObj.signal.size()-1),
        static_cast<int>(transmitterObj.signal.size())
    )};
    auto    basebanding_vector      {
        svr::exp(
            -1 * std::complex<T>(0, 1) * 2.00 * static_cast<T>(std::numbers::pi)                 * \
            (static_cast<T>(transmitterObj.f_low) / static_cast<T>(this->sampling_frequency))   * \
            linspace00
        )
    };

    // multiplying signal with basebanding signal
    auto    match_filter        {   
        svr::complex(   transmitterObj.signal   ) * basebanding_vector   
    };
    basebanding_vector.clear();

    // low-pass filtering with baseband signal to obtain pure baseband signal
    match_filter    =   svr::conv1D_long_FFTPlanPool(
        svr::complex(   match_filter    ),
        svr::complex(   this->lowpass_filter_coefficients_for_decimation    ),
        fph_match_filter,
        ifph_match_filter
    );
    
    // creating sampling-indices
    int     decimation_factor   {static_cast<int>(std::floor(
        (static_cast<T>(this->sampling_frequency)/2.00) /   \
        (static_cast<T>(transmitterObj.bandwidth)/2.00))
    )};
    this->decimation_factor =   decimation_factor;
    int     final_num_samples   {static_cast<int>(std::ceil(
        static_cast<T>(match_filter.size())/                \
        static_cast<T>(decimation_factor)
    ))};
    auto    sampling_indices    {
        svr::linspace(
            1,
            (final_num_samples - 1) * decimation_factor,
            final_num_samples
        )
    };

    // sampling the signal
    match_filter    =   svr::index(match_filter, sampling_indices);

    // taking conjugate and flipping the signal
    match_filter    =   svr::fliplr(    match_filter);
    match_filter    =   svr::conj(      match_filter);

    // storing the match-filter to the class member
    this->matchFilter   =   std::move(match_filter);

    // Logging
    spdlog::info("Finished match-filter creation");
}