#pragma once

/*==============================================================================
Dependencies
------------------------------------------------------------------------------*/ 
#include "FFTPlanPoolClass.hpp"

namespace   svr
{
    template    <typename   T>
    concept     FFTPlanUniformPoolHandleSourceDestinationType    =   \
        std::is_floating_point_v<T>     ||
        (
            std::is_class_v<T>          &&
            std::is_floating_point_v<typename   T::value_type>
        );
    template    <
        FFTPlanUniformPoolHandleSourceDestinationType   sourceType,
        FFTPlanUniformPoolHandleSourceDestinationType   destinationType
    >
    struct FFTPlanUniformPoolHandle
    {
        /*======================================================================
        Core Members
        ----------------------------------------------------------------------*/ 
        svr::FFTPlanUniformPool<sourceType, destinationType>    uniform_pool;
        std::mutex                                              mutex;
        std::size_t                                             num_plans;
        std::size_t                                             nfft;

        /*======================================================================
        Special Member-functions
        ----------------------------------------------------------------------*/ 
        FFTPlanUniformPoolHandle()      =   default;
        FFTPlanUniformPoolHandle(const  std::size_t num_plans_arg,
                                 const  std::size_t nfft_arg)
            :   uniform_pool(num_plans_arg, nfft_arg),
                num_plans(num_plans_arg),
                nfft(nfft_arg)    {}
        FFTPlanUniformPoolHandle(               const FFTPlanUniformPoolHandle& other   ) = delete;
        FFTPlanUniformPoolHandle& operator=(    const FFTPlanUniformPoolHandle& other   ) = delete;
        FFTPlanUniformPoolHandle(               FFTPlanUniformPoolHandle&& other        ) = default;
        FFTPlanUniformPoolHandle& operator=(    FFTPlanUniformPoolHandle&& other        ) = default;

        /*======================================================================
        Member Functions
        ----------------------------------------------------------------------*/ 
        auto    lock()
        {
            return std::unique_lock<std::mutex>(this->mutex);
        }
    };
}

    