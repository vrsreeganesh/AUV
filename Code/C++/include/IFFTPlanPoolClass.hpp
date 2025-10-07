namespace   svr     {
    template    <typename   sourceType,
                 typename   destinationType,
                 typename = std::enable_if_t<std::is_same_v<sourceType,         double> &&
                                             std::is_same_v<destinationType,    std::complex<double>>
                                            >
                >
    class   IFFTPlanUniformPool
    {
        public:

            // Members
            struct  AccessPairs
            {
                // members
                svr::IFFTPlanClass   <sourceType, destinationType>   plan;
                std::mutex                                          mutex;

                // special members 
                AccessPairs()                                           =   default;
                AccessPairs(const   std::size_t    nfft):
                    plan(nfft)  {}
                AccessPairs(const   AccessPairs&    other)              =   default;
                AccessPairs&    operator=(const AccessPairs&    other)  =   default;
                AccessPairs(AccessPairs&& other)                        =   default;
                AccessPairs&    operator=(AccessPairs&& other)          =   default;
            };
            std::vector<    std::unique_ptr<AccessPairs>   >   engines;
        
            // Special Member Function
            IFFTPlanUniformPool()                                                =   default;
            IFFTPlanUniformPool(const    std::size_t     num_plans,
                                const    std::size_t     nfft)
            {
                engines.reserve(num_plans);
                for(auto i = 0; i<num_plans; ++i)
                    engines.emplace_back(std::make_unique<AccessPairs>(nfft));
            }
            IFFTPlanUniformPool(const    IFFTPlanUniformPool& other)              =   delete;
            IFFTPlanUniformPool& operator=(const IFFTPlanUniformPool& other)      =   delete;
            IFFTPlanUniformPool(IFFTPlanUniformPool&&     other)                  =   default;
            IFFTPlanUniformPool& operator=(IFFTPlanUniformPool&&  other)          =   default;

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
                throw std::runtime_error("FILE: IFFTPlanPoolClass.hpp | CLASS: IFFTPlanUniformPool | FUNCTION: fetch_plan() | Report: No plans available despite num-rounds rounds of searching");
                
            }
    };
}
