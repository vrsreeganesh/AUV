#pragma once
namespace svr {
    /*==========================================================================
    y = cart2sph(vector)
    --------------------------------------------------------------------------*/
    template <typename T>
    auto    cart2sph(const     std::vector<T>&  cartesian_vector){

        // splatting the point onto xy-plane
        auto    xysplat     {cartesian_vector};
        xysplat[2]      =   0;

        // finding splat lengths
        auto    xysplat_lengths     {norm(xysplat)};

        // finding azimuthal and elevation angles
        auto    azimuthal_angles    {svr::atan2(xysplat[1],
                                                xysplat[0]) \
                                     * 180.00/std::numbers::pi};
        auto    elevation_angles    {svr::atan2(cartesian_vector[2], 
                                                xysplat_lengths) \
                                     * 180.00/std::numbers::pi};
        auto    rho_values          {norm(cartesian_vector)};

        // creating tensor to send back
        auto    spherical_vector    {std::vector<T>{azimuthal_angles,
                                                    elevation_angles,
                                                    rho_values}};

        // moving it back
        return std::move(spherical_vector);
    }
    /*==========================================================================
    y = cart2sph(vector)
    --------------------------------------------------------------------------*/
    template <typename T>
    auto    cart2sph_inplace(std::vector<T>&  cartesian_vector){

        // splatting the point onto xy-plane
        auto    xysplat     {cartesian_vector};
        xysplat[2]      =   0;

        // finding splat lengths
        auto    xysplat_lengths     {norm(xysplat)};

        // finding azimuthal and elevation angles
        auto    azimuthal_angles    {svr::atan2(xysplat[1], xysplat[0])     *   180.00/std::numbers::pi};
        auto    elevation_angles    {svr::atan2(cartesian_vector[2], 
                                                xysplat_lengths)   *   180.00/std::numbers::pi};
        auto    rho_values          {norm(cartesian_vector)};

        // creating tesnor
        cartesian_vector[0]     =   azimuthal_angles;
        cartesian_vector[1]     =   elevation_angles;
        cartesian_vector[2]     =   rho_values;
    }
    /*==========================================================================
    y = cart2sph(input_matrix, dim)
    --------------------------------------------------------------------------*/
    template <typename T>
    auto cart2sph(const     std::vector<std::vector<T>>&    input_matrix,
                  const     std::size_t                     axis)
    {
        // fetching dimensions
        const   auto&   num_rows    {input_matrix.size()};
        const   auto&   num_cols    {input_matrix[0].size()};

        // checking the axis and dimensions
        if (axis == 0 && num_rows != 3) {std::cerr << "cart2sph: incorrect num-elements \n";}
        if (axis == 1 && num_cols != 3) {std::cerr << "cart2sph: incorrect num-elements \n";}

        // creating canvas
        auto    canvas      {std::vector<std::vector<T>>(
            num_rows,
            std::vector<T>(num_cols, 0)
        )};

        // if axis = 0, performing operation column-wise
        if(axis == 0)
        {
            for(auto col = 0; col < num_cols; ++col)
            {
                // fetching current column
                auto    curr_column    {std::vector<T>({input_matrix[0][col],
                                                       input_matrix[1][col],
                                                       input_matrix[2][col]})};
                
                // performing inplace transformation
                cart2sph_inplace(curr_column);

                // storing it back
                canvas[0][col]    =   curr_column[0];
                canvas[1][col]    =   curr_column[1];
                canvas[2][col]    =   curr_column[2];
            }
        }
        // if axis == 1, performing operations row-wise
        else if(axis == 0)
        {
            std::cerr << "cart2sph: yet to be implemented \n";
        }
        else
        {
            std::cerr << "cart2sph: yet to be implemented \n";
        }

        // returning
        return std::move(canvas);

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
