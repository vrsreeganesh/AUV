// =============================================================================
#include "./include/before.hpp"
// =============================================================================
int main(){

	// Setup
	svr::Timer          timer(          "main");
    svr::ThreadPool     thread_pool(    3);
    
    // setting up FFT/IFFT plan pools
    auto    num_plans                   {32};
    svr::FFTPlanUniformPoolHandle<double, 
                                  std::complex<double>
                                 >     fft_pool_handle(num_plans,     16384);
    svr::IFFTPlanUniformPoolHandle<std::complex<double>, 
                                   double
                                  >    ifft_pool_handle(num_plans,    16384);

    // Building Sea-Floor
    auto    seafloor     {ScattererClass<double>()};
    thread_pool.push_back([&]{fSeaFloorSetup(std::ref(seafloor));});

    // Building ULAs
    ULAClass<double>    ula_fls, 
                        ula_portside, 
                        ula_starboard;
    thread_pool.push_back([&]{
        fULASetup<double>(
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

    // Building AUV
    AUVClass<double>    auv;
    fAUVSetup(auv);

    // attaching components to the AUV
    auv.ULA_fls                 =   std::move(      ula_fls);
    auv.ULA_portside            =   std::move(      ula_portside);
    auv.ULA_starboard           =   std::move(      ula_starboard);
    auv.transmitter_fls         =   std::move(      transmitter_fls);
    auv.transmitter_portside    =   std::move(      transmitter_portside);
    auv.transmitter_starboard   =   std::move(      transmitter_starboard);

    // precomputing the data-structures required for processing
    auv.init(thread_pool);

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
                    fft_pool_handle,
                    ifft_pool_handle);

        // moving to next hop-position 
        auv.step(0.5);
    }
   
    // =====================================================
	return 0;
    // =====================================================
}