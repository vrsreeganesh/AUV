/*==============================================================================
Dependencies
------------------------------------------------------------------------------*/ 
#pragma once
#include <vector>
#include <complex>

namespace   svr {
    /*==============================================================================
    in-place
    ------------------------------------------------------------------------------*/ 
    template <typename T>
    auto linspace(
        auto&             input,
        const     auto    startvalue,
        const     auto    endvalue,
        const     auto    numpoints
    ) -> void
    {
        auto stepsize = static_cast<T>(endvalue - startvalue)/static_cast<T>(numpoints-1);
        for(int i = 0; i<input.size(); ++i) {input[i] = startvalue + i*stepsize;}
    };
    /*==============================================================================
    in-place
    ------------------------------------------------------------------------------*/ 
    template <typename T>
    auto linspace(
        std::vector<std::complex<T>>&     input,
        const     auto                    startvalue,
        const     auto                    endvalue,
        const     auto                    numpoints
    ) -> void
    {
        auto stepsize = static_cast<T>(endvalue - startvalue)/static_cast<T>(numpoints-1);
        for(int i = 0; i<input.size(); ++i) {
            input[i] = startvalue + static_cast<T>(i)*stepsize;
        }
    };
    /*==============================================================================
    ------------------------------------------------------------------------------*/ 
    template <
        typename T,
        typename    =   std::enable_if_t<
            std::is_arithmetic_v<T> ||
            std::is_same_v<T,   std::complex<float>  >  ||
            std::is_same_v<T,   std::complex<double>    >
        >
    >
    auto linspace(
        const     T               startvalue,
        const     T               endvalue,
        const     std::size_t     numpoints
    )
    {
        std::vector<T> input(numpoints);
        auto stepsize = static_cast<T>(endvalue - startvalue)/static_cast<T>(numpoints-1);
        for(int i = 0; i<input.size(); ++i) {input[i] = startvalue + static_cast<T>(i)*stepsize;}
        return std::move(input);
    };
    /*==============================================================================
    ------------------------------------------------------------------------------*/ 
    template <typename T, typename U>
    auto linspace(
        const     T               startvalue,
        const     U               endvalue,
        const     std::size_t     numpoints
    )
    {
        std::vector<double> input(numpoints);
        auto stepsize = static_cast<double>(endvalue - startvalue)/static_cast<double>(numpoints-1);
        for(int i = 0; i<input.size(); ++i) {input[i] = startvalue + i*stepsize;}
        return std::move(input);
    };
}
