namespace   svr {
    template    <typename T, 
                 typename T2,
                 typename = std::enable_if_t<std::is_integral<T2>::value>
                >
    auto    slice(const     std::vector<T>&             input_vector,
                  const     T2                          start_index_arg,
                  const     T2                          end_index_arg)
    {

        svr::Timer  timer("slice");

        // setup
        std::size_t     start_index;
        std::size_t     end_index;

        // checking bounds
        start_index = start_index_arg == -1 ? 0                     : start_index_arg;
        start_index = start_index_arg == -2 ? input_vector.size()-1 : start_index;
        end_index   = end_index_arg == -1 ? 0                     : end_index_arg;
        end_index   = end_index_arg == -2 ? input_vector.size()-1 : end_index_arg;

        // checking bounds
        if (end_index < start_index)
            std::runtime_error("slice:: slice_bounds[0] < slice_bounds[1] check failed");

        // creating canvas
        const   auto    canvas_length   {end_index - start_index + 1};
        auto    canvas      {std::vector<T>(canvas_length, 0.0)};

        // copying-values
        std::copy(input_vector.begin()  +   start_index,
                  input_vector.begin()  +   end_index,
                  canvas.begin());

        // moving it back
        return std::move(canvas);

    }
}