#include "utils/coordinate_transforms.hpp"
#include "core/math.hpp"
#include <cmath>

namespace utils {

#ifdef MINISET_HAS_PROJ
GroundPointsBatch transformCoordinates(const GroundPointsBatch& input,
                                       const std::string& source_proj,
                                       const std::string& dest_proj) {
    // Placeholder - requires PROJ implementation
    return input;
}
#endif

LatLon ecefToLatLon(const Vec3& ecef_pt, double semi_major, double semi_minor) {
    double lon = std::atan2(ecef_pt.y, ecef_pt.x);
    double p = std::sqrt(ecef_pt.x * ecef_pt.x + ecef_pt.y * ecef_pt.y);
    double lat = std::atan2(ecef_pt.z, p);
    
    // Iterative refinement for geodetic latitude
    double e2 = 1.0 - (semi_minor * semi_minor) / (semi_major * semi_major);
    for (int i = 0; i < 5; ++i) {
        double sin_lat = std::sin(lat);
        double N = semi_major / std::sqrt(1.0 - e2 * sin_lat * sin_lat);
        lat = std::atan2(ecef_pt.z + e2 * N * sin_lat, p);
    }
    
    return LatLon(lat, lon);
}

Vec3 latLonToEcef(double lat_rad, double lon_rad, double height,
                  double semi_major, double semi_minor) {
    double e2 = 1.0 - (semi_minor * semi_minor) / (semi_major * semi_major);
    double sin_lat = std::sin(lat_rad);
    double cos_lat = std::cos(lat_rad);
    double N = semi_major / std::sqrt(1.0 - e2 * sin_lat * sin_lat);

    double x = (N + height) * cos_lat * std::cos(lon_rad);
    double y = (N + height) * cos_lat * std::sin(lon_rad);
    double z = (N * (1.0 - e2) + height) * sin_lat;

    return Vec3(x, y, z);
}

std::vector<LatLon> batchEcefToLatLon(const GroundPointsBatch& batch,
                                      double semi_major, double semi_minor) {
    std::vector<LatLon> results;
    results.reserve(batch.count);

    for (size_t i = 0; i < batch.count; ++i) {
        Vec3 pt{batch.x[i], batch.y[i], batch.z[i]};
        results.push_back(ecefToLatLon(pt, semi_major, semi_minor));
    }

    return results;
}

} // namespace utils
