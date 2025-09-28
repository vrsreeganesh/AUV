// =============================================================================
#include "./include/before.hpp"
// =============================================================================
int main(){

	// Configuration
	svr::Timer timer("main");

    // Building Sea-Floor
    auto    seafloor     {ScattererClass<double>()};
    std::thread scatterThread_t(fSeaFloorSetup, 
                                std::ref(seafloor));
    
    // Building ULA
    ULAClass<double>    ula_fls, ula_portside, ula_starboard;
    std::thread ulaThread_t(fULASetup<double>,
                            std::ref(ula_fls),
                            std::ref(ula_portside),
                            std::ref(ula_starboard));

    // Building Transmitter
    TransmitterClass<double> transmitter_fls, 
                             transmitter_portside, 
                             transmitter_starboard; 
    std::thread transmitterThread_t(fTransmitterSetup<double>,
                                    std::ref(transmitter_fls),
                                    std::ref(transmitter_portside),
                                    std::ref(transmitter_starboard));

    // Recombining threads
    scatterThread_t.join();
    ulaThread_t.join();
    transmitterThread_t.join();

    // Building AUV
    AUVClass<double>    auv;
    fAUVSetup(auv);

    // attaching components to the AUV
    auv.ULA_fls                 =   std::move(ula_fls);
    auv.ULA_portside            =   std::move(ula_portside);
    auv.ULA_starboard           =   std::move(ula_starboard);
    auv.transmitter_fls         =   std::move(transmitter_fls);
    auv.transmitter_portside    =   std::move(transmitter_portside);
    auv.transmitter_starboard   =   std::move(transmitter_starboard);

    // precomputing the data-structures required for processing
    auv.init();

    // starting simulation
    auto    num_stop_hops   {4};
    for(auto    i = 0; i < num_stop_hops; ++i){

        // starting timer
        svr::Timer timer_stophop_i   {"stop-hop, i = "+std::to_string(i)};

        // signal simulation
        auv.simulate_signal(seafloor);



    }




    // =====================================================
    
    // =====================================================
	return 0;
}