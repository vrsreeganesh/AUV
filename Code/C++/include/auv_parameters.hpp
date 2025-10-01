namespace   svr {
    class   AUVParameters
    {
    public:
        
        // special-members
        AUVParameters()     = delete;
        AUVParameters(const std::string& configuration_file);
        AUVParameters(const AUVParameters&  other)  = delete;
        AUVParameters&  operator=(const AUVParameters&  other)   = delete;
        AUVParameters(AUVParameters&& other)                = default;
        AUVParameters&  operator=(AUVParameters&& other)    = default;
    };
    /*==========================================================================
    Special-Member: Constructor
    --------------------------------------------------------------------------*/
    AUVParameters::AUVParameters(const std::string&    configuration_file)
    {
        // throwing an error
        std::cerr << "AUVParameter-class constructor not implemented \n";
    }

}