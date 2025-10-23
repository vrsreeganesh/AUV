/*==============================================================================
Aim: Subsetting the scatterers to those inside the transmission cone
------------------------------------------------------------------------------*/
template    <typename   T>
auto TransmitterClass<T>::subset_scatterers(
    const ScattererClass<T>&    seafloor,
    std::vector<std::size_t>&   indices,
    const T&                    tilt_angle
){
    // translating the origin 
    auto seafloor_coordinates_origin_shifted    {seafloor.coordinates - \
                                                 transpose(this->location)};

    // finding spherical coordinates of scatterers and pointing direction
    auto scatterers_spherical           {svr::cart2sph(seafloor_coordinates_origin_shifted, 0)};
    auto pointing_direction_spherical   {svr::cart2sph(this->pointing_direction)};

    // calculating relative azimuths and radians
    auto& relative_spherical    {scatterers_spherical};
    relative_spherical          = scatterers_spherical - \
                                  transpose(pointing_direction_spherical);

    // resetting 
    svr::reset(pointing_direction_spherical);

    // calculating length of axes
    auto axis_a     {this->azimuthal_beamwidth/2};
    auto axis_b     {this->elevation_beamwidth/2};

    // finding the points inside the tilted ellipse
    auto scatter_boolean     {
        (svr::square(
            relative_spherical[0] * cos(tilt_angle * std::numbers::pi / 180.00)     + 
            relative_spherical[1] * sin(tilt_angle * std::numbers::pi / 180.00)    
        )/svr::square(axis_a) + 
        svr::square(
            relative_spherical[0] * sin(tilt_angle * std::numbers::pi / 180.00)     - 
            relative_spherical[1] * cos(tilt_angle * std::numbers::pi / 180.00)
        )/svr::square(axis_b)) <= 1
    };

    // subsetting points within the elliptical beam
    auto    mask                            {scatter_boolean == 1};
    auto    seafloor_mask_indices           {svr::mask_indices(mask)};

    // moving it back into the output
    indices = std::move(seafloor_mask_indices);

}