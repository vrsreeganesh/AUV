/* =============================================================================
Aim: Init 
------------------------------------------------------------------------------*/
template <
    svr::PureFloatingPointType          T,
    svr::FFT_SourceDestination_Type     sourceType,
    svr::FFT_SourceDestination_Type     destinationType,
    svr::PureComplexFloatingType        T_PureComplex
>
void    ULAClass<T, sourceType, destinationType, T_PureComplex>::init(
    const TransmitterClass<         T                           >&      transmitterObj,
    svr::FFTPlanUniformPoolHandle<  T_PureComplex, T_PureComplex >&     fph_match_filter,
    svr::IFFTPlanUniformPoolHandle< T_PureComplex, T_PureComplex >&     ifph_match_filter
){    
    // calculating range-related parameters
    this->range_resolution      =   1500.00/(2 * transmitterObj.fc);
    this->range_cell_size       =   40  *   this->range_resolution;
    
    // calculating azimuth-related parameters
    this->azimuthal_resolution  =   (1500.00 / transmitterObj.fc)   /   \
                                    (this->num_sensors - 1)  *   (this->inter_element_spacing);
    this->azimuth_cell_size     =   2   *   this->azimuthal_resolution;

    // creating basebanding signal
    this->basebanding_signal    =   svr::linspace(
        static_cast<std::complex<T>>(   0),
        static_cast<std::complex<T>>(   this->num_samples-1),
        static_cast<std::size_t>(       this->num_samples)
    );
    // exponentiating signal to create final-basebanding matrix
    this->basebanding_signal  =   svr::exp(
        1i * 2 * std::numbers::pi * transmitterObj.f_low * \
        basebanding_signal / sampling_frequency
    );
    // asserting that the size of the signal is the same as that of the 
    if (this->basebanding_signal.size() != this->num_samples)
        throw std::runtime_error(
            "FILE: ULAClass.hpp | FUNCTION: ULAClass::init() | REPORT: basebanding_signal.size() != num_samples"
        );

    // creating and storing match-filter
    this->nfdc_CreateMatchFilter(   transmitterObj, 
                                    fph_match_filter, 
                                    ifph_match_filter);

    // beamforming-related
    init_beamforming();

    // logging
    spdlog::info("Complete: ULA Initialization");
}