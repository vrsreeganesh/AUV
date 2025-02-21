/* =====================================
Aim: Setup sea floor
NOAA: 50 to 100 KHz is the transmission frequency
we'll create our LFM with 50 to 70KHz
======================================*/ 

#ifndef DEVICE
    #define DEVICE          torch::kMPS
    // #define DEVICE          torch::kCPU
#endif

// =========================================================
void AUVSetup(AUVClass* auv) {

    // building properties for the auv
    torch::Tensor location           = torch::tensor({0,50,30}).reshape({3,1}).to(DATATYPE).to(DEVICE); // starting location of AUV
    torch::Tensor velocity           = torch::tensor({5,0, 0}).reshape({3,1}).to(DATATYPE).to(DEVICE);   // starting velocity of AUV
    torch::Tensor pointing_direction = torch::tensor({1,0, 0}).reshape({3,1}).to(DATATYPE).to(DEVICE);   // pointing direction of AUV

    // assigning 
    auv->location           = location;             // assigning location of auv
    auv->velocity           = velocity;             // assigning vector representing velocity
    auv->pointing_direction = pointing_direction;   // assigning pointing direction of auv   
}