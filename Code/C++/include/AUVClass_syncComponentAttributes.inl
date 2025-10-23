/*==========================================================================
Aim: update attributes
--------------------------------------------------------------------------*/
template <
    svr::PureFloatingPointType          T,
    svr::FFT_SourceDestination_Type     sourceType,
    svr::FFT_SourceDestination_Type     destinationType,
    svr::PureComplexFloatingType        T_PureComplex
>
void AUVClass<T, sourceType, destinationType, T_PureComplex>::syncComponentAttributes(){
    // updating locations of ULAs
    this->ULA_fls.location          = this->location;
    this->ULA_portside.location     = this->location;
    this->ULA_starboard.location    = this->location;
    
    // updating pointing-direction of ULAs
    auto    ula_fls_sensor_direction_spherical      {svr::cart2sph(this->pointing_direction)};
    ula_fls_sensor_direction_spherical[0]           -=  90;
    auto    ula_fls_sensor_direction_cart           {svr::sph2cart(ula_fls_sensor_direction_spherical)};

    this->ULA_fls.sensor_direction          =   ula_fls_sensor_direction_cart;
    this->ULA_portside.sensor_direction     =   -1  *   this->pointing_direction;
    this->ULA_starboard.sensor_direction    =   -1  *   this->pointing_direction;

    // calling function to update arguments (verified ula-coordinates)
    this->ULA_fls.buildCoordinatesBasedOnLocation(          this->location);
    this->ULA_portside.buildCoordinatesBasedOnLocation(     this->location);
    this->ULA_starboard.buildCoordinatesBasedOnLocation(    this->location);

    // updating transmitter location
    this->transmitter_fls.location                  =   this->location;
    this->transmitter_portside.location             =   this->location;
    this->transmitter_starboard.location            =   this->location;

    // updating transmitter pointing direction
    this->transmitter_fls.updatePointingAngle(          this->pointing_direction);
    this->transmitter_portside.updatePointingAngle(     this->pointing_direction);
    this->transmitter_starboard.updatePointingAngle(    this->pointing_direction);
}