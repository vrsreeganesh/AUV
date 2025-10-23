// // header-files
// #include <iostream>
// #include <ostream>
// #include <cmath>

// // Including classes
// #include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/include/ScattererClass.h"

// // Including functions
// #include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Functions/fCart2Sph.cpp"
// #include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Functions/fPrintTensorSize.cpp"
// #include "/Users/vrsreeganesh/Documents/GitHub/AUV/Code/C++/Functions/fSph2Cart.cpp"

// #pragma once

// // hash defines
// #ifndef PRINTSPACE
// #   define PRINTSPACE      std::cout<<"\n\n\n\n\n\n\n\n"<<std::endl;
// #endif
// #ifndef PRINTSMALLLINE
// #   define PRINTSMALLLINE  std::cout<<"------------------------------------------------"<<std::endl;
// #endif
// #ifndef PRINTLINE
// #   define PRINTLINE       std::cout<<"================================================"<<std::endl;
// #endif

// #define PI              3.14159265
// #define DEBUGMODE_TRANSMITTER       false

// #ifndef DEVICE
//     #define DEVICE          torch::kMPS
//     // #define DEVICE          torch::kCPU
// #endif



// // control panel
// #define ENABLE_RAYTRACING                false








// class TransmitterClass{
// public:

//     // physical/intrinsic properties
//     torch::Tensor location;             // location tensor 
//     torch::Tensor pointing_direction;   // pointing direction

//     // basic parameters
//     torch::Tensor Signal;       // transmitted signal (LFM)
//     float azimuthal_angle;      // transmitter's azimuthal pointing direction
//     float elevation_angle;      // transmitter's elevation pointing direction
//     float azimuthal_beamwidth;  // azimuthal beamwidth of transmitter
//     float elevation_beamwidth;  // elevation beamwidth of transmitter
//     float range;                // a parameter used for spotlight mode. 

//     // transmitted signal attributes
//     float f_low;                // lowest frequency of LFM
//     float f_high;               // highest frequency of LFM
//     float fc;                   // center frequency of LFM
//     float bandwidth;            // bandwidth of LFM

//     // shadowing properties
//     int azimuthQuantDensity;            // quantization of angles along the azimuth
//     int elevationQuantDensity;          // quantization of angles along the elevation
//     float rangeQuantSize;               // range-cell size when shadowing
//     float azimuthShadowThreshold;       // azimuth thresholding 
//     float elevationShadowThreshold;     // elevation thresholding
    
//     // // shadowing related
//     // torch::Tensor checkbox;             // box indicating whether a scatter for a range-angle pair has been found
//     // torch::Tensor finalScatterBox;      // a 3D tensor where the third dimension represnets the vector length
//     // torch::Tensor finalReflectivityBox; // to store the reflectivity



//     // Constructor
//     TransmitterClass(torch::Tensor location     = torch::zeros({3,1}), 
//                      torch::Tensor Signal       = torch::zeros({10,1}), 
//                      float azimuthal_angle      = 0, 
//                      float elevation_angle      = -30, 
//                      float azimuthal_beamwidth  = 30, 
//                      float elevation_beamwidth  = 30):
//                      location(location), 
//                      Signal(Signal), 
//                      azimuthal_angle(azimuthal_angle), 
//                      elevation_angle(elevation_angle), 
//                      azimuthal_beamwidth(azimuthal_beamwidth), 
//                      elevation_beamwidth(elevation_beamwidth) {}

//     // overloading output 
//     friend std::ostream& operator<<(std::ostream& os, TransmitterClass& transmitter){
//         os<<"\t> azimuth            : "<<transmitter.azimuthal_angle    <<std::endl;
//         os<<"\t> elevation          : "<<transmitter.elevation_angle    <<std::endl;
//         os<<"\t> azimuthal beamwidth: "<<transmitter.azimuthal_beamwidth<<std::endl;
//         os<<"\t> elevation beamwidth: "<<transmitter.elevation_beamwidth<<std::endl;
//         PRINTSMALLLINE
//         return os;
//     }

//     // overloading copyign operator
//     TransmitterClass& operator=(const TransmitterClass& other){

//         // checking self-assignment
//         if(this==&other){
//             return *this;
//         }

