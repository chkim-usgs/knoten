#ifndef MINISET_CSM_CAMERA_OPS_HPP
#define MINISET_CSM_CAMERA_OPS_HPP

#include "core/types.hpp"
#include <vector>

#ifdef MINISET_HAS_CSMAPI
#include <csm/RasterGM.h>
#endif

namespace csm {

#ifdef MINISET_HAS_CSMAPI
// Image to ground conversion
Vec3 imageToGround(const ::csm::RasterGM* sensor, double line, double sample, double height);

// Ground to image conversion
void groundToImage(const ::csm::RasterGM* sensor, const Vec3& ground_pt, double& line, double& sample);

// Generate boundary points
std::vector<ImageCoord> generateBoundary(int width, int height, int npoints = 10);
#endif

} // namespace csm

#endif
