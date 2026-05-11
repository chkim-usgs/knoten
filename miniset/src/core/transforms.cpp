#include "core/transforms.hpp"

namespace transforms {

std::vector<Vec3> imageToGroundBatch(const std::vector<ImageCoord>& coords, double height) {
    std::vector<Vec3> result;
    result.reserve(coords.size());

    // Placeholder implementation
    for (const auto& coord : coords) {
        result.push_back(Vec3(coord.sample, coord.line, height));
    }

    return result;
}

std::vector<ImageCoord> groundToImageBatch(const std::vector<Vec3>& points) {
    std::vector<ImageCoord> result;
    result.reserve(points.size());

    // Placeholder implementation
    for (const auto& pt : points) {
        result.push_back(ImageCoord(pt.y, pt.x));
    }

    return result;
}

} // namespace transforms
