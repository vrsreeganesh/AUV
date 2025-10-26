/*==============================================================================
Decimating the recorded signal
    -> basebanding the signal
    -> lowpass-filtering
    -> downsample signals
------------------------------------------------------------------------------*/
template <
    svr::PureFloatingPointType          T,
    svr::FFT_SourceDestination_Type     sourceType,
    svr::FFT_SourceDestination_Type     destinationType,
    svr::PureComplexFloatingType        T_PureComplex
>
void    ULAClass<T, sourceType, destinationType, T_PureComplex>::decimate_signal(
    const  TransmitterClass<T>&                                                 transmitter,
    svr::FFTPlanUniformPoolHandle<      T_PureComplex, T_PureComplex>&          fph,
    svr::IFFTPlanUniformPoolHandle<     T_PureComplex, T_PureComplex>&          ifph
){
    // basebanding signals
    auto    basebanded_signal_matrix   {
        this->signal_matrix * this->basebanding_signal
    };

    // low-pass filtering 
    auto&    basebanded_lowpassfiltered_signal_matrix    {basebanded_signal_matrix};
    for(auto row = 0; row < basebanded_signal_matrix.size(); ++row){
        basebanded_lowpassfiltered_signal_matrix[row] = \
            std::move(
                svr::conv1D_long_FFTPlanPool(
                    basebanded_signal_matrix[row],
                    svr::complex(   this->lowpass_filter_coefficients_for_decimation    ),
                    fph, 
                    ifph
                )
            );
    }

    // // low-pass filtering 
    // auto&    basebanded_lowpassfiltered_signal_matrix    {basebanded_signal_matrix};
    // for(auto row = 0; row < basebanded_signal_matrix.size(); ++row){
    //     basebanded_lowpassfiltered_signal_matrix[row] = \
    //         std::move(
    //             svr::conv1D_long_FFTPlanPool(
    //                 basebanded_signal_matrix[row],
    //                 svr::complex(   this->lowpass_filter_coefficients_for_decimation    ),
    //                 *this->fph_image_ptr, 
    //                 *this->ifph_image_ptr
    //             )
    //         );
    // }

    // decimating the signal
    auto&   decimated_signal_matrix_src     {basebanded_lowpassfiltered_signal_matrix};
    this->decimated_signal_matrix     =   svr::sample(
        decimated_signal_matrix_src,
        static_cast<std::size_t>(0),
        static_cast<std::size_t>(this->decimation_factor),
        static_cast<std::size_t>(0)
    );
}