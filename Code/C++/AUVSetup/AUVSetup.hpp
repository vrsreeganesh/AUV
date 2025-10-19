template <
    svr::PureFloatingPointType          T,
    svr::FFT_SourceDestination_Type     sourceType,
    svr::FFT_SourceDestination_Type     destinationType,
    svr::PureComplexFloatingType        T_PureComplex
>
void fAUVSetup(
    AUVClass<T, sourceType, destinationType, T_PureComplex>& auv
) {

    // building properties for the auv
    auto    location            {std::vector<T>{0, 50, 30}};    // starting location
    auto    velocity            {std::vector<T>{5, 0, 0}};      // starting velocity
    auto    pointing_direction  {std::vector<T>{1, 0, 0}};      // pointing direction

    // assigning 
    auv.location            = std::move(location);              // assigning location
    auv.velocity            = std::move(velocity);              // assigning velocity
    auv.pointing_direction  = std::move(pointing_direction);    // assigning pointing direction

    // signaling end
    spdlog::info("Completed AUV-setup");
}