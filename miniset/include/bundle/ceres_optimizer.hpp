#ifndef MINISET_BUNDLE_CERES_OPTIMIZER_HPP
#define MINISET_BUNDLE_CERES_OPTIMIZER_HPP

#include "bundle/bundle_types.hpp"

namespace bundle {

// Closest approach algorithm (multi-ray triangulation)
std::pair<Vec3, std::vector<double>> closestApproach(
    const std::vector<Vec3>& points,
    const std::vector<Vec3>& directions
);

#ifdef MINISET_HAS_CERES
// Run bundle adjustment with Ceres
BundleResult runBundleAdjustment(BundleObservations& observations,
                                 BundlePoints& points,
                                 BundleImageParams& image_params,
                                 const BundleConfig& config);
#endif

} // namespace bundle

#endif