//         // allocating memory
//         this->location              = other.location;
//         this->Signal                = other.Signal;
//         this->azimuthal_angle       = other.azimuthal_angle;
//         this->elevation_angle       = other.elevation_angle;
//         this->azimuthal_beamwidth   = other.azimuthal_beamwidth;
//         this->elevation_beamwidth   = other.elevation_beamwidth;
//         this->range                 = other.range;

//         // transmitted signal attributes
//         this->f_low                 = other.f_low;
//         this->f_high                = other.f_high;
//         this->fc                    = other.fc;
//         this->bandwidth             = other.bandwidth;

//         // shadowing properties
//         this->azimuthQuantDensity       = other.azimuthQuantDensity;
//         this->elevationQuantDensity     = other.elevationQuantDensity;
//         this->rangeQuantSize            = other.rangeQuantSize;
//         this->azimuthShadowThreshold    = other.azimuthShadowThreshold;
//         this->elevationShadowThreshold  = other.elevationShadowThreshold;
        
//         // this->checkbox                  = other.checkbox;
//         // this->finalScatterBox           = other.finalScatterBox;
//         // this->finalReflectivityBox      = other.finalReflectivityBox;

//         // returning 
//         return *this;

//     };

//     /*==========================================================================
//     Aim: Update pointing angle
//     ----------------------------------------------------------------------------
//     Note:
//         > This function updates pointing angle based on AUV's pointing angle
//         > for now, we're assuming no roll;
//     --------------------------------------------------------------------------*/
//     void updatePointingAngle(torch::Tensor AUV_pointing_vector){

//         // calculate yaw and pitch
//         if(DEBUGMODE_TRANSMITTER) std::cout<<"\t TransmitterClass: page 140 \n";
//         torch::Tensor AUV_pointing_vector_spherical = fCart2Sph(AUV_pointing_vector);
//         torch::Tensor yaw                           = AUV_pointing_vector_spherical[0];
//         torch::Tensor pitch                         = AUV_pointing_vector_spherical[1];
//         if(DEBUGMODE_TRANSMITTER) std::cout<<"\t TransmitterClass: page 144 \n";

//         // std::cout<<"\t TransmitterClass: AUV_pointing_vector = "<<torch::transpose(AUV_pointing_vector, 0, 1)<<std::endl;
//         // std::cout<<"\t TransmitterClass: AUV_pointing_vector_spherical = "<<torch::transpose(AUV_pointing_vector_spherical, 0, 1)<<std::endl;

//         // calculating azimuth and elevation of transmitter object
//         torch::Tensor absolute_azimuth_of_transmitter   = yaw + torch::tensor({this->azimuthal_angle}).to(DATATYPE).to(DEVICE);
//         torch::Tensor absolute_elevation_of_transmitter = pitch + torch::tensor({this->elevation_angle}).to(DATATYPE).to(DEVICE);
//         if(DEBUGMODE_TRANSMITTER) std::cout<<"\t TransmitterClass: page 149 \n";

//         // std::cout<<"\t TransmitterClass: this->azimuthal_angle = "<<this->azimuthal_angle<<std::endl;
//         // std::cout<<"\t TransmitterClass: this->elevation_angle = "<<this->elevation_angle<<std::endl;
//         // std::cout<<"\t TransmitterClass: absolute_azimuth_of_transmitter = "<<absolute_azimuth_of_transmitter<<std::endl;
//         // std::cout<<"\t TransmitterClass: absolute_elevation_of_transmitter = "<<absolute_elevation_of_transmitter<<std::endl;

//         // converting back to Cartesian
//         torch::Tensor pointing_direction_spherical  = torch::zeros({3,1}).to(DATATYPE).to(DEVICE);
//         pointing_direction_spherical[0]             = absolute_azimuth_of_transmitter;
//         pointing_direction_spherical[1]             = absolute_elevation_of_transmitter;
//         pointing_direction_spherical[2]             = torch::tensor({1}).to(DATATYPE).to(DEVICE);
//         if(DEBUGMODE_TRANSMITTER) std::cout<<"\t TransmitterClass: page 60 \n";

//         this->pointing_direction = fSph2Cart(pointing_direction_spherical);
//         if(DEBUGMODE_TRANSMITTER) std::cout<<"\t TransmitterClass: page 169 \n";
        
