/*==============================================================================
Imaging 
------------------------------------------------------------------------------*/
template <
    svr::PureFloatingPointType          T,
    svr::FFT_SourceDestination_Type     sourceType,
    svr::FFT_SourceDestination_Type     destinationType,
    svr::PureComplexFloatingType        T_PureComplex
>
void    ULAClass< T, sourceType, destinationType, T_PureComplex >::image(
    const  TransmitterClass<T>&                                                 transmitter,
    svr::FFTPlanUniformPoolHandle<      T_PureComplex, T_PureComplex>&          fph,
    svr::IFFTPlanUniformPoolHandle<     T_PureComplex, T_PureComplex>&          ifph
){
    // decimating 
    this->decimate_signal(transmitter,fph, ifph);

    // beamforming


    // throwing a warning
    spdlog::warn("ULAClass::image() | incomplete");
}