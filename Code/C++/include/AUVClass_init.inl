/* =========================================================================
Aim: Initializing objects and variables
--------------------------------------------------------------------------*/ 
template <
    svr::PureFloatingPointType          T,
    svr::FFT_SourceDestination_Type     sourceType,
    svr::FFT_SourceDestination_Type     destinationType,
    svr::PureComplexFloatingType        T_PureComplex
>
void AUVClass<T, sourceType, destinationType, T_PureComplex>::init(
    svr::ThreadPool&                                                 thread_pool,
    svr::FFTPlanUniformPoolHandle<  T_PureComplex, T_PureComplex>&   fph_match_filter,
    svr::IFFTPlanUniformPoolHandle< T_PureComplex, T_PureComplex>&   ifph_match_filter
){
    // call sync-component attributes
    this->syncComponentAttributes();

    // initializing the ULAs
    thread_pool.push_back([&]{this->ULA_fls.init(         
        std::ref(this->transmitter_fls),
        fph_match_filter,
        ifph_match_filter);});
    thread_pool.push_back([&]{this->ULA_portside.init(    
        std::ref(this->transmitter_portside),
        fph_match_filter,
        ifph_match_filter);});
    thread_pool.push_back([&]{this->ULA_starboard.init(   
        std::ref(this->transmitter_starboard),
        fph_match_filter,
        ifph_match_filter);});
    thread_pool.converge();

    // logging
    spdlog::info("Finished AUV Initialization");
}