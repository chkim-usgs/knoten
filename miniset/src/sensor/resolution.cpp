#include "sensor/resolution.hpp"

namespace sensor {

Resolution computeResolution(double ground_distance, double image_distance) {
    Resolution res;
    if (image_distance > 0) {
        res.line_resolution = ground_distance / image_distance;
        res.sample_resolution = ground_distance / image_distance;
        res.pixel_resolution = ground_distance / image_distance;
    } else {
        res.line_resolution = 0.0;
        res.sample_resolution = 0.0;
        res.pixel_resolution = 0.0;
    }
    return res;
}

} // namespace sensor
