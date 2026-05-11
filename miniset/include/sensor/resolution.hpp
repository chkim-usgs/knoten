#ifndef MINISET_SENSOR_RESOLUTION_HPP
#define MINISET_SENSOR_RESOLUTION_HPP

namespace sensor {

struct Resolution {
    double line_resolution;    // meters/pixel
    double sample_resolution;  // meters/pixel  
    double pixel_resolution;   // average
};

// Placeholder for resolution calculations
Resolution computeResolution(double ground_distance, double image_distance);

} // namespace sensor

#endif
