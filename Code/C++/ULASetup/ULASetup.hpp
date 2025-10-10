template    <
    typename    T,
    typename    = std::enable_if_t<
        std::is_same_v<T, double>    ||
        std::is_same_v<T, float>
    >
>
void fULASetup(
    ULAClass<T>&     ula_fls,
    ULAClass<T>&     ula_portside,
    ULAClass<T>&     ula_starboard)
{
    // setting up ula
    auto    num_sensors                 {static_cast<int>(16)};             // number of sensors
    T       sampling_frequency          {static_cast<T>(160e3)};            // sampling frequency
    T       inter_element_spacing       {1500/(2*sampling_frequency)};      // space between samples
    T       recording_period            {10e-2};                            // sampling-period

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

    // assigning values 
    ula_fls.num_sensors                                 = num_sensors;                  // assigning number of sensors
    ula_fls.inter_element_spacing                       = inter_element_spacing;        // assigning inter-element spacing
    ula_fls.coordinates                                 = ULA_coordinates;              // assigning ULA coordinates
    ula_fls.sampling_frequency                          = sampling_frequency;           // assigning sampling frequencys
    ula_fls.recording_period                            = recording_period;             // assigning recording period
    ula_fls.sensor_direction                            = ULA_direction;                // ULA direction 
    ula_fls.lowpass_filter_coefficients_for_decimation  = lowpassfiltercoefficients;    // storing coefficients


    // assigning values 
    ula_portside.num_sensors                                = num_sensors;                  // assigning number of sensors
    ula_portside.inter_element_spacing                      = inter_element_spacing;        // assigning inter-element spacing
    ula_portside.coordinates                                = ULA_coordinates;              // assigning ULA coordinates
    ula_portside.sampling_frequency                         = sampling_frequency;           // assigning sampling frequencys
    ula_portside.recording_period                           = recording_period;             // assigning recording period
    ula_portside.sensor_direction                           = ULA_direction;                // ULA direction
    ula_portside.lowpass_filter_coefficients_for_decimation = lowpassfiltercoefficients;    // storing coefficients


    // assigning values 
    ula_starboard.num_sensors                                   = num_sensors;                  // assigning number of sensors
    ula_starboard.inter_element_spacing                         = inter_element_spacing;        // assigning inter-element spacing
    ula_starboard.coordinates                                   = ULA_coordinates;              // assigning ULA coordinates
    ula_starboard.sampling_frequency                            = sampling_frequency;           // assigning sampling frequencys
    ula_starboard.recording_period                              = recording_period;             // assigning recording period
    ula_starboard.sensor_direction                              = ULA_direction;                // ULA direction 
    ula_starboard.lowpass_filter_coefficients_for_decimation    = lowpassfiltercoefficients;    // storing coefficients
}