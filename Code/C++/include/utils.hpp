// // =============================================================================
#include "svr_WriteToCSV.hpp"
// // =============================================================================
// template <typename F, typename R>
// constexpr auto fElementWise(F&& func, R& range){
//     std::transform(std::begin(range),
//                    std::end(range),
//                    std::begin(range),
//                    std::forward<F>(func));
//     // return range;
// }
// // =============================================================================
// #include "svr_repmat.hpp"
// // =============================================================================
// auto SineElementWise(auto& input, auto constantvalue){
//     for(auto& x: input) {x = std::sin(constantvalue * x);}
//     // replace this with std::transform
// };
// // =============================================================================
#include "svr_linspace.hpp"
#include "svr_meshgrid.hpp"
#include "svr_reshape.hpp"
#include "svr_concatenate.hpp"
#include "svr_min.hpp"
#include "svr_max.hpp"
#include "svr_rand.hpp"
#include "svr_operators.hpp"
#include "svr_operator_plus.hpp"
#include "svr_operator_subtraction.hpp"
#include "svr_operator_star.hpp"
#include "svr_data_structures.hpp"
#include "svr_print_containers.hpp"
#include "svr_transpose.hpp"
#include "svr_cosine.hpp"
#include "svr_abs.hpp"
#include "svr_exponentiate.hpp"
#include "svr_operator_division.hpp"
#include "svr_boolean_comparators.hpp"
#include "svr_edit.hpp"
#include "svr_sum.hpp"
#include "svr_norm.hpp"
#include "svr_equality.hpp"
#include "svr_tan.hpp"
#include "svr_tile.hpp"
#include "svr_fft.hpp"
#include "svr_conv.hpp"
#include "svr_index.hpp"
#include "svr_flip.hpp"
#include "svr_conj.hpp"


#include "svr_coordinate_change.hpp"



// Pipeline functions
#include "fCreateHills.hpp"


// // ======================================================
// #include "svr_fft.hpp"
// // =============================================================================
// #include "svr_rand.hpp"
// // =============================================================================
// #include "svr_operator_star.hpp"
// // =============================================================================
// #include "svr_operators.hpp"
// // =============================================================================
// #include "svr_tensor_inits.hpp"
// // =============================================================================
// #include "svr_sin.hpp"
// // =============================================================================
// #include "svr_slice.hpp"
// // =============================================================================
// #include "svr_matrix_operations.hpp"
// // =============================================================================
// #include <boost/type_index.hpp>
// template <typename T>
// auto type(T inputarg){
//     std::cout << boost::typeindex::type_id_with_cvr<decltype(inputarg)>().pretty_name()<< "\n";
// }
// // =============================================================================
// #include "svr_shape.hpp"
// // =============================================================================
// #include "svr_sum.hpp"
