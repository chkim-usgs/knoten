#ifndef MINISET_UTILS_COORDINATE_TRANSFORMS_HPP
#define MINISET_UTILS_COORDINATE_TRANSFORMS_HPP

#include "core/types.hpp"
#include <string>

namespace utils {

#ifdef MINISET_HAS_PROJ
// Transform coordinates using PROJ
GroundPointsBatch transformCoordinates(const GroundPointsBatch& input,
                                       const std::string& source_proj,
                                       const std::string& dest_proj);
#endif

// ECEF to Lat/Lon/Alt
LatLon ecefToLatLon(const Vec3& ecef_pt, double semi_major, double semi_minor);

// Lat/Lon/Alt to ECEF
Vec3 latLonToEcef(double lat_rad, double lon_rad, double height,
                  double semi_major, double semi_minor);

// Batch conversions
std::vector<LatLon> batchEcefToLatLon(const GroundPointsBatch& batch,
                                      double semi_major, double semi_minor);

} // namespace utils

#endif
