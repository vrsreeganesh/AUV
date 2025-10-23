/*==============================================================================
Aim: Simulate Signals received by ULAs in the AUV
------------------------------------------------------------------------------*/ 
template <
    svr::PureFloatingPointType          T,
    svr::FFT_SourceDestination_Type     sourceType,
    svr::FFT_SourceDestination_Type     destinationType,
    svr::PureComplexFloatingType        T_PureComplex
>
void AUVClass<T, sourceType, destinationType, T_PureComplex>::simulate_signal(
    const     ScattererClass<T>&                                                seafloor,
    svr::ThreadPool&                                                            thread_pool,
    svr::FFTPlanUniformPoolHandle<  sourceType,      destinationType     >&     fft_pool_handle,
    svr::IFFTPlanUniformPoolHandle< destinationType, sourceType         >&      ifft_pool_handle
){
    // boolean-vector indicating which scatterers are present 
    auto    fls_scatterer_indices               {std::vector<std::size_t>()};
    auto    portside_scatterer_indices          {std::vector<std::size_t>()};
    auto    starboard_scatterer_indices         {std::vector<std::size_t>()};

    // asking the transmitters subset the scatterers 
    this->subset_scatterers(seafloor,
                            thread_pool,
                            fls_scatterer_indices,
                            portside_scatterer_indices,
                            starboard_scatterer_indices);


    // asking ULAs to simulate signal
    thread_pool.push_back(
        [&]{this->ULA_fls.simulate_signals(
            seafloor,
            fls_scatterer_indices,
            this->transmitter_fls,
            fft_pool_handle,
            ifft_pool_handle
        );});
    thread_pool.push_back(
        [&](){this->ULA_portside.simulate_signals(
            seafloor, 
            portside_scatterer_indices,
            this->transmitter_portside,
            fft_pool_handle,
            ifft_pool_handle
        );});
    thread_pool.push_back(
        [&](){this->ULA_starboard.simulate_signals(
            seafloor,
            starboard_scatterer_indices,
            this->transmitter_starboard,
            fft_pool_handle,
            ifft_pool_handle
        );});
    
    // waiting for threads to converge
    thread_pool.converge();

    // logging
    spdlog::info("Signal-Simulation Complete");
}