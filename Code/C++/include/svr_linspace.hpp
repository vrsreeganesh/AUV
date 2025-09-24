// in-place
template <typename T>
auto linspace(auto&            input,
              auto             startvalue,
              auto             endvalue,
              auto             numpoints) -> void
{
    auto stepsize = static_cast<T>(endvalue - startvalue)/static_cast<T>(numpoints-1);
    for(int i = 0; i<input.size(); ++i) {input[i] = startvalue + i*stepsize;}
};
// in-place
template <typename T>
auto linspace(vector<complex<T>>&   input,
              auto                  startvalue,
              auto                  endvalue,
              auto                  numpoints) -> void
{
    auto stepsize = static_cast<T>(endvalue - startvalue)/static_cast<T>(numpoints-1);
    for(int i = 0; i<input.size(); ++i) {
        input[i] = startvalue + static_cast<T>(i)*stepsize;
    }
};

// return-type
template <typename T>
auto linspace(T             startvalue,
              T             endvalue,
              size_t        numpoints)
{
    vector<T> input(numpoints);
    auto stepsize = static_cast<T>(endvalue - startvalue)/static_cast<T>(numpoints-1);
    
    for(int i = 0; i<input.size(); ++i) {input[i] = startvalue + static_cast<T>(i)*stepsize;}
    
    return input;
};

// return-type
template <typename T, typename U>
auto linspace(T             startvalue,
              U             endvalue,
              size_t        numpoints)
{
    vector<double> input(numpoints);
    auto stepsize = static_cast<double>(endvalue - startvalue)/static_cast<double>(numpoints-1);
    
    for(int i = 0; i<input.size(); ++i) {input[i] = startvalue + i*stepsize;}
    
    return input;
};