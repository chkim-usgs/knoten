#ifndef MINISET_CORE_TRANSFORMS_HPP
#define MINISET_CORE_TRANSFORMS_HPP

#include "core/types.hpp"
#include <vector>

namespace transforms {

// Batch image to ground transformations
std::vector<Vec3> imageToGroundBatch(const std::vector<ImageCoord>& coords, double height);

// Batch ground to image transformations
std::vector<ImageCoord> groundToImageBatch(const std::vector<Vec3>& points);

} // namespace transforms

#endif
