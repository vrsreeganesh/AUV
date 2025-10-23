/*==============================================================================
Beamforming
------------------------------------------------------------------------------*/
template <
    svr::PureFloatingPointType          T,
    svr::FFT_SourceDestination_Type     sourceType,
    svr::FFT_SourceDestination_Type     destinationType,
    svr::PureComplexFloatingType        T_PureComplex
>
void    ULAClass<T, sourceType, destinationType, T_PureComplex>::beamform(
    svr::FFTPlanUniformPoolHandle<      T_PureComplex, T_PureComplex >&         fph,
    svr::IFFTPlanUniformPoolHandle<     T_PureComplex, T_PureComplex >&         ifph   
){

    // 

    // throwing a warning
    spdlog::warn("ULAClass::beamform() | Incomplete");
}