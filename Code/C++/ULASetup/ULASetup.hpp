template <
    svr::PureFloatingPointType          T,
    svr::FFT_SourceDestination_Type     sourceType,
    svr::FFT_SourceDestination_Type     destinationType,
    svr::PureComplexFloatingType        T_PureComplex
>
void fULASetup(
    ULAClass<T, sourceType, destinationType, T_PureComplex>&     ula_fls,
    ULAClass<T, sourceType, destinationType, T_PureComplex>&     ula_portside,
    ULAClass<T, sourceType, destinationType, T_PureComplex>&     ula_starboard)
{
    // setting up ula
    auto    num_sensors                 {static_cast<int>(32)};
    T       sampling_frequency          {static_cast<T>(240e3)};
    spdlog::warn("Create better method for choosing system-wide sampling-frequency");
    T       inter_element_spacing       {1500/(2*sampling_frequency)};
    T       recording_period            {10e-2};
    auto    num_samples                 {static_cast<std::size_t>(
        std::ceil(
            sampling_frequency    *   recording_period
        )
    )};

    // building the direction for the sensors
    auto    ULA_direction               {std::vector<T>({-1, 0, 0})};
    auto    ULA_direction_norm          {norm(ULA_direction)};
    if (ULA_direction_norm != 0)        {ULA_direction = ULA_direction/ULA_direction_norm;}
    ULA_direction           =           ULA_direction    *   inter_element_spacing;

    // building coordinates for sensors
    auto    ULA_coordinates             {
        transpose(ULA_direction) * \
        svr::linspace<double>(
            0.00,
            num_sensors -1,
            num_sensors)
    };

    // coefficients of decimation filter
    auto    lowpassfiltercoefficients   {std::vector<T>{0.0000, 0.0000, 0.0000,  0.0000, 0.0000, 0.0000, 0.0001, 0.0003, 0.0006, 0.0015, 0.0030, 0.0057, 0.0100, 0.0163, 0.0251, 0.0364, 0.0501, 0.0654, 0.0814, 0.0966, 0.1093, 0.1180, 0.1212, 0.1179, 0.1078, 0.0914, 0.0699, 0.0451, 0.0192, -0.0053, -0.0262, -0.0416, -0.0504, -0.0522, -0.0475, -0.0375, -0.0239, -0.0088, 0.0057, 0.0179, 0.0263, 0.0303, 0.0298, 0.0253, 0.0177, 0.0086, -0.0008, -0.0091, -0.0153, -0.0187, -0.0191, -0.0168, -0.0123, -0.0065, -0.0004, 0.0052, 0.0095, 0.0119, 0.0125, 0.0112, 0.0084, 0.0046, 0.0006, -0.0031, -0.0060, -0.0078, -0.0082, -0.0075, -0.0057, -0.0033, -0.0006, 0.0019, 0.0039, 0.0051, 0.0055, 0.0050, 0.0039, 0.0023, 0.0005, -0.0012, -0.0025, -0.0034, -0.0036, -0.0034, -0.0026, -0.0016, -0.0004, 0.0007, 0.0016, 0.0022, 0.0024, 0.0023, 0.0018, 0.0011, 0.0003, -0.0004,  -0.0011, -0.0015, -0.0016, -0.0015}};






    /*==========================================================================
    Creating plan handles for signal-simulation
    --------------------------------------------------------------------------*/ 
    const   auto    num_plans_ula_signsim   {8};
    const   auto    fft_length_ula_sigsim   {16384};
    
    auto    fph_ptr_ula_fls_sigsim      {std::make_unique<
        svr::FFTPlanUniformPoolHandle< double,std::complex<double> >
    >(  num_plans_ula_signsim, fft_length_ula_sigsim   )};
    auto    ifph_ptr_ula_fls_sigsim     {std::make_unique<
        svr::IFFTPlanUniformPoolHandle<std::complex<double>,double>
    >(  num_plans_ula_signsim, fft_length_ula_sigsim   )};

    auto    fph_ptr_ula_portside_sigsim      {std::make_unique<
        svr::FFTPlanUniformPoolHandle< double,std::complex<double> >
    >(  num_plans_ula_signsim, fft_length_ula_sigsim   )};
    auto    ifph_ptr_ula_portside_sigsim     {std::make_unique<
        svr::IFFTPlanUniformPoolHandle<std::complex<double>,double>
    >(  num_plans_ula_signsim, fft_length_ula_sigsim   )};
    
    auto    fph_ptr_ula_starboard_sigsim      {std::make_unique<
        svr::FFTPlanUniformPoolHandle< double,std::complex<double> >
    >(  num_plans_ula_signsim, fft_length_ula_sigsim   )};
    auto    ifph_ptr_ula_starboard_sigsim     {std::make_unique<
        svr::IFFTPlanUniformPoolHandle<std::complex<double>,double>
    >(  num_plans_ula_signsim, fft_length_ula_sigsim   )};
    

    
    /*==========================================================================
    Creating plan handles for imaging
    --------------------------------------------------------------------------*/ 
    const   auto    num_plans_ula_imaging       {129};
    const   auto    fft_length_ula_imaging      {128};
    auto ula_fls_fph_ptr = std::make_unique<
        svr::FFTPlanUniformPoolHandle<  T_PureComplex,T_PureComplex>
        >(num_plans_ula_imaging, fft_length_ula_imaging);
    auto ula_portside_fph_ptr = std::make_unique<
        svr::FFTPlanUniformPoolHandle< 
            T_PureComplex,T_PureComplex>
        >(num_plans_ula_imaging, fft_length_ula_imaging);
    auto ula_starboard_fph_ptr = std::make_unique<
        svr::FFTPlanUniformPoolHandle<T_PureComplex,T_PureComplex>
        >(num_plans_ula_imaging, fft_length_ula_imaging);
    
    auto ula_fls_ifph_ptr = std::make_unique<
        svr::IFFTPlanUniformPoolHandle<T_PureComplex,T_PureComplex>
        >(num_plans_ula_imaging, fft_length_ula_imaging);
    auto ula_portside_ifph_ptr= std::make_unique<
        svr::IFFTPlanUniformPoolHandle<T_PureComplex,T_PureComplex>
        >(num_plans_ula_imaging, fft_length_ula_imaging);
    auto ula_starboard_ifph_ptr = std::make_unique<
        svr::IFFTPlanUniformPoolHandle<
            T_PureComplex,
            T_PureComplex
        >>(num_plans_ula_imaging, fft_length_ula_imaging);




    /*==========================================================================
    Attaching objects to the ULA
    --------------------------------------------------------------------------*/

    // assigning values 
    ula_fls.num_sensors                                 = num_sensors;
    ula_fls.inter_element_spacing                       = inter_element_spacing;
    ula_fls.coordinates                                 = ULA_coordinates;
    ula_fls.sampling_frequency                          = sampling_frequency;
    ula_fls.recording_period                            = recording_period;
    ula_fls.sensor_direction                            = ULA_direction;
    ula_fls.lowpass_filter_coefficients_for_decimation  = lowpassfiltercoefficients;
    ula_fls.num_samples                                 = num_samples;
    ula_fls.fph_image_ptr                               = std::move(ula_fls_fph_ptr);
    ula_fls.ifph_image_ptr                              = std::move(ula_fls_ifph_ptr);
    ula_fls.fph_ptr_sigsim                              = std::move(fph_ptr_ula_fls_sigsim);
    ula_fls.ifph_ptr_sigsim                             = std::move(ifph_ptr_ula_fls_sigsim);


    // assigning values 
    ula_portside.num_sensors                                    = num_sensors;
    ula_portside.inter_element_spacing                          = inter_element_spacing;
    ula_portside.coordinates                                    = ULA_coordinates;
    ula_portside.sampling_frequency                             = sampling_frequency;
    ula_portside.recording_period                               = recording_period;
    ula_portside.sensor_direction                               = ULA_direction;   
    ula_portside.lowpass_filter_coefficients_for_decimation     = lowpassfiltercoefficients;
    ula_portside.num_samples                                    = num_samples;
    ula_portside.fph_image_ptr                                  = std::move(ula_portside_fph_ptr);
    ula_portside.ifph_image_ptr                                 = std::move(ula_portside_ifph_ptr);
    ula_portside.fph_ptr_sigsim                                 = std::move(fph_ptr_ula_portside_sigsim);
    ula_portside.ifph_ptr_sigsim                                = std::move(ifph_ptr_ula_portside_sigsim);


    // assigning values 
    ula_starboard.num_sensors                                   = num_sensors;
    ula_starboard.inter_element_spacing                         = inter_element_spacing;
    ula_starboard.coordinates                                   = ULA_coordinates;
    ula_starboard.sampling_frequency                            = sampling_frequency;
    ula_starboard.recording_period                              = recording_period;
    ula_starboard.sensor_direction                              = ULA_direction;
    ula_starboard.lowpass_filter_coefficients_for_decimation    = lowpassfiltercoefficients;
    ula_starboard.num_samples                                   = num_samples;
    ula_starboard.fph_image_ptr                                 = std::move(ula_starboard_fph_ptr);
    ula_starboard.ifph_image_ptr                                = std::move(ula_starboard_ifph_ptr);
    ula_starboard.fph_ptr_sigsim                                 = std::move(fph_ptr_ula_starboard_sigsim);
    ula_starboard.ifph_ptr_sigsim                                = std::move(ifph_ptr_ula_starboard_sigsim);
}