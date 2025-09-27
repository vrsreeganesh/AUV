// =============================================================================
template <typename T, typename BooleanVector, typename U>
auto edit(std::vector<T>&                   input_vector,
          BooleanVector                     bool_vector,
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
