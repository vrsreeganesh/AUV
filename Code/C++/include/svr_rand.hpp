#pragma once
/*==============================================================================
------------------------------------------------------------------------------*/ 
template <typename T>
auto rand(const     T   min, 
          const     T   max) {
    static std::random_device rd;   // Seed
    static std::mt19937 gen(rd());  // Mersenne Twister generator
    std::uniform_real_distribution<> dist(min, max);
    return dist(gen);
}
/*==============================================================================
------------------------------------------------------------------------------*/ 
template <typename T>
auto rand(const     T               min, 
          const     T               max, 
          const     std::size_t     numelements) 
{
    static std::random_device rd;   // Seed
    static std::mt19937 gen(rd());  // Mersenne Twister generator
    std::uniform_real_distribution<> dist(min, max);

    // building the fianloutput
    vector<T> finaloutput(numelements);
    for(int i = 0; i<finaloutput.size(); ++i)   {finaloutput[i] = static_cast<T>(dist(gen));}

    return finaloutput;
}
/*==============================================================================
------------------------------------------------------------------------------*/ 
template <typename T>
auto rand(const T                   argmin, 
          const T                   argmax, 
          const std::vector<int>    dimensions)
{

    // throwing an error if dimension is greater than two 
    if (dimensions.size() > 2)  {std::cerr << "dimensions are too high\n";}

    // creating random engine
    static std::random_device rd;   // Seed
    static std::mt19937 gen(rd());  // Mersenne Twister generator
    std::uniform_real_distribution<> dist(argmin, argmax);

    // building the finaloutput
    vector<vector<T>> finaloutput;
    for(int i = 0; i<dimensions[0]; ++i){
        vector<T> temp; 
        for(int j = 0; j<dimensions[1]; ++j)    {temp.push_back(dist(gen));}
        // cout << format("\t\t temp = {}\n", temp);

        finaloutput.push_back(temp);
    }
    
    // returning the finaloutput
    return finaloutput;

}
/*==============================================================================
------------------------------------------------------------------------------*/ 
auto rand(const std::vector<int>     dimensions)
{
    using ReturnType = double;

    // throwing an error if dimension is greater than two 
    if (dimensions.size() > 2)  {std::cerr << "dimensions are too high\n";}

    // creating random engine
    static std::random_device rd;   // Seed
    static std::mt19937 gen(rd());  // Mersenne Twister generator
    std::uniform_real_distribution<> dist(0.00, 1.00);

    // building the finaloutput
    vector<vector<ReturnType>> finaloutput;
    for(int i = 0; i<dimensions[0]; ++i){
        vector<ReturnType> temp; 
        for(int j = 0; j<dimensions[1]; ++j)    {temp.push_back(dist(gen));}
        finaloutput.push_back(std::move(temp));
    }
    
    // returning the finaloutput
    return std::move(finaloutput);

}
/*==============================================================================
------------------------------------------------------------------------------*/ 
template <typename T>
auto rand_complex_double(const T                    argmin, 
                         const T                    argmax, 
                         const std::vector<int>&    dimensions)
{

    // throwing an error if dimension is greater than two 
    if (dimensions.size() > 2)  {std::cerr << "dimensions are too high\n";}

    // creating random engine
    static std::random_device rd;   // Seed
    static std::mt19937 gen(rd());  // Mersenne Twister generator
    std::uniform_real_distribution<> dist(argmin, argmax);

    // building the finaloutput
    vector<vector<complex<double>>> finaloutput;
    for(int i = 0; i<dimensions[0]; ++i){
        vector<complex<double>> temp; 
        for(int j = 0; j<dimensions[1]; ++j)    {temp.push_back(static_cast<double>(dist(gen)));}
        finaloutput.push_back(std::move(temp));
    }

    // returning the finaloutput
    return finaloutput;
}