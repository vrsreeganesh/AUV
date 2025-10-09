template <
    typename    T,
    typename    =   std::enable_if_t<
        std::is_same_v<T, double>    || 
        std::is_same_v<T, float>
    >
>
void fAUVSetup(AUVClass<T>& auv) {

    // building properties for the auv
    auto    location            {std::vector<T>{0, 50, 30}};    // starting location
    auto    velocity            {std::vector<T>{5, 0, 0}};      // starting velocity
    auto    pointing_direction  {std::vector<T>{1, 0, 0}};      // pointing direction

    // assigning 
    auv.location            = std::move(location);              // assigning location
    auv.velocity            = std::move(velocity);              // assigning velocity
    auv.pointing_direction  = std::move(pointing_direction);    // assigning pointing direction

    // signaling end
    std::cout << format("> Completed AUV-setup\n");
    
}