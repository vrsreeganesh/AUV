/*==============================================================================
Aim: Moving the AUV to the next discrete position in the trajectory
------------------------------------------------------------------------------*/ 
template <
    svr::PureFloatingPointType          T,
    svr::FFT_SourceDestination_Type     sourceType,
    svr::FFT_SourceDestination_Type     destinationType,
    svr::PureComplexFloatingType        T_PureComplex
>
void AUVClass<T, sourceType, destinationType, T_PureComplex>::step(
    T    time_step
){
    // updating location
    this->location  = this->location + this->velocity * time_step;

    // update parameters of attached components
    this->syncComponentAttributes();
}