#pragma once
namespace   svr {

    /*==========================================================================
    FFT Source Destination Type
    --------------------------------------------------------------------------*/
    template    <typename T>
    concept     FFT_SourceDestination_Type    =   \
        std::is_floating_point_v<T>     ||
        (
            std::is_class_v<T>                      &&
            requires    {typename   T::value_type;} &&
            std::is_floating_point_v<typename   T::value_type>
        );
    /*==========================================================================
    Floating type or its complex variants
    --------------------------------------------------------------------------*/
    template    <typename   T>
    concept     PureFloatingPointType   =   \
        std::is_floating_point_v<T>;
    /*==========================================================================
    Pure Complex-Floating type
    --------------------------------------------------------------------------*/
    template    <typename   T>
    concept     PureComplexFloatingType =   \
        (
            std::is_class_v<T>          &&
            requires    {typename   T::value_type;} &&
            std::is_floating_point_v<typename   T::value_type>
        );
    /*==========================================================================
    --------------------------------------------------------------------------*/
    template    <typename   T>
    concept     PureOrComplexFloatingType   =   \
        std::is_floating_point_v<T>     ||
        (
            std::is_class_v<T>                      &&
            requires    {typename   T::value_type;} &&
            std::is_floating_point_v<typename   T::value_type>
        );
}