//     }

//     /*==========================================================================
//     Aim: Subsetting Scatterers inside the cone
//     ............................................................................
//     steps:
//         1. Find azimuth and range of all points. 
//         2. Fint azimuth and range of current pointing vector. 
//         3. Subtract azimuth and range of points from that of azimuth and range of current pointing vector
//         4. Use tilted ellipse equation to find points in the ellipse
//     --------------------------------------------------------------------------*/
//     void subsetScatterers(ScattererClass* scatterers,
//                           float tilt_angle){

//         // translationally change origin 
//         scatterers->coordinates = \
//             scatterers->coordinates - this->location; 



//         /*
//         Note: I think something we can do is see if we can subset the matrices by checking coordinate values right away. If one of the coordinate values is x (relative coordiantes), we know for sure that the distance is greater than x, for sure. So, maybe that's something that we can work with 
//         */

//         // Finding spherical coordinates of scatterers and pointing direction
//         torch::Tensor scatterers_spherical          = fCart2Sph(scatterers->coordinates);
//         torch::Tensor pointing_direction_spherical  = fCart2Sph(this->pointing_direction);


//         // Calculating relative azimuths and radians
//         torch::Tensor relative_spherical = \
//             scatterers_spherical - pointing_direction_spherical;

        
//         // clearing some stuff up
//         scatterers_spherical.reset();
//         pointing_direction_spherical.reset();


//         // tensor corresponding to switch. 
//         torch::Tensor tilt_angle_Tensor = \
//             torch::tensor({tilt_angle}).to(DATATYPE).to(DEVICE);

//         // calculating length of axes
//         torch::Tensor axis_a    = \
//             torch::tensor({
//                 this->azimuthal_beamwidth / 2
//                 }).to(DATATYPE).to(DEVICE);
//         torch::Tensor axis_b    = \
//             torch::tensor({
//                 this->elevation_beamwidth / 2
//                 }).to(DATATYPE).to(DEVICE);
        
//         // part of calculating the tilted ellipse
//         torch::Tensor xcosa = relative_spherical[0] * torch::cos(tilt_angle_Tensor * PI/180);
//         torch::Tensor ysina = relative_spherical[1] * torch::sin(tilt_angle_Tensor * PI/180);
//         torch::Tensor xsina = relative_spherical[0] * torch::sin(tilt_angle_Tensor * PI/180);
//         torch::Tensor ycosa = relative_spherical[1] * torch::cos(tilt_angle_Tensor * PI/180);
//         relative_spherical.reset();


//         // finding points inside the tilted ellipse
//         torch::Tensor scatter_boolean = \
//             torch::div(torch::square(xcosa + ysina), torch::square(axis_a)) + \
//             torch::div(torch::square(xsina - ycosa), torch::square(axis_b)) <= 1;


//         // clearing
//         xcosa.reset(); ysina.reset(); xsina.reset(); ycosa.reset();


//         // subsetting points within the elliptical beam
//         auto mask                   = (scatter_boolean == 1);    // creating a mask 
//         scatterers->coordinates     = scatterers->coordinates.index({torch::indexing::Slice(),  mask});
//         scatterers->reflectivity    = scatterers->reflectivity.index({torch::indexing::Slice(), mask});


//         // this is where histogram shadowing comes in (later)
//         if (ENABLE_RAYTRACING) {
//             rangeHistogramShadowing(scatterers); 
//         }

//         // translating back to the points
//         scatterers->coordinates = scatterers->coordinates + this->location;

//     }

//     /*==========================================================================
//     Aim: Shadowing method (range-histogram shadowing)
//     ............................................................................
//     Note:
//         > cut down the number of threads into range-cells
//         > for each range cell, calculate histogram
//         > 
//         std::cout<<"\t TransmitterClass: "
//     --------------------------------------------------------------------------*/ 
//     void rangeHistogramShadowing(ScattererClass* scatterers){
        
//         // converting points to spherical coordinates
//         torch::Tensor spherical_coordinates = fCart2Sph(scatterers->coordinates); std::cout<<"\t\t TransmitterClass: line 252 "<<std::endl;
        
