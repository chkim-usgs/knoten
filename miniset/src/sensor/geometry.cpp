#include "sensor/geometry.hpp"
#include "core/math.hpp"

namespace sensor {

double slantDistance(const Vec3& sensor_pos, const Vec3& ground_pt) {
    return math::distance(sensor_pos, ground_pt);
}

double localRadius(const Vec3& ground_pt) {
    return math::magnitude(ground_pt);
}

} // namespace sensor
