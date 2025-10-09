template    <
    typename    T,
    typename    =   std::enable_if_t<
        std::is_same_v<T, double>    || 
        std::is_same_v<T, float>
    >
>
void fTransmitterSetup(
    TransmitterClass<T>&    transmitter_fls,
    TransmitterClass<T>&    transmitter_portside,
    TransmitterClass<T>&    transmitter_starboard
){
    // Setting up transmitter
    T       sampling_frequency      {160e3};                // sampling frequency
    T       f1                      {50e3};                 // first frequency of LFM
    T       f2                      {70e3};                 // second frequency of LFM
    T       fc                      {(f1 + f2)/2.00};       // finding center-frequency
    T       bandwidth               {std::abs(f2 - f1)};    // bandwidth 
    T       pulselength             {5e-2};                 // time of recording

    // building LFM
    auto    timearray       {linspace<T>(0.00,
                                         pulselength,
                                         std::floor(pulselength * sampling_frequency))};
    auto    K               {f2 - f1/pulselength};    // calculating frequency-slope
    auto    Signal          {cos(2 * std::numbers::pi * \
                             (f1 + K*timearray) * \
                             timearray)};            // frequency at each time-step, with f1 = 0

    // Setting up transmitter
    auto    location                            {std::vector<T>(3, 0)};         // location of transmitter
    T       azimuthal_angle_fls                 {0};                            // initial pointing direction
    T       azimuthal_angle_port                {90};                           // initial pointing direction
    T       azimuthal_angle_starboard           {-90};                          // initial pointing direction
    
    T       elevation_angle                     {-60};                          // initial pointing direction
    
    T       azimuthal_beamwidth_fls             {20};                           // azimuthal beamwidth of the signal cone
    T       azimuthal_beamwidth_port            {20};                           // azimuthal beamwidth of the signal cone
    T       azimuthal_beamwidth_starboard       {20};                           // azimuthal beamwidth of the signal cone
    
    T       elevation_beamwidth_fls             {20};                           // elevation beamwidth of the signal cone
    T       elevation_beamwidth_port            {20};                           // elevation beamwidth of the signal cone
    T       elevation_beamwidth_starboard       {20};                           // elevation beamwidth of the signal cone
    
    int     azimuthQuantDensity                 {10};   // number of points, a degree is split into quantization density along azimuth (used for shadowing)
    int     elevationQuantDensity               {10};   // number of points, a degree is split into quantization density along elevation (used for shadowing)
    T       rangeQuantSize                      {10};   // the length of a cell (used for shadowing)

    T       azimuthShadowThreshold              {1};    // azimuth threshold      (in degrees)
    T       elevationShadowThreshold            {1};    // elevation threshold    (in degrees)


    // transmitter-fls
    transmitter_fls.location                = location;                         // Assigning location
    transmitter_fls.signal                  = Signal;                           // Assigning signal
    transmitter_fls.azimuthal_angle         = azimuthal_angle_fls;              // assigning azimuth angle
    transmitter_fls.elevation_angle         = elevation_angle;                  // assigning elevation angle
    transmitter_fls.azimuthal_beamwidth     = azimuthal_beamwidth_fls;          // assigning azimuth-beamwidth
    transmitter_fls.elevation_beamwidth     = elevation_beamwidth_fls;          // assigning elevation-beamwidth
    // updating quantization densities
    transmitter_fls.azimuthQuantDensity        = azimuthQuantDensity;        // assigning azimuth quant density
    transmitter_fls.elevationQuantDensity      = elevationQuantDensity;      // assigning elevation quant density
    transmitter_fls.rangeQuantSize             = rangeQuantSize;             // assigning range-quantization
    transmitter_fls.azimuthShadowThreshold     = azimuthShadowThreshold;     // azimuth-threshold in shadowing
    transmitter_fls.elevationShadowThreshold   = elevationShadowThreshold;   // elevation-threshold in shadowing
    // signal related
    transmitter_fls.f_low                      = f1;           // assigning lower frequency
    transmitter_fls.f_high                     = f2;           // assigning higher frequency
    transmitter_fls.fc                         = fc;           // assigning center frequency
    transmitter_fls.bandwidth                  = bandwidth;    // assigning bandwidth


    // transmitter-portside
    transmitter_portside.location                  = location;                     // Assigning location
    transmitter_portside.signal                    = Signal;                       // Assigning signal
    transmitter_portside.azimuthal_angle           = azimuthal_angle_port;         // assigning azimuth angle
    transmitter_portside.elevation_angle           = elevation_angle;              // assigning elevation angle
    transmitter_portside.azimuthal_beamwidth       = azimuthal_beamwidth_port;     // assigning azimuth-beamwidth
    transmitter_portside.elevation_beamwidth       = elevation_beamwidth_port;     // assigning elevation-beamwidth
    // updating quantization densities
    transmitter_portside.azimuthQuantDensity       = azimuthQuantDensity;          // assigning azimuth quant density
    transmitter_portside.elevationQuantDensity     = elevationQuantDensity;        // assigning elevation quant density
    transmitter_portside.rangeQuantSize            = rangeQuantSize;               // assigning range-quantization
    transmitter_portside.azimuthShadowThreshold    = azimuthShadowThreshold;       // azimuth-threshold in shadowing
    transmitter_portside.elevationShadowThreshold  = elevationShadowThreshold;     // elevation-threshold in shadowing
    // signal related
    transmitter_portside.f_low                     = f1;                           // assigning lower frequency
    transmitter_portside.f_high                    = f2;                           // assigning higher frequency
    transmitter_portside.fc                        = fc;                           // assigning center frequency
    transmitter_portside.bandwidth                 = bandwidth;                    // assigning bandwidth


    // transmitter-starboard
    transmitter_starboard.location                     = location;                     // assigning location
    transmitter_starboard.signal                       = Signal;                       // assigning signal
    transmitter_starboard.azimuthal_angle              = azimuthal_angle_starboard;    // assigning azimuthal signal
    transmitter_starboard.elevation_angle              = elevation_angle;
    transmitter_starboard.azimuthal_beamwidth          = azimuthal_beamwidth_starboard;
    transmitter_starboard.elevation_beamwidth          = elevation_beamwidth_starboard;
    // updating quantization densities
    transmitter_starboard.azimuthQuantDensity          = azimuthQuantDensity;           // assigning azimuth-quant-density
    transmitter_starboard.elevationQuantDensity        = elevationQuantDensity;         
    transmitter_starboard.rangeQuantSize               = rangeQuantSize;
    transmitter_starboard.azimuthShadowThreshold       = azimuthShadowThreshold;
    transmitter_starboard.elevationShadowThreshold     = elevationShadowThreshold;
    // signal related
    transmitter_starboard.f_low                        = f1;                            // assigning lower frequency
    transmitter_starboard.f_high                       = f2;                            // assigning higher frequency
    transmitter_starboard.fc                           = fc;                            // assigning center frequency
    transmitter_starboard.bandwidth                    = bandwidth;                     // assigning bandwidth
    
}