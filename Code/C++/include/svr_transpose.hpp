#pragma once
/*==============================================================================
------------------------------------------------------------------------------*/ 
template <typename T>
auto transpose(const    std::vector<T>&     input_vector){
    
    // creating canvas
    auto    canvas      {std::vector<std::vector<T>>{
        input_vector.size(),
        std::vector<T>(1)
    }};
    
    // filling canvas
    for(auto i = 0; i < input_vector.size(); ++i){
        canvas[i][0]    = input_vector[i];
    }

    // moving it back
    return std::move(canvas);
}