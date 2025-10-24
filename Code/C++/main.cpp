// =============================================================================
#include "./include/before.hpp"
// =============================================================================
int main(){

	// Setup
	svr::Timer          timer(          "main");
    svr::ThreadPool     thread_pool(    3);
    
    // setting up FFT/IFFT plan pools
    auto    num_plans                   {32};
    auto    nfft                        {16384};
    
    // pools for real-convolution
    svr::FFTPlanUniformPoolHandle<  double, 
                                    std::complex<double>  > fft_pool_handle(num_plans,   nfft);
    svr::IFFTPlanUniformPoolHandle< std::complex<double>, 
                                    double                > ifft_pool_handle(num_plans,   nfft);
    
                                    // pools for complex-convolution
    svr::FFTPlanUniformPoolHandle<  std::complex<double>, 
                                    std::complex<double>  >  fph_match_filter(num_plans,  128);
    svr::IFFTPlanUniformPoolHandle< std::complex<double>, 
                                    std::complex<double>  >  ifph_match_filter(num_plans, 128);
    
    // logging
    spdlog::info("Finished Setting up FFT-Plans");

    
    
    // Building Sea-Floor
    auto    seafloor     {ScattererClass<double>()};
    thread_pool.push_back([&]{fSeaFloorSetup(std::ref(seafloor));});

    // Building ULAs
    ULAClass<   double, 
                double, 
                std::complex<double>,
                std::complex<double>>   ula_fls, 
                                        ula_portside, 
                                        ula_starboard;
    thread_pool.push_back([&]{
        fULASetup<  double, 
                    double, 
                    std::complex<double>,
                    std::complex<double>>(
            std::ref(ula_fls),
            std::ref(ula_portside),
            std::ref(ula_starboard)
        );
    });

    // Building transmiter
    TransmitterClass<double> transmitter_fls, 
                             transmitter_portside, 
                             transmitter_starboard; 
    thread_pool.push_back([&]{
        fTransmitterSetup<double>(
            std::ref(transmitter_fls),
            std::ref(transmitter_portside),
            std::ref(transmitter_starboard)
        );
    });
    
    // converging threads
    thread_pool.converge();

    // Logging
    spdlog::info("Finished ULA Setup");
    spdlog::info("Finished Transmitter Setup");

    // Building AUV
    AUVClass<       double, 
                    double, 
                    std::complex<double>,
                    std::complex<double>    >    auv;
    fAUVSetup(auv);

    // attaching components to the AUV
    auv.ULA_fls                 =   std::move(      ula_fls);
    auv.ULA_portside            =   std::move(      ula_portside);
    auv.ULA_starboard           =   std::move(      ula_starboard);
    auv.transmitter_fls         =   std::move(      transmitter_fls);
    auv.transmitter_portside    =   std::move(      transmitter_portside);
    auv.transmitter_starboard   =   std::move(      transmitter_starboard);
    spdlog::info("Finished AUV Setup");

    // precomputing the data-structures required for processing
    auv.init(thread_pool, fph_match_filter, ifph_match_filter);
    spdlog::info("Finished ULA Initialization");

    // starting simulation
    auto    num_stop_hops   {10};
    const   auto&   seafloor_const  {seafloor};
    for(auto    i = 0; i < num_stop_hops; ++i){

        // starting timer
        svr::Timer timer_stophop_i   {"stop-hop, i = "+std::to_string(i)};

        // signal simulation
        auv.simulate_signal(seafloor_const, 
                            thread_pool,
                            fft_pool_handle,
                            ifft_pool_handle);

        // imaging
        auv.image(  thread_pool,
                    fph_match_filter,
                    ifph_match_filter);

        // moving to next hop-position 
        auv.step(0.5);
    }
   
    // =====================================================
	return 0;
    // =====================================================
}