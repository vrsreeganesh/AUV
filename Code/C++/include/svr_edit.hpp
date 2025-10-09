#pragma once
/*==============================================================================
Matlab's equivalent of A[A < 0.5] = 0
------------------------------------------------------------------------------*/ 
template <typename T, typename U>
auto edit(std::vector<T>&                   input_vector,
          const std::vector<bool>&          bool_vector,
          const     U                       scalar)
{
    // throwing an error
    if (input_vector.size() != bool_vector.size())
        std::cerr << "edit: incompatible size\n";

    // overwriting input-vector
    std::transform(input_vector.begin(), input_vector.end(),
                   bool_vector.begin(),
                   input_vector.begin(),
                   [&scalar](auto& argx, auto argy){
                        if(argy == true)    {return static_cast<T>(scalar);}
                        else                {return argx;}
                   });

    // no-returns since in-place
}

/*==============================================================================
accumulate version of edit, instead of just placing values

THings to add
    - ensuring template only accepts int, std::size_t and similar for T2
    - bring in histogram method to ensure SIMD
-------------------------------------------------------------------------------*/
template    <typename T1, 
             typename T2>
auto    edit_accumulate(std::vector<T1>&            input_vector,
                        const   std::vector<T2>&    indices_to_edit,
                        const   std::vector<T1>&    new_values)
{
    // certain checks
    if (indices_to_edit.size() != new_values.size())
        std::cerr << "svr::edit | edit_accumulate | size-disparity occured \n";

    // going through each and accumulating
    for(auto    i = 0; i < input_vector.size(); ++i){
        const   auto   target_index    {static_cast<std::size_t>(indices_to_edit[i])};   // 
        const   auto   new_value       {new_values[i]};
        if (target_index < input_vector.size()){
            input_vector[target_index] = input_vector[target_index] + new_value;
        }
        else{
            // std::cout << "warning: FILE: svr_edit.hpp | FUNCTION: edit_accumulate | REPORT: index out of bounds";
        }
    }
    
    // no-return since in-place
} 