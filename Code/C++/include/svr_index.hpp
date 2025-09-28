namespace   svr {
    /*==========================================================================
    y = index(vector, mask)
    --------------------------------------------------------------------------*/
    template    <typename T1,
                 typename T2,
                 typename = std::enable_if_t<std::is_arithmetic_v<T1>                   || 
                                             std::is_same_v<T1, std::complex<float> >   ||
                                             std::is_same_v<T1, std::complex<double> >
                                            >
                 >
    auto    index(const     std::vector<T1>&        input_vector,
                  const     std::vector<T2>&        indices_to_sample)
    {
        // creating canvas
        auto    canvas      {std::vector<T1>(indices_to_sample.size(), 0)};

        // copying the associated values
        for(int i = 0; i < indices_to_sample.size(); ++i){
            auto    source_index    {indices_to_sample[i]};
            if(source_index < input_vector.size()){
                canvas[i] = input_vector[source_index];
            }
            else
                cout << "svr::index | source_index !< input_vector.size()\n";
        }

        // returning
        return std::move(canvas);
    }
    // /*==========================================================================
    // y = index(matrix, mask)
    // --------------------------------------------------------------------------*/
    // template <typename T1, typename T2>
    // auto index(const    std::vector<std::vector<T1>>&   input_matrix,
    //            const    std::vector<bool>&              boolean_mask)
    // {

    // }
}