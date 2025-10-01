/*==============================================================================
Dependencies
------------------------------------------------------------------------------*/ 
#pragma once
#include <vector>   // for std::vector
#include <utility>  // for std::pair
#include <complex>  // for std::complex


/*==============================================================================
mesh-grid when working with l-values
------------------------------------------------------------------------------*/ 
template <typename T>
auto meshgrid(const     std::vector<T>&    x,
              const     std::vector<T>&    y)
{

    // creating and filling x-grid
    std::vector<std::vector<T>> xcanvas(y.size(), std::vector<T>(x.size(), 0));
    for(auto row = 0; row < y.size(); ++row)  
        std::copy(x.begin(), x.end(), xcanvas[row].begin());

    // creating and filling y-grid
    std::vector<std::vector<T>> ycanvas(y.size(), std::vector<T>(x.size(), 0));
    for(auto col = 0; col < x.size(); ++col)
        for(auto row = 0; row < y.size(); ++row)
            ycanvas[row][col]   = y[row];

    // returning
    return std::move(std::pair{xcanvas, ycanvas});

}
/*==============================================================================
meshgrid when working with r-values
------------------------------------------------------------------------------*/ 
template <typename T>
auto meshgrid(std::vector<T>&&    x,
              std::vector<T>&&    y)
{    

    // creating and filling x-grid
    std::vector<std::vector<T>> xcanvas(y.size(), std::vector<T>(x.size(), 0));
    for(auto row = 0; row < y.size(); ++row)  
        std::copy(x.begin(), x.end(), xcanvas[row].begin());

    // creating and filling y-grid
    std::vector<std::vector<T>> ycanvas(y.size(), std::vector<T>(x.size(), 0));
    for(auto col = 0; col < x.size(); ++col)
        for(auto row = 0; row < y.size(); ++row)
            ycanvas[row][col]   = y[row];

    // returning
    return std::move(std::pair{xcanvas, ycanvas});

}