//         // finding maximum range
//         torch::Tensor maxdistanceofpoints = torch::max(spherical_coordinates[2]); std::cout<<"\t\t TransmitterClass: line 256 "<<std::endl;

//         // calculating number of range-cells  (verified)
//         int numrangecells = std::ceil(maxdistanceofpoints.item<int>()/this->rangeQuantSize);

//         // finding range-cell boundaries  (verified)
//         torch::Tensor rangeBoundaries = \
//             torch::linspace(this->rangeQuantSize, \
//                             numrangecells * this->rangeQuantSize,\
//                             numrangecells); std::cout<<"\t\t TransmitterClass: line 263 "<<std::endl;

//         // creating the checkbox  (verified)
//         int numazimuthcells     = std::ceil(this->azimuthal_beamwidth * this->azimuthQuantDensity);
//         int numelevationcells   = std::ceil(this->elevation_beamwidth * this->elevationQuantDensity); std::cout<<"\t\t TransmitterClass: line 267 "<<std::endl;

//         // finding the deltas
//         float delta_azimuth    = this->azimuthal_beamwidth / numazimuthcells;
//         float delta_elevation  = this->elevation_beamwidth / numelevationcells; std::cout<<"\t\t TransmitterClass: line 271"<<std::endl;

//         // creating the centers (verified)
//         torch::Tensor azimuth_centers   = torch::linspace(delta_azimuth/2, \
//                                                           numazimuthcells   * delta_azimuth   - delta_azimuth/2, \
//                                                           numazimuthcells);
//         torch::Tensor elevation_centers = torch::linspace(delta_elevation/2, \
//                                                           numelevationcells * delta_elevation - delta_elevation/2, \
//                                                           numelevationcells); std::cout<<"\t\t TransmitterClass: line 279"<<std::endl;

//         // centering (verified)
//         azimuth_centers     = azimuth_centers + torch::tensor({this->azimuthal_angle - \
//                                                                (this->azimuthal_beamwidth/2)}).to(DATATYPE);
//         elevation_centers   = elevation_centers + torch::tensor({this->elevation_angle - \
//                                                                  (this->elevation_beamwidth/2)}).to(DATATYPE);  std::cout<<"\t\t TransmitterClass: line 285"<<std::endl;

//         // building checkboxes
//         torch::Tensor checkbox              = torch::zeros({numelevationcells, numazimuthcells}, torch::kBool);
//         torch::Tensor finalScatterBox       = torch::zeros({numelevationcells, numazimuthcells, 3}).to(DATATYPE);
//         torch::Tensor finalReflectivityBox  = torch::zeros({numelevationcells, numazimuthcells}).to(DATATYPE);  std::cout<<"\t\t TransmitterClass: line 290"<<std::endl;

//         // going through each-range-cell
//         for(int i = 0; i<(int)rangeBoundaries.numel(); ++i){
//             this->internal_subsetCurrentRangeCell(rangeBoundaries[i],       \
//                                                   scatterers,               \
//                                                   checkbox,                 \
//                                                   finalScatterBox,          \
//                                                   finalReflectivityBox,     \
//                                                   azimuth_centers,          \
//                                                   elevation_centers,        \
//                                                   spherical_coordinates);  std::cout<<"\t\t TransmitterClass: line 301"<<std::endl;

//             // after each-range-cell
//             torch::Tensor checkboxfilled = torch::sum(checkbox);
//             std::cout<<"\t\t\t\t checkbox-filled = "<<checkboxfilled.item<int>()<<"/"<<checkbox.numel()<<" | percent = "<<100 * checkboxfilled.item<float>()/(float)checkbox.numel()<<std::endl;
            
//         }

//         // converting from box structure to [3, num-points] structure
//         torch::Tensor final_coords_spherical = \
//             torch::permute(finalScatterBox, {2, 0, 1}).reshape({3, (int)(finalScatterBox.numel()/3)});
//         torch::Tensor final_coords_cart = fSph2Cart(final_coords_spherical);  std::cout<<"\t\t TransmitterClass: line 308"<<std::endl;
//         std::cout<<"\t\t finalReflectivityBox.shape = "; fPrintTensorSize(finalReflectivityBox);
//         torch::Tensor final_reflectivity = finalReflectivityBox.reshape({finalReflectivityBox.numel()}); std::cout<<"\t\t TransmitterClass: line 310"<<std::endl;
//         torch::Tensor test_checkbox = checkbox.reshape({checkbox.numel()});  std::cout<<"\t\t TransmitterClass: line 311"<<std::endl;

