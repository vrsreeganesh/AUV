namespace   svr {
    /*==========================================================================
    --------------------------------------------------------------------------*/
    template <typename T>
    auto    zeros(const std::array<std::size_t, 2>   input_dimensions)
    {
        // create canvas
        auto    canvas      {std::vector<std::vector<T>>(
            input_dimensions[0],
            std::vector<T>(input_dimensions[1], 0)
        )};

        // returning
        return std::move(canvas);
    }
}