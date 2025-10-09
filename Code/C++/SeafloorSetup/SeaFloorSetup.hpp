void fSeaFloorSetup(
    ScattererClass<double>&     scatterers
){

    // auto    save_files      {false};
    const   auto    save_files              {false};
    const   auto    hill_creation_flag      {true};

    // sea-floor bounds
	auto	bed_width	    {100.00};
	auto	bed_length	    {100.00};

	// creating tensors for coordinates and reflectivity
	vector<vector<double>>	        box_coordinates;
	vector<double>					box_reflectivity;

	// scatter density
	// auto	bed_width_density	{static_cast<double>(	10.00)};
	// auto 	bed_length_density	{static_cast<double>(	10.00)};
    auto	bed_width_density	{static_cast<double>(	5.00)};
	auto 	bed_length_density	{static_cast<double>(	5.00)};

	// setting up coordinates
	auto	xpoints		{linspace<double>(0.00,
                                          bed_width,
                                          bed_width * bed_width_density)};
    auto    ypoints     {linspace<double>(0.00,
                                          bed_length,
                                          bed_length * bed_length_density)};
    if(save_files)  fWriteVector(xpoints,   "../csv-files/xpoints.csv");        // verified
    if(save_files)  fWriteVector(ypoints,   "../csv-files/ypoints.csv");        // verified

    // creating mesh
    auto [xgrid, ygrid] = meshgrid(std::move(xpoints), std::move(ypoints));
    if(save_files)  fWriteMatrix(xgrid,     "../csv-files/xgrid.csv");          // verified
    if(save_files)  fWriteMatrix(ygrid,     "../csv-files/ygrid.csv");          // verified

    // reshaping 
    auto    X       {reshape(xgrid, xgrid.size()*xgrid[0].size())};
    auto    Y       {reshape(ygrid, ygrid.size()*ygrid[0].size())};
    if(save_files)  fWriteVector(X,         "../csv-files/X.csv");              // verified
    if(save_files)  fWriteVector(Y,         "../csv-files/Y.csv");              // verified

    // creating heights of scatterers
    if(hill_creation_flag){

        // setting up hill parameters
        auto    num_hills       {10};

        // setting up placement of hills
        auto    points2D                {concatenate<0>(X, Y)};                     // verified
        auto    min2D                   {min<1, double>(points2D)};                 // verified
        auto    max2D                   {max<1, double>(points2D)};                 // verified
        auto    hill_2D_center          {min2D + \
                                         rand({2, num_hills}) * (max2D - min2D)};   // verified

        // setup: hill-dimensions
        auto    hill_dimensions_min     {transpose(vector<double>{5, 5, 2})};       // verified
        auto    hill_dimensions_max     {transpose(vector<double>{30, 30, 10})};    // verified
        auto    hill_dimensions         {hill_dimensions_min + \
                                         rand({3, num_hills}) * (hill_dimensions_max - hill_dimensions_min)};                       // verified

        // function-call: hill-creation function
        fCreateHills(hill_2D_center, 
                     hill_dimensions, 
                     points2D);

        // setting up floor reflectivity
        auto   floorScatter_reflectivity        {std::vector<double>(Y.size(), 1.00)};

        // populating the values of the incoming argument
        scatterers.coordinates      = std::move(points2D);
        scatterers.reflectivity     = std::move(floorScatter_reflectivity);
        
    }
    else{

        // assigning flat heights
        auto    Z       {std::vector<double>(Y.size(), 0)};

        // setting up floor coordinates
        auto    floorScatter_coordinates        {concatenate<0>(X, Y, Z)};
        auto    floorScatter_reflectivity       {std::vector<double>(Y.size(), 1)};

        // populating the values of the incoming argument        
        scatterers.coordinates      = std::move(floorScatter_coordinates);
        scatterers.reflectivity     = std::move(floorScatter_reflectivity);

    }

    // printing status
    std::cout << format("> Finished Sea-Floor Setup \n");
}