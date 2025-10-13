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
    /*==========================================================================
    Long-signal Conv1D with FFT Plan
    improvements:
        > make an inplace version of this
    --------------------------------------------------------------------------*/
    template    <
        typename    T,
        std::enable_if_t<
            std::is_floating_point_v<T>
        >
    >
    auto    conv1D_long_prototype(
        const   std::vector<T>&                     input_vector_A,
        const   std::vector<T>&                     input_vector_B,
        svr::FFTPlanClass<T, std::complex<T>>&      fft_plan,
        svr::IFFTPlanClass<std::complex<T>, T>&     ifft_plan
    )
    {
        // Error checks
        if (fft_plan.nfft_  != ifft_plan.nfft_)
            throw std::runtime_error("fft_plan.nfft_  != ifft_plan.nfft_");

        // fetching references to large-signal and small-signal
        const   auto&   large_signal_original    {
            input_vector_A.size() >= input_vector_B.size()      ?
            input_vector_A          :   input_vector_B
        };
        const   auto&   small_signal    {
            input_vector_A.size()   <   input_vector_B.size()   ?
            input_vector_A          :   input_vector_B
        };

        // copying 
        auto    large_signal    {std::vector<double>(
            input_vector_A.size() + input_vector_B.size() -1
        )};
        std::copy(large_signal_original.begin(), 
                  large_signal_original.end(),
                  large_signal.begin());;

        // calculating parameters
        const   auto    signal_size             {large_signal_original.size()};
        const   auto    filter_size             {small_signal.size()};
        const   auto    input_signal_block_size {fft_plan.nfft_ + 1 - filter_size};
        if (input_signal_block_size <= 0)
            throw std::runtime_error("input_signal_block_size <= 0 ");
        const   auto    block_output_length     {fft_plan.nfft_};
        const   auto    num_blocks              {static_cast<int>(
            1 + std::ceil((signal_size + filter_size - 2)/input_signal_block_size)
        )
        };
        const   auto    final_output_size       {signal_size + filter_size - 1};
        const   auto    useful_sample_length          {block_output_length - (filter_size -1) - (filter_size -1)};

        // parameters for re-use
        auto    start_index             {static_cast<int>(0)};
        auto    end_index               {static_cast<int>(0)};
        auto    output_start_index      {static_cast<int>(0)};

        // calculating fft(filter)
        auto    filter_zero_padded      {std::vector<double>(block_output_length, 0.0)};
        std::copy(small_signal.begin(), small_signal.end(), filter_zero_padded.begin());
        auto    filter_FFT              {fft_plan.fft(filter_zero_padded)};

        // allocating space for storing input-blocks
        auto    signal_block_zero_padded    {std::vector<double>(block_output_length,   0.0)};
        auto    fftw_output                 {std::vector<double>()};
        auto    conv_output                 {std::vector<double>()};
        auto    finaloutput                 {std::vector<double>(final_output_size,     0.0)};

        // going through the values
        svr::Timer timer("fft-loop");
        for(auto i = 0; i<num_blocks; ++i){
            
            // calculating bounds
            auto    analytical_start    {
                (i*static_cast<int>(input_signal_block_size)) - (static_cast<int>(filter_size) - 1)
            };
            auto    analytical_end      {(i+1)*input_signal_block_size -1};
            start_index     =   std::max(
                static_cast<int>(0),    static_cast<int>(analytical_start)
            );
            end_index       =   std::min(
                static_cast<int>(signal_size-1),static_cast<int>(analytical_end)
            ); // [start-index, end-index)

            // copying values
            signal_block_zero_padded = std::move(std::vector<double>(block_output_length, 0.0));
            std::copy(large_signal.begin()  +   start_index,
                      large_signal.begin()  +   end_index + 1,
                      signal_block_zero_padded.begin() + start_index - analytical_start);
            
            // performing ifft(fft(x) * fft(y))
            fftw_output     =   ifft_plan.ifft(
                fft_plan.fft(signal_block_zero_padded) * filter_FFT
            );

            // trimming away the first parts (since partial)
            conv_output = std::vector<double>(fftw_output.begin() + filter_size - 1,fftw_output.end());

            // writing to final-output 
            std::copy(conv_output.begin(), conv_output.end(), finaloutput.begin() + output_start_index);
            output_start_index      += conv_output.size();
        }

    }

    /*==========================================================================
    Long-signal Conv1D with FFT-Plan-Pool
    --------------------------------------------------------------------------*/
    template    <
        typename    T,
        typename    =   std::enable_if_t<
            std::is_same_v<T, double>    ||
            std::is_same_v<T, float>
        >
    >
    auto    conv_per_plan(
        const   int                                             i,
        const   int&                                            input_signal_block_size,
        const   int&                                            filter_size,
        const   int&                                            block_output_length,
        const   std::vector<T>&                                 large_signal,
        std::vector<T>                                          signal_block_zero_padded,
        svr::FFTPlanUniformPoolHandle<T, std::complex<T>>&      fft_pool_handle,
        svr::IFFTPlanUniformPoolHandle<std::complex<T>, T>&     ifft_pool_handle,
        const   std::vector<std::complex<T>>&                   filter_FFT,
        std::vector<T>                                          fftw_output,
        std::vector<T>                                          conv_output,
        std::vector<T>&                                         output_vector,
        std::mutex&                                             output_vector_mutex,
        const   auto&                                           signal_size
    )
    {

        // calculating bounds
        auto    analytical_start    {
            (i*static_cast<int>(input_signal_block_size)) - (static_cast<int>(filter_size) - 1)
        };
        auto    analytical_end      {(i+1)*input_signal_block_size -1};
        auto start_index     =   std::max(
            static_cast<int>(0),    static_cast<int>(analytical_start)
        );
        auto end_index       =   std::min(
            static_cast<int>(signal_size-1),static_cast<int>(analytical_end)
        ); // [start-index, end-index)

        // copying values
        signal_block_zero_padded = std::move(std::vector<double>(block_output_length, 0.0));
        std::copy(
            large_signal.begin()                +   start_index,
            large_signal.begin()                +   end_index       + 1,
            signal_block_zero_padded.begin()    +   start_index     - analytical_start
        );

        // fetching an fft and IFFT plan
        auto    fph_lock        {fft_pool_handle.lock()};
        auto    ifph_lock       {ifft_pool_handle.lock()};
        auto    fft_pair        {fft_pool_handle.uniform_pool.fetch_plan()};
        auto    ifft_pair       {ifft_pool_handle.uniform_pool.fetch_plan()};

        // performing ifft(fft(x) * filter-FFT)
        fftw_output     =   ifft_pair.plan.ifft_l2_conserved(
            fft_pair.plan.fft_l2_conserved(signal_block_zero_padded)   *   filter_FFT
        );

        // trimming away the first parts (since partial)
        conv_output =   std::vector<T>(
            fftw_output.begin() + filter_size - 1,
            fftw_output.end()
        );

        // writing to final-output
        auto    output_start_index = i * (block_output_length - (filter_size - 1));
        std::lock_guard<std::mutex>     output_lock(output_vector_mutex);
        std::copy(
            conv_output.begin(), conv_output.end(),
            output_vector.begin() + output_start_index
        );
    }
    

    template    <
        typename T,
        typename    =   std::enable_if_t<
            std::is_same_v<T, double>    ||
            std::is_same_v<T, float>
        >
    >
    auto    conv1D_long_FFTPlanPool(
        const   std::vector<T>&                                 input_vector_A,
        const   std::vector<T>&                                 input_vector_B,
        svr::FFTPlanUniformPoolHandle<T, std::complex<T>>&      fft_pool_handle,
        svr::IFFTPlanUniformPoolHandle<std::complex<T>, T>&     ifft_pool_handle
    )
    {
        // Error checks
        if  (fft_pool_handle.nfft!=ifft_pool_handle.nfft)
            throw std::runtime_error("FILE: svr_conv.hpp | FUNCTION: conv1D_long_FFTPlanPool | Report: the pool-handles are for different nffts");

        // fetching references to the large signal and small signal
        const   auto&   large_signal_original   {
            input_vector_A.size()   >=  input_vector_B.size()   ?
            input_vector_A          :   input_vector_B
        };
        const   auto&   small_signal    {
            input_vector_A.size()   <   input_vector_B.size()   ?
            input_vector_A          :   input_vector_B
        };

        // copying 
        auto    large_signal    {std::vector<double>(
            input_vector_A.size() + input_vector_B.size() - 1
        )};
        std::copy(large_signal_original.begin(),
                  large_signal_original.end(),
                  large_signal.begin());

        // calculating some parameters
        const   auto    signal_size     {large_signal_original.size()};
        const   auto    filter_size     {small_signal.size()};
        const   auto    input_signal_block_size {
            fft_pool_handle.nfft + 1 - filter_size
        };

        // throwing an error if nfft < filter-size
        if (fft_pool_handle.nfft < filter_size)
            throw std::runtime_error("FILE: svr_conv.hpp | FUNCTION: conv1D_long_FFTPlanPool | REPORT: filter is bigger than nfft");

        // throwing an error if number of useful samples is less than zero
        if (input_signal_block_size <= 0)
            throw std::runtime_error("FILE: svr_conv.hpp | FUNCTION: conv1D_long_FFTPlanPool | REPORT: input_signal_block_size = 0");

        
        const   auto    block_output_length     {fft_pool_handle.nfft};
        const   auto    num_blocks              {static_cast<int>(
            1 + std::ceil((signal_size + filter_size - 2) / input_signal_block_size)
        )};
        const   auto    final_output_size       {signal_size + filter_size - 1};
        const   auto    useful_sample_length    {
            block_output_length - (filter_size - 1) - (filter_size -1)
        };

        // parameters for re-use
        auto    start_index             {static_cast<int>(0)};
        auto    end_index               {static_cast<int>(0)};
        auto    output_start_index      {static_cast<int>(0)};

        // calculating fft(filter)
        auto    filter_zero_padded      {std::vector<double>(block_output_length, 0.0)};
        std::copy(small_signal.begin(), small_signal.end(), filter_zero_padded.begin());
        auto    fph_lock0               {fft_pool_handle.lock()};
        auto    curr_plan_pair          {fft_pool_handle.uniform_pool.fetch_plan()};
        auto    pool_num_plans          {fft_pool_handle.num_plans};
        fph_lock0.unlock();
        auto    filter_FFT              {
            curr_plan_pair.plan.fft(
                filter_zero_padded
            )
        };
        curr_plan_pair.lock.unlock();

        // allocating space for storing input-blocks
        auto    signal_block_zero_padded    {std::vector<T>(block_output_length,    0.0)};
        auto    fftw_output                 {std::vector<T>()};
        auto    conv_output                 {std::vector<T>()};
        auto    output_vector               {std::vector<T>(final_output_size,      0.0)};
        auto    output_vector_mutex         {std::mutex()};

        // creating boost
        svr::ThreadPool local_pool(pool_num_plans);

        // going through the values 
        for(auto i = 0; i < num_blocks; ++i)
        {
            local_pool.push_back(
                [
                    i,
                    &input_signal_block_size,
                    &filter_size,
                    &block_output_length,
                    &large_signal,
                    signal_block_zero_padded,
                    &fft_pool_handle,
                    &ifft_pool_handle,
                    &filter_FFT,
                    fftw_output,
                    conv_output,
                    &output_vector,
                    &output_vector_mutex,
                    &signal_size
                ]{
                    conv_per_plan<T>(
                        i,
                        std::ref(input_signal_block_size),
                        std::ref(filter_size),
                        std::ref(block_output_length),
                        std::ref(large_signal),
                        signal_block_zero_padded,
                        fft_pool_handle,
                        ifft_pool_handle,
                        filter_FFT,
                        fftw_output,
                        conv_output,
                        std::ref(output_vector),
                        output_vector_mutex,
                        signal_size
                    );
                }
            );
        }
        local_pool.converge();

        // returning final output
        return std::move(output_vector);
        // return output_vector;
    }

    /*==========================================================================
    Short-conv1D
    --------------------------------------------------------------------------*/ 
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
    
}