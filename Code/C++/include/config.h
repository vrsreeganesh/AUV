// hash defines
#ifndef PRINTSPACE
    #define PRINTSPACE      std::cout<<"\n\n\n";
#endif
#ifndef PRINTSMALLLINE
    #define PRINTSMALLLINE  std::cout<<"------------------------------------------------------------------------------------"<<std::endl;
#endif
#ifndef PRINTDOTS
    #define PRINTDOTS       std::cout<<"...................................................................................."<<std::endl;
#endif
#ifndef PRINTLINE
    #define PRINTLINE       std::cout<<"===================================================================================="<<std::endl;
#endif
#ifndef PI
    #define PI              3.14159265
#endif

// debugging hashdefine
#ifndef DEBUGMODE
    #define DEBUGMODE       false
#endif

// deciding to save tensors or not
#ifndef SAVETENSORS
    #define SAVETENSORS       true
    // #define SAVETENSORS       false
#endif

// choose device here
#ifndef DEVICE
    #define DEVICE          torch::kCPU
    // #define DEVICE          torch::kMPS
    // #define DEVICE          torch::kCUDA
#endif

// Enable Imaging
#define IMAGING_TOGGLE                  true

// data-type related
#ifndef DATATYPE
    #define DATATYPE torch::kHalf
    // #define DATATYPE torch::kFloat
#endif