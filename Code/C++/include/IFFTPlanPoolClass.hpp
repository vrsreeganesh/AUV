#pragma once

/*==============================================================================
Dependencies
------------------------------------------------------------------------------*/ 

namespace   svr     {
    
    template    <typename   T>
    concept     IFFTPlanUniformPoolSourceDestinationType    =   \
        std::is_floating_point_v<T>     ||
        (
            std::is_class_v<T>  &&
            std::is_floating_point_v<typename   T::value_type>
        );
    template    <
        IFFTPlanUniformPoolSourceDestinationType    sourceType,
        IFFTPlanUniformPoolSourceDestinationType    destinationType
    >
    class IFFTPlanUniformPool
    {
        public:
            /*==================================================================
            Structure used for interfacing to plans
            ------------------------------------------------------------------*/ 
            struct  AccessPairs
            {
                /*==============================================================
                Core Members
                --------------------------------------------------------------*/ 
                svr::IFFTPlanClass<sourceType, destinationType>&        plan;
                std::unique_lock<std::mutex>                            lock;

                /*==============================================================
                Special Members
                --------------------------------------------------------------*/ 
                AccessPairs()                                           =   delete;
                AccessPairs(
                    svr::IFFTPlanClass<sourceType, destinationType>&    plan_arg,
                    std::mutex&                                         plan_mutex_arg
                ):  plan(plan_arg), lock(plan_mutex_arg)    {}
                AccessPairs(
                    svr::IFFTPlanClass<sourceType, destinationType>&    plan_arg,
                    std::unique_lock<std::mutex>&&                      lock_arg
                ):  plan(plan_arg), lock(std::move(lock_arg))   {}
                AccessPairs(const   AccessPairs&    other)              =  delete;
                AccessPairs&    operator=(const AccessPairs& other)     =  delete;
                AccessPairs(AccessPairs&&   other)                      =  delete;
                AccessPairs&    operator=(AccessPairs&& other)          =  delete;
            };

            /*==================================================================
            Core Members
            ------------------------------------------------------------------*/
            std::vector<    svr::IFFTPlanClass<sourceType, destinationType> >   plans;
            std::vector<    std::mutex                                      >   mutexes;

            /*==================================================================
            Special Members
            ------------------------------------------------------------------*/
            IFFTPlanUniformPool()                           =   default;
            IFFTPlanUniformPool(const   std::size_t num_plans,
                                const   std::size_t nfft)
            {
                // reserving space
                plans.reserve(num_plans);
                for(auto i = 0; i < num_plans; ++i)
                    plans.emplace_back(nfft);

                // creating vector of mutexes
                mutexes     =   std::vector<std::mutex>(num_plans);
            }
            IFFTPlanUniformPool(const   IFFTPlanUniformPool&    other)              =   delete;
            IFFTPlanUniformPool&    operator=(const IFFTPlanUniformPool&    other)  =   delete;
            IFFTPlanUniformPool(IFFTPlanUniformPool&&   other)                      =   default;
            IFFTPlanUniformPool&    operator=(IFFTPlanUniformPool&& other)          =   default;

            /*==================================================================
            Member-Functions
            ------------------------------------------------------------------*/ 
            AccessPairs fetch_plan()
            {
                // setting the number of rounds to take
                const   int num_rounds  {12};

                // performing rounds
                for(auto round = 0; round < num_rounds; ++round)
                {
                    // going through vector mutexes 
                    for(auto i =0; i < mutexes.size(); ++i)
                    {
                        // trying to lock current mutex
                        std::unique_lock<std::mutex>    curr_lock(mutexes[i], std::try_to_lock);

                        // if our lock contains the mutex, returning the plan and lock
                        if (curr_lock.owns_lock())
                            return AccessPairs(plans[i], std::move(curr_lock));   
                    }
                }

                // throwing error
                throw std::runtime_error("FILE: IFFTPlanPoolClass.hpp | CLASS: IFFTPlanUniformPool | REPORT: COULDN'T FIND ANY AVAILABLE PLANS");
            }
    };
}
