/*==============================================================================
Member-Function: Subsetting the scatterers in the transmitter-range
------------------------------------------------------------------------------*/ 
template <
    svr::PureFloatingPointType          T,
    svr::FFT_SourceDestination_Type     sourceType,
    svr::FFT_SourceDestination_Type     destinationType,
    svr::PureComplexFloatingType        T_PureComplex
>
void AUVClass<T, sourceType, destinationType, T_PureComplex>::subset_scatterers(
    const  ScattererClass<T>&    seafloor,
    svr::ThreadPool&             thread_pool,
    std::vector<std::size_t>&    fls_scatterer_indices,
    std::vector<std::size_t>&    portside_scatterer_indices,
    std::vector<std::size_t>&    starboard_scatterer_indices
){
    // ensuring the components are synced
    this->syncComponentAttributes();

    // finding the pointing direction in spherical 
    auto    auv_pointing_direction_spherical    {svr::cart2sph(this->pointing_direction)};

    // spinning up the thread-pool
    thread_pool.push_back(
        [&]{transmitter_fls.subset_scatterers(seafloor,
                                              fls_scatterer_indices,
                                              0);}
    );
    thread_pool.push_back(
        [&]{transmitter_portside.subset_scatterers(seafloor,
                                                   portside_scatterer_indices,
                                                   auv_pointing_direction_spherical[1]);}
    );
    thread_pool.push_back(
        [&]{transmitter_starboard.subset_scatterers(seafloor,
                                                    starboard_scatterer_indices,
                                                    -1*auv_pointing_direction_spherical[1]);
                                                }
    );

    // waiting until all the work is done
    thread_pool.converge();
}