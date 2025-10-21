#include "svr_WriteToCSV.hpp"
#include "svr_concepts.hpp"
#include "svr_sample.hpp"
#include "svr_linspace.hpp"
#include "svr_meshgrid.hpp"
#include "svr_reshape.hpp"
#include "svr_concatenate.hpp"
#include "svr_min.hpp"
#include "svr_max.hpp"
#include "svr_complex.hpp"
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

#include "svr_real.hpp"
#include "svr_imag.hpp"
#include "svr_slice.hpp"


#include "FFTPlanClass.hpp"
#include "IFFTPlanClass.hpp"

#include "FFTPlanPoolClass.hpp"
#include "FFTPlanUniformPoolHandle.hpp"

#include "IFFTPlanPoolClass.hpp"
#include "IFFTPlanUniformPoolHandle.hpp"

#include "svr_thread_pool.hpp"
#include "svr_conv.hpp"

#include "svr_index.hpp"
#include "svr_flip.hpp"
#include "svr_conj.hpp"
#include "svr_reset.hpp"
#include "svr_square.hpp"
#include "svr_mask.hpp"

#include "svr_tensor_init.hpp"
#include "svr_floor.hpp"
#include "svr_squeeze.hpp"

#include "auv_parameters.hpp"


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
