/*==============================================================================
Aim: Function that begins imaging from the recorded signals
------------------------------------------------------------------------------*/
template <
    svr::PureFloatingPointType          T,
    svr::FFT_SourceDestination_Type     sourceType,
    svr::FFT_SourceDestination_Type     destinationType,
    svr::PureComplexFloatingType        T_PureComplex
>
void AUVClass<T, sourceType, destinationType, T_PureComplex>::image(
    svr::ThreadPool&                                                    thread_pool,
    svr::FFTPlanUniformPoolHandle<      T_PureComplex, T_PureComplex>&  fph,
    svr::IFFTPlanUniformPoolHandle<     T_PureComplex, T_PureComplex>&  ifph
){
    thread_pool.push_back([&]{
        this->ULA_fls.image(
            this->transmitter_fls,
            fph, ifph
        );
    });
    thread_pool.push_back([&]{
        this->ULA_portside.image(
            this->transmitter_portside,
            fph, ifph
        );
    });
    thread_pool.push_back([&]{
        this->ULA_starboard.image(
            this->transmitter_starboard,
            fph, ifph
        );
    });
    
    
    // waiting for threads to converge
    thread_pool.converge();
}