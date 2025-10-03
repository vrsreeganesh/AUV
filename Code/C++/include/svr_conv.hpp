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

    template <>
    auto    conv1D(const    std::vector<double>&     input_vector_A,
                   const    std::vector<double>&     input_vector_B)
    {
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

    /*==============================================================================
    1D convolution of two vectors
    > implemented through fft
    ------------------------------------------------------------------------------*/ 
    template    <typename T1, typename T2>
    auto    conv1D_fftw(const    std::vector<T1>&     input_vector_A,
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
        auto    convolved_result    {svr::ifft(fft_AB, fft_AB.size())};

        // returning
        return std::move(convolved_result);
    }

    /*==========================================================================
    Long-signal Conv1D
    improvements:
        > make an inplace version of this
    --------------------------------------------------------------------------*/
    template    <std::size_t    L,  typename   T>
    auto    conv1D_long(const   std::vector<T>&     input_vector_A,
                        const   std::vector<T>&     input_vector_B)
    {
        // fetching dimensions
        const   auto    maxlength       {std::max(input_vector_A.size(),
                                                  input_vector_B.size())};
        const   auto    filter_size     {std::min(input_vector_A.size(),
                                                  input_vector_B.size())};
        const   auto    block_size      {L  +   filter_size -   1};
        const   auto    num_blocks      {2 + static_cast<std::size_t>(
            (maxlength - block_size)/L
        )};

        // obtaining references
        const   auto&   large_vector    {input_vector_A.size() >= input_vector_B.size() ? \
                                         input_vector_A         : input_vector_B};
        const   auto&   small_vector    {input_vector_A.size() <  input_vector_B.size() ? \
                                         input_vector_A         :  input_vector_B};

        // setup
        auto    starting_index          {static_cast<std::size_t>(0)};
        auto    ending_index            {static_cast<std::size_t>(0)};
        auto    length_left_to_fill     {ending_index - starting_index};
        auto    canvas                  {std::vector<double>(block_size, 0)};
        auto    finaloutput             {std::vector<double>(maxlength, 0)};
        auto    block_conv_output_size  {L + 2 * filter_size -2};
        auto    block_conv_output       {std::vector<double>(block_conv_output_size, 0)};

        // block-wise processing
        for(auto    bid = 0; bid < num_blocks; ++bid)
        {
            // estimating indices
            starting_index          =   L*bid;
            ending_index            =   std::min(starting_index + block_size - 1,maxlength - 1);
            length_left_to_fill     =   ending_index - starting_index;

            // copying to the common-block
            std::copy(large_vector.begin() + starting_index,    
                      large_vector.begin() + ending_index + 1,
                      canvas.begin());

            // performing convolution
            block_conv_output   =   svr::conv1D_fftw(canvas, 
                                                     small_vector);

            // discarding edges and writing values
            std::copy(block_conv_output.begin() + filter_size-2, 
                      block_conv_output.begin() + filter_size-2 + std::min(static_cast<int>(L-1), static_cast<int>(length_left_to_fill)) + 1,
                      finaloutput.begin()+starting_index);   
        }

        // returning
        return std::move(finaloutput);

    }

    // /*==============================================================================
    // 1D convolution of two vectors
    // > implemented through fft
    // ------------------------------------------------------------------------------*/ 
    // template    <typename T1, typename T2>
    // auto    conv1D(const    std::vector<T1>&     input_vector_A,
    //                const    std::vector<T2>&     input_vector_B)
    // {
    //     // resulting type
    //     using   T3  = decltype(std::declval<T1>()   *   std::declval<T2>());

    //     // creating canvas
    //     auto    canvas_length       {input_vector_A.size() + input_vector_B.size() - 1};

    //     // calculating fft of two arrays
    //     auto    fft_A       {svr::fft(input_vector_A,    canvas_length)};
    //     auto    fft_B       {svr::fft(input_vector_B,    canvas_length)};

    //     // element-wise multiplying the two matrices
    //     auto    fft_AB      {fft_A  *   fft_B};

    //     // finding inverse FFT
    //     auto    convolved_result    {ifft(fft_AB)};

    //     // returning
    //     return std::move(convolved_result);
    // }



    /*==========================================================================
    Short-conv1D
    --------------------------------------------------------------------------*/ 
    // template    <std::size_t    shortsize, 
    //              typename       T1,
    //              typename       T2>
    template    <typename       T1,
                 typename       T2>
    auto    conv1D_short(const    std::vector<T1>&   input_vector_A,
                         const    std::vector<T2>&   input_vector_B)
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
        // return std::move(convolved_result);
        return convolved_result;

    }


    /*==========================================================================
    1D Convolution of a matrix and a vector
    --------------------------------------------------------------------------*/
    template    <typename T>
    auto    conv1D(const    std::vector<std::vector<T>>&    input_matrix,
                   const    std::vector<T>&                 input_vector,
                   const    std::size_t&                    dim)
    {
        // getting dimensions
        const   auto&   num_rows_matrix         {input_matrix.size()};
        const   auto&   num_cols_matrix         {input_matrix[0].size()};
        const   auto&   num_elements_vector     {input_vector.size()};

        // creating canvas
        auto    canvas      {std::vector<std::vector<T>>()};

        // creating output based on dim
        if (dim == 1)
        {
            // performing convolutions row by row
            for(auto    row = 0; row < num_rows_matrix; ++row)
            {
                cout << format("\t\t row = {}/{}\n", row, num_rows_matrix);
                auto bruh   {conv1D(input_matrix[row], input_vector)};
                auto bruh_real  {svr::real(std::move(bruh))};

                canvas.push_back(
                        svr::real(
                            std::move(bruh_real)
                        )
                );
            }
        }
        else{
            std::cerr << "svr_conv.hpp | conv1D | yet to be implemented \n";
        }

        // returning
        return std::move(canvas);

    }

    /*==========================================================================
    1D Convolution of a matrix and a vector (in-place)
    --------------------------------------------------------------------------*/
    
}
