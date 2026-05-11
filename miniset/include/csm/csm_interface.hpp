#ifndef MINISET_CSM_CSM_INTERFACE_HPP
#define MINISET_CSM_CSM_INTERFACE_HPP

#include "core/types.hpp"
#include <string>

#ifdef MINISET_HAS_CSMAPI
#include <csm/RasterGM.h>
#endif

namespace csm {

// CSM sensor state
struct SensorState {
    Vec3 sensor_pos;
    Vec3 look_vec;
    double sensor_time;
};

#ifdef MINISET_HAS_CSMAPI
// Get radii from CSM sensor
std::pair<double, double> getRadii(const ::csm::RasterGM* camera);

// Get sensor state at image point
SensorState getSensorState(const ::csm::RasterGM* sensor, double line, double sample);

// Create CSM sensor from ISD file
::csm::RasterGM* createCsmFromISD(const std::string& isd_file);
#endif

} // namespace csm

#endif
