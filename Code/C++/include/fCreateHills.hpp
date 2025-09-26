template <typename T>
void fCreateHills(const     std::vector<std::vector<T>>     mean_vector,
                  const     std::vector<std::vector<T>>     dimensions_vector,
                  std::vector<std::vector<T>>&              points2D)
{
    // creating hills
    auto    num_hills       {static_cast<int>(mean_vector[0].size())};

    // the heights of the point
    auto    finalHeights    {std::vector<T>(points2D[0].size(), 0)};

    // setup
    auto    piby2           {static_cast<double>(std::numbers::pi)/2.00};

    // creating hills 
    for(int hill_index = 0; hill_index < num_hills; ++hill_index){

        // mini-setup
        const   auto    halfwidth_x             {dimensions_vector[0][hill_index]   /   2.00};
        const   auto    halfwidth_y             {dimensions_vector[1][hill_index]   /   2.00};
        const   auto&   curr_hill_height        {dimensions_vector[2][hill_index]};
        const   auto&   curr_hill_center_x      {mean_vector[0][hill_index]};
        const   auto&   curr_hill_center_y      {mean_vector[1][hill_index]};

        // changing origin 
        auto    normalizedx     {piby2 * (points2D[0] - curr_hill_center_x) / halfwidth_x};
        auto    normalizedy     {piby2 * (points2D[1] - curr_hill_center_y) / halfwidth_y};

        // building height values
        auto    heightx         {cos(normalizedx)   *   exp(abs(normalizedx)/10.00) };
        auto    heighty         {cos(normalizedy)   *   exp(abs(normalizedy)/10.00) };
        auto    heights         {curr_hill_height  * heightx   * heighty};

        // boolean masking:     indices outside hill
        auto    maskx_piby2         {normalizedx > std::numbers::pi/2.00};
        auto    maskx_minuspiby2    {normalizedx < -std::numbers::pi/2.00};
        auto    masky_piby2         {normalizedy > std::numbers::pi/2.00};
        auto    masky_minuspiby2    {normalizedy < -std::numbers::pi/2.00};

        // zeroing out 
        edit(heights,   maskx_piby2         == true,    0.00);
        edit(heights,   maskx_minuspiby2    == true,    0.00);
        edit(heights,   masky_piby2         == true,    0.00);
        edit(heights,   masky_minuspiby2    == true,    0.00);

        // maxing hill-amplitudes for now
        finalHeights    = sum<0>(heights, finalHeights);
    }

    // appending z-coordinates to the points
    points2D    =   concatenate<0>(points2D, finalHeights);

}