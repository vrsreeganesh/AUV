/*==============================================================================
Aim: Simulate signals received by uniform-linear-array
------------------------------------------------------------------------------*/ 
template <
    svr::PureFloatingPointType          T,
    svr::FFT_SourceDestination_Type     sourceType,
    svr::FFT_SourceDestination_Type     destinationType,
    svr::PureComplexFloatingType        T_PureComplex
>
void    ULAClass<T, sourceType, destinationType, T_PureComplex>::simulate_signals(
    const ScattererClass<T>&                                seafloor,
    const std::vector<std::size_t>                          scatterer_indices,
    const TransmitterClass<T>&                              transmitter,
    svr::FFTPlanUniformPoolHandle<T, std::complex<T>>&      fft_pool_handle,
    svr::IFFTPlanUniformPoolHandle<std::complex<T>, T>&     ifft_pool_handle
){
    // creating signal matrix
    auto    num_samples     {static_cast<std::size_t>(
        std::ceil(
            this->sampling_frequency * this->recording_period
        ))} ;
    this->signal_matrix     = svr::zeros<T>({this->num_sensors, num_samples});

    // placing dots for each sensor
    for(auto    sensor_index    =   0;
        sensor_index    < num_sensors;
        ++sensor_index)
    {
        // fetching sensor coordinates
        const   auto   curr_sensor_coordinate   {std::vector<T>({
            coordinates[0][sensor_index],
            coordinates[1][sensor_index],
            coordinates[2][sensor_index]
        })};

        // subsetting scatterers
        const   auto    subsetted_scatterers    {svr::index(seafloor.coordinates,
                                                            scatterer_indices,
                                                            0)};
        const   auto    subset_scatterer_return {svr::index(seafloor.reflectivity,
                                                            scatterer_indices)};
                                                            
        // calculating distance between sensor and scatterer
        auto    dist_from_sensor    {subsetted_scatterers - transpose(curr_sensor_coordinate)};
        dist_from_sensor        =   norm(dist_from_sensor, 0);

        // distance between transmitter and scatterer
        auto    dist_from_transmitter   {subsetted_scatterers - transpose(curr_sensor_coordinate)};
        dist_from_transmitter   =   norm(dist_from_transmitter, 0);
        
        // calculating time-of-flight
        auto   time_of_flight  {
            dist_from_sensor/transmitter.speed_of_sound     + \
            dist_from_transmitter/transmitter.speed_of_sound
        };
        auto&   indices_of_flight   {time_of_flight};
        indices_of_flight   = svr::floor(time_of_flight * this->sampling_frequency);

        // placing returns in the signal matrix
        edit_accumulate(this->signal_matrix[sensor_index],
                        svr::squeeze(indices_of_flight),
                        subset_scatterer_return);

    }

    // convolving input-signal with each row
    for(auto row = 0; row < this->signal_matrix.size(); ++row)
    {
        // // peforming convolution
        // this->signal_matrix[row] = svr::conv1D_long_FFTPlanPool(
        //     this->signal_matrix[row],
        //     transmitter.signal,
        //     fft_pool_handle,
        //     ifft_pool_handle
        // );

        // // checking if nullptr
        // if(this->fph_ptr_sigsim == nullptr)     {throw std::runtime_error("NULLPTR ISSUE");}
        // if(this->ifph_ptr_sigsim == nullptr)     {throw std::runtime_error("NULLPTR ISSUE");}

        // peforming convolution
        this->signal_matrix[row] = svr::conv1D_long_FFTPlanPool(
            this->signal_matrix[row],
            transmitter.signal,
            *(this->fph_ptr_sigsim),
            *(this->ifph_ptr_sigsim)
        );

        // only keeping first num-samples amount of samples
        this->signal_matrix[row].resize(this->num_samples);
    }

    // logging
    spdlog::info("Finished signal-simulation");}