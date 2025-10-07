namespace   svr     {
    template    <typename   sourceType,
                 typename   destinationType,
                 typename = std::enable_if_t<std::is_same_v<sourceType,         double> &&
                                             std::is_same_v<destinationType,    std::complex<double>>
                                            >
                >
    class   FFTPlanUniformPool
    {
        public:

            // Members
            struct  AccessPairs
            {
                // members
                svr::FFTPlanClass   <sourceType, destinationType>   plan;
                std::mutex                                          mutex;

                // special members 
                AccessPairs()                                           =   default;
                AccessPairs(const   std::size_t    nfft):   plan(nfft)  {}
                AccessPairs(const   AccessPairs&    other)              =   default;
                AccessPairs&    operator=(const AccessPairs&    other)  =   default;
                AccessPairs(AccessPairs&& other)                        =   default;
                AccessPairs&    operator=(AccessPairs&& other)          =   default;
            };
            std::vector<    std::unique_ptr<AccessPairs>   >   engines;
        
            
            // Special Member Function
            FFTPlanUniformPool()                                                =   default;
            FFTPlanUniformPool(const    std::size_t     num_plans,
                               const    std::size_t     nfft)
            {
                engines.reserve(num_plans);
                for(auto i = 0; i<num_plans; ++i)
                    engines.emplace_back(std::make_unique<AccessPairs>(nfft));
            }
            FFTPlanUniformPool(const    FFTPlanUniformPool& other)              =   delete;
            FFTPlanUniformPool& operator=(const FFTPlanUniformPool& other)      =   delete;
            FFTPlanUniformPool(FFTPlanUniformPool&&     other)                  =   default;
            FFTPlanUniformPool& operator=(FFTPlanUniformPool&&  other)          =   default;

            
            // Members
            AccessPairs&    fetch_plan()
            {
                auto num_rounds     {12};
                for(auto i = 0; i < num_rounds; ++i){
                    for(auto&   engine_ptr: engines){
                        if(engine_ptr->mutex.try_lock()){
                            engine_ptr->mutex.unlock();
                            return *engine_ptr;
                        }
                    }
                }
                throw std::runtime_error("FILE: FFTPlanPoolClass.hpp | CLASS: FFTPlanUniformPool | FUNCTION: fetch_plan() | Report: No plans available despite num-rounds rounds of searching");
            }
    };
}
