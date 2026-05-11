#ifndef MINISET_BUNDLE_BUNDLE_TYPES_HPP
#define MINISET_BUNDLE_BUNDLE_TYPES_HPP

#include "core/types.hpp"

namespace bundle {

// Bundle adjustment configuration
struct BundleConfig {
    int max_iterations = 50;
    double function_tolerance = 1e-6;
    double parameter_tolerance = 1e-8;
    bool use_sparse_solver = true;
    int num_threads = 1;
};

// Bundle adjustment result
struct BundleResult {
    int num_iterations;
    double final_cost;
    double sigma0;
    bool converged;
};

} // namespace bundle

#endif
