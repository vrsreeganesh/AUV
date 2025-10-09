#pragma once

/*==============================================================================
Dependencies
------------------------------------------------------------------------------*/ 
#include "IFFTPlanPoolClass.hpp"


namespace   svr
{
    template    <
        typename    sourceType,
        typename    destinationType,
        typename    =   std::enable_if_t<
            std::is_same_v<     sourceType,         std::complex<double>  >    &&
            std::is_same_v<     destinationType,    double   >
        >
    >
    struct  IFFTPlanUniformPoolHandle
    {
        /*======================================================================
        Members
        ----------------------------------------------------------------------*/ 
        IFFTPlanUniformPool<    sourceType,
                                destinationType     >       uniform_pool;
        std::mutex                                          mutex;
        std::size_t                                         num_plans;
        std::size_t                                         nfft;

        /*======================================================================
        Special Member Functions
        ----------------------------------------------------------------------*/
        IFFTPlanUniformPoolHandle()     =   default;
        IFFTPlanUniformPoolHandle(const std::size_t num_plans_arg,
                                  const std::size_t nfft_arg)
            :   uniform_pool(           num_plans_arg, nfft_arg),
                num_plans(              num_plans_arg),
                nfft(                   nfft_arg)   {}
        IFFTPlanUniformPoolHandle(const IFFTPlanUniformPoolHandle& other)               = delete;
        IFFTPlanUniformPoolHandle& operator=(const IFFTPlanUniformPoolHandle& other)    = delete;
        IFFTPlanUniformPoolHandle(IFFTPlanUniformPoolHandle&& other)                    = delete;
        IFFTPlanUniformPoolHandle& operator=(IFFTPlanUniformPoolHandle&& other)         = delete;

        /*======================================================================
        Member Functions
        ----------------------------------------------------------------------*/ 
        auto    lock()
        {
            return std::unique_lock<std::mutex>(this->mutex);
        }
        
    };
}