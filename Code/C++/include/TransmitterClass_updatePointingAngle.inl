/*==========================================================================
Aim: Update pointing angle
----------------------------------------------------------------------------
Note:
    > This function updates pointing angle based on AUV's pointing angle
    > for now, we're assuming no roll;
--------------------------------------------------------------------------*/
template <typename T>
auto TransmitterClass<T>::updatePointingAngle(
    std::vector<T> AUV_pointing_vector
){
    
    // calculate yaw and pitch
    auto    AUV_pointing_vector_spherical   {svr::cart2sph(AUV_pointing_vector)};
    auto    yaw                             {AUV_pointing_vector_spherical[0]};
    auto    pitch                           {AUV_pointing_vector_spherical[1]};
    
    // calculating azimuth and elevation of transmitter object
    auto    absolute_azimuth_of_transmitter     {yaw + this->azimuthal_angle};
    auto    absolute_elevation_of_transmitter   {pitch + this->elevation_angle};

    // converting back to Cartesian
    auto    pointing_direction_spherical        {std::vector<T>(3, 0)};
    pointing_direction_spherical[0]             = absolute_azimuth_of_transmitter;
    pointing_direction_spherical[1]             = absolute_elevation_of_transmitter; 
    pointing_direction_spherical[2]             = 1;
    this->pointing_direction                    = svr::sph2cart(pointing_direction_spherical);
}