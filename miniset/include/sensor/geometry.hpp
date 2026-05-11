#ifndef MINISET_SENSOR_GEOMETRY_HPP
#define MINISET_SENSOR_GEOMETRY_HPP

#include "core/types.hpp"

namespace sensor {

// Compute distance from sensor to ground point
double slantDistance(const Vec3& sensor_pos, const Vec3& ground_pt);

// Compute local radius at ground point
double localRadius(const Vec3& ground_pt);

} // namespace sensor

#endif
