#pragma once
namespace   svr     {
    template    <typename   T>
    concept FFTPlanUniformPoolSourceDestinationType =   \
        std::is_floating_point_v<T>     ||
        (
            std::is_class_v<T>          &&
            std::is_floating_point_v<typename   T::value_type>
        );
    template    <
        FFTPlanUniformPoolSourceDestinationType     sourceType,
        FFTPlanUniformPoolSourceDestinationType     destinationType
    >
    class FFTPlanUniformPool {
    public:
        /*======================================================================
        Handle to Plan
        ----------------------------------------------------------------------*/ 
        struct AccessPairs
        {
            /*==================================================================
            Members
            ------------------------------------------------------------------*/ 
            svr::FFTPlanClass<sourceType, destinationType>&     plan;
            std::unique_lock<std::mutex>                        lock;

            /*==================================================================
            Special Members
            ------------------------------------------------------------------*/ 
            AccessPairs() = delete;
            AccessPairs(
                svr::FFTPlanClass<sourceType, destinationType>& plan_arg,
                std::mutex&                                     plan_mutex
            )   :   plan(plan_arg), lock(plan_mutex)    {}
            AccessPairs(
                svr::FFTPlanClass<sourceType, destinationType>&     plan_arg,
                std::unique_lock<std::mutex>&&                      lock_arg
            ):  plan(plan_arg), lock(std::move(lock_arg))   {}
            AccessPairs(const   AccessPairs&    other)              =   delete;
            AccessPairs&    operator=(const AccessPairs&    other)  =   delete;
            AccessPairs(AccessPairs&&   other)                      =   delete;
            AccessPairs&    operator=(AccessPairs&& other)          =   delete;
        };

        /*======================================================================
        Core Members
        ----------------------------------------------------------------------*/ 
        std::vector<svr::FFTPlanClass<sourceType, destinationType>> plans;
        std::vector<std::mutex>                                     mutexes;

        /*======================================================================
        Special-Members
        ----------------------------------------------------------------------*/ 
        FFTPlanUniformPool()                                            =   default;
        FFTPlanUniformPool(const    std::size_t     num_plans,
                           const    std::size_t     nfft)
        {
            // reserving space
            plans.reserve(num_plans);
            for(auto i = 0; i < num_plans; ++i){
                plans.emplace_back(nfft);
            }

            // creating a vector of mutexes
            mutexes = std::move(std::vector<std::mutex>(num_plans));
        }
        FFTPlanUniformPool(             const FFTPlanUniformPool& other ) =   delete;
        FFTPlanUniformPool& operator=(  const FFTPlanUniformPool& other ) =   delete;
        FFTPlanUniformPool(             FFTPlanUniformPool&& other      ) =   default;
        FFTPlanUniformPool& operator=(  FFTPlanUniformPool&& other      ) =   default;

        /*======================================================================
        Function to fetch a plan
            > searches for a free-plan
            > if found, locks the plan
                > return the handle to the plan
        ----------------------------------------------------------------------*/ 
        AccessPairs fetch_plan() {
            const int num_rounds = 12;
            for (int round = 0; round < num_rounds; ++round) {
                for (int i = 0; i < mutexes.size(); ++i) {

                    std::unique_lock<std::mutex> curr_lock(
                        mutexes[i], 
                        std::try_to_lock
                    );
                    if (curr_lock.owns_lock())
                        return AccessPairs(plans[i], std::move(curr_lock));

                }
            }
            throw std::runtime_error(
                "FILE: FFTPlanPoolClass.hpp | CLASS: FFTPlanUniformPool | FUNCTION: fetch_plan() | "
                "Report: No plans available despite num_rounds rounds of searching");
        }
    };
}