//         // just taking the points corresponding to the filled. Else, there's gonna be a lot of zero zero zero tensors
//         auto mask = (test_checkbox == 1);  std::cout<<"\t\t TransmitterClass: line 319"<<std::endl;
//         final_coords_cart   = final_coords_cart.index({torch::indexing::Slice(), mask});  std::cout<<"\t\t TransmitterClass: line 320"<<std::endl;
//         final_reflectivity  = final_reflectivity.index({mask}); std::cout<<"\t\t TransmitterClass: line 321"<<std::endl;

//         // overwriting the scatterers
//         scatterers->coordinates     = final_coords_cart;
//         scatterers->reflectivity    = final_reflectivity; std::cout<<"\t\t TransmitterClass: line 324"<<std::endl;

//     }


//     void internal_subsetCurrentRangeCell(torch::Tensor rangeupperlimit,         \
//                                          ScattererClass* scatterers,            \
//                                          torch::Tensor& checkbox,               \
//                                          torch::Tensor& finalScatterBox,        \
//                                          torch::Tensor& finalReflectivityBox,   \
//                                          torch::Tensor& azimuth_centers,        \
//                                          torch::Tensor& elevation_centers,      \
//                                          torch::Tensor& spherical_coordinates){
        
//         // finding indices for points in the current range-cell
//         torch::Tensor pointsincurrentrangecell = \
//             torch::mul((spherical_coordinates[2] <= rangeupperlimit) , \
//                        (spherical_coordinates[2] > rangeupperlimit - this->rangeQuantSize));

//         // checking out if there are no points in this range-cell
//         int num311 = torch::sum(pointsincurrentrangecell).item<int>();
//         if(num311 == 0) return;
        
//         // calculating delta values
//         float delta_azimuth    = azimuth_centers[1].item<float>() - azimuth_centers[0].item<float>();
//         float delta_elevation  = elevation_centers[1].item<float>() - elevation_centers[0].item<float>();

//         // subsetting points in the current range-cell
//         auto mask                                       = (pointsincurrentrangecell == 1);    // creating a mask 
//         torch::Tensor reflectivityincurrentrangecell    = scatterers->reflectivity.index({torch::indexing::Slice(), mask});
//         pointsincurrentrangecell                        = spherical_coordinates.index({torch::indexing::Slice(),  mask});
        
//         // finding number of azimuth sizes and what not
//         int numazimuthcells     = azimuth_centers.numel();
//         int numelevationcells   = elevation_centers.numel();

//         // go through all the combinations
//         for(int azi_index = 0; azi_index < numazimuthcells; ++azi_index){
//             for(int ele_index = 0; ele_index < numelevationcells; ++ele_index){

//                 // check if this particular azimuth-elevation direction has been taken-care of. 
//                 if (checkbox[ele_index][azi_index].item<bool>()) break;

//                 // init (verified)
//                 torch::Tensor current_azimuth   = azimuth_centers.index({azi_index});
//                 torch::Tensor current_elevation = elevation_centers.index({ele_index});

//                 // // finding azimuth boolean 
//                 // torch::Tensor azi_neighbours    = torch::abs(pointsincurrentrangecell[0] - current_azimuth);
//                 // azi_neighbours                  = azi_neighbours <= delta_azimuth;  // tinker with this. 

//                 // // finding elevation boolean 
//                 // torch::Tensor ele_neighbours    = torch::abs(pointsincurrentrangecell[1] - current_elevation);
//                 // ele_neighbours                  = ele_neighbours <= delta_elevation;

//                 // finding azimuth boolean 
//                 torch::Tensor azi_neighbours    = torch::abs(pointsincurrentrangecell[0] - current_azimuth);
//                 azi_neighbours                  = azi_neighbours <= this->azimuthShadowThreshold;  // tinker with this. 

//                 // finding elevation boolean 
//                 torch::Tensor ele_neighbours    = torch::abs(pointsincurrentrangecell[1] - current_elevation);
//                 ele_neighbours                  = ele_neighbours <= this->elevationShadowThreshold;


