#pragma once

/*==============================================================================
Dependencies
------------------------------------------------------------------------------*/ 
#include "FFTPlanPoolClass.hpp"

namespace   svr
{
    template    <
        typename    sourceType,
        typename    destinationType,
        typename    =   std::enable_if_t<
            std::is_same_v<     sourceType, double  >    &&
            std::is_same_v<     destinationType, std::complex<double>   >
        >
    >
    struct FFTPlanUniformPoolHandle
    {
        /*======================================================================
        Core Members
        ----------------------------------------------------------------------*/ 
        FFTPlanUniformPool<sourceType, destinationType>     uniform_pool;
        std::mutex                                          mutex;
        std::size_t                                         num_plans;
        std::size_t                                         nfft;

        /*======================================================================
        Special Member-functions
        ----------------------------------------------------------------------*/ 
        FFTPlanUniformPoolHandle()      =   default;
        FFTPlanUniformPoolHandle(const  std::size_t num_plans_arg,
                                 const  std::size_t nfft_arg)
            :   uniform_pool(num_plans_arg, nfft_arg),
                num_plans(num_plans_arg),
                nfft(nfft_arg)    {}
        FFTPlanUniformPoolHandle(const FFTPlanUniformPoolHandle& other)             = delete;
        FFTPlanUniformPoolHandle& operator=(const FFTPlanUniformPoolHandle& other)  = delete;
        FFTPlanUniformPoolHandle(FFTPlanUniformPoolHandle&& other)                  = delete;
        FFTPlanUniformPoolHandle& operator=(FFTPlanUniformPoolHandle&& other)       = delete;

        /*======================================================================
        Member Functions
        ----------------------------------------------------------------------*/ 
        auto    lock()
        {
            return std::unique_lock<std::mutex>(this->mutex);
        }
    };
}

    