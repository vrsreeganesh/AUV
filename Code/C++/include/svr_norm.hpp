// =========================================================
template <typename T>
auto norm(const     std::vector<T>&     input_vector)
{
    return std::sqrt(std::inner_product(input_vector.begin(), input_vector.end(),
                                        input_vector.begin(),
                                        (T)0));
}



/*
Templates to create
    -   matrix and norm-axis
    -   axis instantiated std::vector<T>
*/ 