/* =============================================================================
Beamforming Initialization
------------------------------------------------------------------------------*/
template <
    svr::PureFloatingPointType          T,
    svr::FFT_SourceDestination_Type     sourceType,
    svr::FFT_SourceDestination_Type     destinationType,
    svr::PureComplexFloatingType        T_PureComplex
>
void    ULAClass<T, sourceType, destinationType, T_PureComplex>::init_beamforming(){

    /*
    We expect each range to be handled by plans in the pool. Naturally this means that we need to ensure that the range-sample and space for delay add up to the Pool-length.
    Perhaps we should create our own pool for this so that we can have threads to be done continuously.
    */     

    // throwing a warning
    spdlog::warn("ULAClass::init_beamforming | incomplete");
}