#pragma once
namespace   svr {

    /*==========================================================================
    --------------------------------------------------------------------------*/ 
    // template    <
    //     typename   T,
    //     typename    = std::enable_if_t<
    //         std::is_floating_point_v<T> ||
    //         (
    //             std::is_class_v<T>                          &&
    //             requires    {typename   T::value_type;}      &&
    //             std::is_floating_point_v<typename   T::value_type>
    //         )
    //     >
    // >
    // template <typename T,
    //       typename = std::enable_if_t<
    //           std::is_floating_point_v<T> ||
    //           (std::is_class_v<T> &&
    //            requires { typename T::value_type; } &&
    //            std::is_floating_point_v<typename T::value_type>)
    //       >>
    template    <typename   T>
    auto    complex(
        const   std::vector<T>&     input_vector
    )
    {
        // checking if it is complex
        if constexpr(
            std::is_same_v<T, std::complex<double>> ||
            std::is_same_v<T,   std::complex<float>>
        ){
            return input_vector;
        }
        else {
            // creating canvas and filling 
            auto    canvas      {std::vector<std::complex<T>>(input_vector.size())};
            std::transform(
                input_vector.begin(),   input_vector.end(),
                canvas.begin(),
                [](const    auto&   argx){
                    return std::complex<T>(argx, 0);
                }
            );
            return std::move(canvas);
        }

    }

}