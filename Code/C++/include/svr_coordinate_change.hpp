namespace svr {
    // =========================================================================
    template <typename T>
    auto    cart2sph(const     std::vector<T>  cartesian_vector){

        // splatting the point onto xy-plane
        auto    xysplat     {cartesian_vector};
        xysplat[2]      =   0;

        // finding splat lengths
        auto    xysplat_lengths     {norm(xysplat)};

        // finding azimuthal and elevation angles
        auto    azimuthal_angles    {svr::atan2(xysplat[1], xysplat[0])     *   180.00/std::numbers::pi};
        auto    elevation_angles    {svr::atan2(cartesian_vector[2], xysplat_lengths)   *   180.00/std::numbers::pi};
        auto    rho_values          {norm(cartesian_vector)};

        // creating tensor to send back
        auto    spherical_vector    {std::vector<T>{azimuthal_angles,
                                                    elevation_angles,
                                                    rho_values}};

        // moving it back
        return std::move(spherical_vector);

    }
    // =========================================================================
    template <typename T>
    auto    sph2cart(const  std::vector<T>  spherical_vector){

        // creating cartesian vector
        auto    cartesian_vector    {std::vector<T>(spherical_vector.size(), 0)};

        // populating
        cartesian_vector[0]     =   spherical_vector[2] * \
                                    cos(spherical_vector[1] * std::numbers::pi  / 180.00) * \
                                    cos(spherical_vector[0] * std::numbers::pi  / 180.00);
        cartesian_vector[1]     =   spherical_vector[2] *   \
                                    cos(spherical_vector[1] * std::numbers::pi  / 180.00) * \
                                    sin(spherical_vector[0] * std::numbers::pi  / 180.00);
        cartesian_vector[2]     =   spherical_vector[2] *   \
                                    sin(spherical_vector[1] * std::numbers::pi  / 180.00);

        // returning
        return std::move(cartesian_vector);
    }
}
