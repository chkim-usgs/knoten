#include "csm/camera_ops.hpp"

namespace csm {

#ifdef MINISET_HAS_CSMAPI
Vec3 imageToGround(const ::csm::RasterGM* sensor, double line, double sample, double height) {
    ::csm::ImageCoord img(line, sample);
    ::csm::EcefCoord ground = sensor->imageToGround(img, height);
    return Vec3(ground.x, ground.y, ground.z);
}

void groundToImage(const ::csm::RasterGM* sensor, const Vec3& ground_pt, double& line, double& sample) {
    ::csm::EcefCoord pt(ground_pt.x, ground_pt.y, ground_pt.z);
    ::csm::ImageCoord img = sensor->groundToImage(pt);
    line = img.line;
    sample = img.samp;
}

std::vector<ImageCoord> generateBoundary(int width, int height, int npoints) {
    std::vector<ImageCoord> boundary;
    boundary.reserve(npoints * 4);

    // Top edge
    for (int i = 0; i < npoints; ++i) {
        double frac = static_cast<double>(i) / (npoints - 1);
        boundary.push_back(ImageCoord(0.0, frac * width));
    }
    // Right edge
    for (int i = 0; i < npoints; ++i) {
        double frac = static_cast<double>(i) / (npoints - 1);
        boundary.push_back(ImageCoord(frac * height, width));
    }
    // Bottom edge
    for (int i = 0; i < npoints; ++i) {
        double frac = static_cast<double>(i) / (npoints - 1);
        boundary.push_back(ImageCoord(height, width - frac * width));
    }
    // Left edge
    for (int i = 0; i < npoints; ++i) {
        double frac = static_cast<double>(i) / (npoints - 1);
        boundary.push_back(ImageCoord(height - frac * height, 0.0));
    }

    return boundary;
}
#endif

} // namespace csm
