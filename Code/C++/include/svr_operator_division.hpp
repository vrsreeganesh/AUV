// =========================================================
// matrix division with scalars
template <typename T>
auto operator/(const    std::vector<T>&     input_vector,
               const    T&                  input_scalar)
{
    // creating canvas
    auto    canvas      {input_vector};

    // filling canvas
    std::transform(canvas.begin(), canvas.end(),
                   canvas.begin(),
                   [&input_scalar](const auto& argx){
                        return static_cast<double>(argx) / static_cast<double>(input_scalar);
                   });

    // returning value
    return std::move(canvas);
}
// =========================================================
// matrix division with scalars
template <typename T>
auto operator/=(const    std::vector<T>&     input_vector,
                const    T&                  input_scalar)
{
    // creating canvas
    auto    canvas      {input_vector};

    // filling canvas
    std::transform(canvas.begin(), canvas.end(),
                   canvas.begin(),
                   [&input_scalar](const auto& argx){
                        return static_cast<double>(argx) / static_cast<double>(input_scalar);
                   });

    // returning value
    return std::move(canvas);
}
