namespace   svr {
    /*==========================================================================
    --------------------------------------------------------------------------*/ 
    // template <typename T>
    // void reset(std::vector<T>& input_vector) {
    //     std::vector<T>().swap(input_vector);
    // }
    /*==========================================================================
    Variadic version of resetting
    --------------------------------------------------------------------------*/ 
    template <typename T, typename... Rest>
    void reset(std::vector<T>& first_vector, Rest&... rest_vectors) {
        // Reset the first vector
        std::vector<T>().swap(first_vector);
        
        // Recursively reset the remaining vectors
        if constexpr (sizeof...(rest_vectors) > 0) {
            reset(rest_vectors...);
        }
    }
}
