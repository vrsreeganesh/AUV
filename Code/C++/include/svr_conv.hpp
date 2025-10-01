#pragma once
namespace   svr {
    /*==============================================================================
    1D convolution of two vectors
    > implemented through fft
    ------------------------------------------------------------------------------*/ 
    template    <typename T1, typename T2>
    auto    conv1D(const    std::vector<T1>&     input_vector_A,
                   const    std::vector<T2>&     input_vector_B)
    {
        // resulting type
        using   T3  = decltype(std::declval<T1>()   *   std::declval<T2>());

        // creating canvas
        auto    canvas_length       {input_vector_A.size() + input_vector_B.size() - 1};

        // calculating fft of two arrays
        auto    fft_A       {svr::fft(input_vector_A,    canvas_length)};
        auto    fft_B       {svr::fft(input_vector_B,    canvas_length)};

        // element-wise multiplying the two matrices
        auto    fft_AB      {fft_A  *   fft_B};

        // finding inverse FFT
        auto    convolved_result    {ifft(fft_AB)};

        // returning
        return std::move(convolved_result);
    }
    
}
