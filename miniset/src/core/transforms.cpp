#include "core/transforms.hpp"

namespace transforms {

GroundPointsBatch imageToGroundBatch(const ImageCoordsBatch& coords, double height) {
    GroundPointsBatch result;
    result.resize(coords.count);
    // Placeholder implementation
    for (size_t i = 0; i < coords.count; ++i) {
        result.x[i] = coords.sample[i];
        result.y[i] = coords.line[i];
        result.z[i] = height;
    }
    return result;
}

ImageCoordsBatch groundToImageBatch(const GroundPointsBatch& points) {
    ImageCoordsBatch result;
    result.resize(points.count);
    // Placeholder implementation
    for (size_t i = 0; i < points.count; ++i) {
        result.sample[i] = points.x[i];
        result.line[i] = points.y[i];
    }
    return result;
}

} // namespace transforms