//                 // combining booleans: means find all points that are within the limits of both the azimuth and boolean. 
//                 torch::Tensor neighbours_boolean = torch::mul(azi_neighbours, ele_neighbours);

//                 // checking if there are any points along this direction
//                 int num347 = torch::sum(neighbours_boolean).item<int>(); 
//                 if (num347 == 0) continue;

//                 // findings point along this direction
//                 mask                                        = (neighbours_boolean == 1);
//                 torch::Tensor coords_along_aziele_spherical = pointsincurrentrangecell.index({torch::indexing::Slice(),       mask});
//                 torch::Tensor reflectivity_along_aziele     = reflectivityincurrentrangecell.index({torch::indexing::Slice(), mask});

//                 // finding the index where the points are at the maximum distance 
//                 int index_where_min_range_is = torch::argmin(coords_along_aziele_spherical[2]).item<int>();
//                 torch::Tensor closest_coord = coords_along_aziele_spherical.index({torch::indexing::Slice(), \
//                                                                                    index_where_min_range_is});
//                 torch::Tensor closest_reflectivity = reflectivity_along_aziele.index({torch::indexing::Slice(), \
//                                                                                       index_where_min_range_is});

//                 // filling the matrices up
//                 finalScatterBox.index_put_({ele_index, azi_index, torch::indexing::Slice()}, \
//                                            closest_coord.reshape({1,1,3}));
//                 finalReflectivityBox.index_put_({ele_index, azi_index}, \
//                                                 closest_reflectivity);
//                 checkbox.index_put_({ele_index, azi_index}, \
//                                     true);
                
//             }
//         }
//     }




// };



template <typename T>
class TransmitterClass{
public:

    // A shared pointer to the configuration object
    std::shared_ptr<svr::AUVParameters> config_ptr;

    // physical/intrinsic properties
    std::vector<T>      location;               // location tensor 
    std::vector<T>      pointing_direction;     // pointing direction

    // basic parameters
    std::vector<T>      signal;                 // transmitted signal (LFM)
    T                   azimuthal_angle;        // transmitter's azimuthal pointing direction
    T                   elevation_angle;        // transmitter's elevation pointing direction
    T                   azimuthal_beamwidth;    // azimuthal beamwidth of transmitter
    T                   elevation_beamwidth;    // elevation beamwidth of transmitter
    T                   range;                  // a parameter used for spotlight mode. 

    // transmitted signal attributes
    T                   f_low;                  // lowest frequency of LFM
    T                   f_high;                 // highest frequency of LFM
    T                   fc;                     // center frequency of LFM
    T                   bandwidth;              // bandwidth of LFM
    T                   speed_of_sound {1500};     // speed of sound

    // shadowing properties
    int                 azimuthQuantDensity;        // quantization of angles along the azimuth
    int                 elevationQuantDensity;      // quantization of angles along the elevation
    T                   rangeQuantSize;             // range-cell size when shadowing
    T                   azimuthShadowThreshold;     // azimuth thresholding 
    T                   elevationShadowThreshold;   // elevation thresholding
    
    // shadowing related
    std::vector<T>      checkbox;             // box indicating whether a scatter for a range-angle pair has been found
    std::vector<std::vector<std::vector<T>>> finalScatterBox;      // a 3D tensor where the third dimension represnets the vector length
    std::vector<T> finalReflectivityBox; // to store the reflectivity

    // constructor
    TransmitterClass()  = default;

    // Deleting copy constructors/assignment
    TransmitterClass(const TransmitterClass&    other)          = delete;
    TransmitterClass&   operator=(TransmitterClass& other)      = delete;

    // Creating move-constructor and move-assignment
    TransmitterClass(TransmitterClass&& other)                  = default;
    TransmitterClass&   operator=(TransmitterClass&& other)     = default;

    // member-functions
    auto updatePointingAngle(std::vector<T> AUV_pointing_vector);
    auto subset_scatterers(const ScattererClass<T>&    seafloor,
                           std::vector<std::size_t>&   indices,
                           const T&                    tilt_angle);

};

// Definitions
#include    "TransmitterClass_subset_scatterers.inl"
#include    "TransmitterClass_updatePointingAngle.inl"