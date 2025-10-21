#pragma once

/*==============================================================================
Includes
------------------------------------------------------------------------------*/
#include "svr_concepts.hpp"

namespace   svr {

    /*==========================================================================
    --------------------------------------------------------------------------*/ 
    template    <
        svr::PureOrComplexFloatingType   T0,
        typename                    T1,
        typename    =   std::enable_if_t<
            std::is_integral_v<T1>
        >
    >
    auto    sample(
        const    std::vector<T0>&    input_vector,
        const    T1                  starting_point,
        const    T1                  constant_difference)
    {
        // trivial-cases
        if (input_vector.size() == 0)   {return input_vector;}

        // error-checks
        if (starting_point >= input_vector.size())      {
            throw std::runtime_error(
                "FILE: svr_sample.hpp | FUNCTION: sample | REPORT: starting-point >= input-vector-size"
            );
        }
        if (constant_difference < 0){
            throw std::runtime_error(
                "FILE: svr_sample.hpp | FUNCTION: sample | REPORT: constant-difference < 0"
            );
        }

        // calculating finaloutput-dimensions
        auto    canvas_size     {1 + std::floor(
            (input_vector.size() - static_cast<std::size_t>(starting_point))/constant_difference
        )};
        auto    canvas      {std::vector<T0>(canvas_size)};

        // writing values
        for(auto i = 0; i < canvas_size; ++i){
            const   auto        source_index        {starting_point + i * constant_difference};
            const   auto&       destination_index   {i};
            canvas[destination_index]   =   input_vector[source_index];
        }

        // returning the output
        return std::move(canvas);
    }
    /*==========================================================================
    Matrix-version
    --------------------------------------------------------------------------*/ 
    template    <
        svr::PureOrComplexFloatingType  T0,
        typename                        T1,
        typename    =   std::enable_if_t<
            std::is_integral_v<T1>
        >
    >
    auto    sample(
        const   std::vector<std::vector<T0>>&   input_matrix,
        const   T1                              starting_index,
        const   T1                              constant_difference,
        const   T1                              dim
    )
    {
        // trivial-cases
        // error-checks
        if (dim != 0)   {throw std::runtime_error(
            "FILE: svr_sample.hpp | FUNCTION: sample matrix-version | REPORT: dim-argument is not implemented"
        );}

        // setup
        auto    canvas      {std::vector<std::vector<T0>>()};

        if (dim == 0){
            
            // calculating finaloutput-dimensions
            auto    canvas_num_cols     {1 + std::floor(
                (
                    input_matrix[0].size() - \
                    static_cast<std::size_t>(starting_index)
                )/constant_difference
            )};
            canvas  =   std::vector<std::vector<T0>>(
                input_matrix.size(),
                std::vector<T0>(canvas_num_cols)
            );

            // writing values
            for(auto row = 0; row < input_matrix.size(); ++row){
                for(auto i = 0; i < canvas_num_cols; ++i){
                    const   auto        source_index        {starting_index + i * constant_difference};
                    const   auto&       destination_index   {i};
                    canvas[row][destination_index]   =   input_matrix[row][source_index];
                }
            }
        }


        // returning
        return std::move(canvas);

    }


}