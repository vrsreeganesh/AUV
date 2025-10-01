#pragma once
/*==============================================================================
input = [vector, vector], 
output = [vector]
------------------------------------------------------------------------------*/ 
template <std::size_t axis, typename T>
auto concatenate(const  std::vector<T>&     input_vector_A,
                 const  std::vector<T>&     input_vector_B) -> std::enable_if_t<axis == 1, std::vector<T> >
{
    // creating canvas vector
    auto    num_elements    {input_vector_A.size() + input_vector_B.size()};
    auto    canvas          {std::vector<T>(num_elements, (T)0) };

    // filling up the canvas
    std::copy(input_vector_A.begin(), input_vector_A.end(), 
              canvas.begin());
    std::copy(input_vector_B.begin(), input_vector_B.end(), 
              canvas.begin()+input_vector_A.size());

    // moving it back
    return std::move(canvas);

}
/*==============================================================================
input = [vector, vector], 
output = [matrix]
------------------------------------------------------------------------------*/ 
template <std::size_t axis, typename T>
auto concatenate(const  std::vector<T>&     input_vector_A,
                 const  std::vector<T>&     input_vector_B) -> std::enable_if_t<axis == 0, std::vector<std::vector<T>> >
{
    // throwing error dimensions
    if (input_vector_A.size() != input_vector_B.size())
        std::cerr << "concatenate:: incorrect dimensions \n";

    // creating canvas
    auto    canvas      {std::vector<std::vector<T>>(
        2, std::vector<T>(input_vector_A.size())
    )};

    // filling up the dimensions
    std::copy(input_vector_A.begin(), input_vector_A.end(), canvas[0].begin());
    std::copy(input_vector_B.begin(), input_vector_B.end(), canvas[1].begin());

    // moving it back
    return std::move(canvas);

}
/*==============================================================================
input = [vector, vector, vector], 
output = [matrix]
------------------------------------------------------------------------------*/ 
template <std::size_t axis, typename T>
auto concatenate(const  std::vector<T>&     input_vector_A,
                 const  std::vector<T>&     input_vector_B,
                 const  std::vector<T>&     input_vector_C) -> std::enable_if_t<axis == 0, std::vector<std::vector<T>> >
{
    // throwing error dimensions
    if (input_vector_A.size() != input_vector_B.size() || 
        input_vector_A.size() != input_vector_C.size())
        std::cerr << "concatenate:: incorrect dimensions \n";

    // creating canvas
    auto    canvas      {std::vector<std::vector<T>>(
        3, std::vector<T>(input_vector_A.size())
    )};

    // filling up the dimensions
    std::copy(input_vector_A.begin(), input_vector_A.end(), canvas[0].begin());
    std::copy(input_vector_B.begin(), input_vector_B.end(), canvas[1].begin());
    std::copy(input_vector_C.begin(), input_vector_C.end(), canvas[2].begin());

    // moving it back
    return std::move(canvas);

}
/*==============================================================================
input = [matrix, vector], 
output = [matrix]
------------------------------------------------------------------------------*/ 
template <std::size_t axis, typename T>
auto concatenate(const  std::vector<std::vector<T>>&    input_matrix,
                 const  std::vector<T>                  input_vector) -> std::enable_if_t<axis == 0, std::vector<std::vector<T>> >
{
    // creating canvas
    auto    canvas      {input_matrix};

    // adding to the canvas
    canvas.push_back(input_vector);

    // returning
    return std::move(canvas);
}