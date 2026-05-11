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

ImageCoordsBatch generateBoundary(int width, int height, int npoints) {
    ImageCoordsBatch boundary;
    boundary.count = npoints * 4;
    boundary.line.reserve(boundary.count);
    boundary.sample.reserve(boundary.count);
    
    for (int i = 0; i < npoints; ++i) {
        double frac = static_cast<double>(i) / (npoints - 1);
        boundary.line.push_back(0.0);
        boundary.sample.push_back(frac * width);
    }
    for (int i = 0; i < npoints; ++i) {
        double frac = static_cast<double>(i) / (npoints - 1);
        boundary.line.push_back(frac * height);
        boundary.sample.push_back(width);
    }
    for (int i = 0; i < npoints; ++i) {
        double frac = static_cast<double>(i) / (npoints - 1);
        boundary.line.push_back(height);
        boundary.sample.push_back(width - frac * width);
    }
    for (int i = 0; i < npoints; ++i) {
        double frac = static_cast<double>(i) / (npoints - 1);
        boundary.line.push_back(height - frac * height);
        boundary.sample.push_back(0.0);
    }
    
    return boundary;
}
#endif

} // namespace csm
