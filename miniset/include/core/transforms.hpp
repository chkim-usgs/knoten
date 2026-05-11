#ifndef MINISET_CORE_TRANSFORMS_HPP
#define MINISET_CORE_TRANSFORMS_HPP

#include "core/types.hpp"

namespace transforms {

// Batch image to ground transformations
GroundPointsBatch imageToGroundBatch(const ImageCoordsBatch& coords, double height);

// Batch ground to image transformations  
ImageCoordsBatch groundToImageBatch(const GroundPointsBatch& points);

} // namespace transforms

#endif
