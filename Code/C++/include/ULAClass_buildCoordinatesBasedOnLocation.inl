/* =============================================================================
Aim: Build Coordinates Based On Location
------------------------------------------------------------------------------*/
template <
    svr::PureFloatingPointType          T,
    svr::FFT_SourceDestination_Type     sourceType,
    svr::FFT_SourceDestination_Type     destinationType,
    svr::PureComplexFloatingType        T_PureComplex
>
void    ULAClass<T, sourceType, destinationType, T_PureComplex>::buildCoordinatesBasedOnLocation(){
    // length-normalizing sensor-direction
    this->sensor_direction      =   this->sensor_direction / norm(this->sensor_direction);

    // multiply with inter-element distance
    auto    inter_element_vector    {this->sensor_direction * \
                                     this->inter_element_spacing};
    
    // create integer array (verified)
    auto    integer_array       {svr::linspace<T>(
        static_cast<T>(0), 
        static_cast<T>(this->num_sensors-1),
        this->num_sensors
    )};

    auto    test    {svr::tile(integer_array,   {3,1})  *  \
                     svr::tile(transpose(inter_element_vector),   
                               {1, static_cast<std::size_t>(this->num_sensors)})};
    
    // translating coordinates
    this->coordinates   =  transpose( this->location)  +  test;
}
/*==============================================================================
------------------------------------------------------------------------------*/ 
template <
    svr::PureFloatingPointType          T,
    svr::FFT_SourceDestination_Type     sourceType,
    svr::FFT_SourceDestination_Type     destinationType,
    svr::PureComplexFloatingType        T_PureComplex
>
void    ULAClass<T, sourceType, destinationType, T_PureComplex>::buildCoordinatesBasedOnLocation(
    const  std::vector<T>&  new_location
){
    // updating location
    this->location  = new_location;

    // calling the update-coordinates-function
    this->buildCoordinatesBasedOnLocation();
}