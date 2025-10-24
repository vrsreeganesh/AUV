#pragma once

/*==============================================================================
Dependencies
------------------------------------------------------------------------------*/ 
#include "IFFTPlanPoolClass.hpp"


namespace   svr
{
    template    <typename   T>
    concept     IFFTPlanUniformPoolHandleSourceDestinationType  =   \
        std::is_floating_point_v<T>     ||
        (
            std::is_class_v<T>  &&
            std::is_floating_point_v<typename   T::value_type>
        );
    template    <
        IFFTPlanUniformPoolHandleSourceDestinationType  sourceType,
        IFFTPlanUniformPoolHandleSourceDestinationType  destinationType
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
        IFFTPlanUniformPoolHandle(              const IFFTPlanUniformPoolHandle& other  ) = delete;
        IFFTPlanUniformPoolHandle& operator=(   const IFFTPlanUniformPoolHandle& other  ) = delete;
        IFFTPlanUniformPoolHandle(              IFFTPlanUniformPoolHandle&& other       ) = default;
        IFFTPlanUniformPoolHandle& operator=(   IFFTPlanUniformPoolHandle&& other       ) = default;

        /*======================================================================
        Member Functions
        ----------------------------------------------------------------------*/ 
        auto    lock()
        {
            return std::unique_lock<std::mutex>(this->mutex);
        }
        
    };
}