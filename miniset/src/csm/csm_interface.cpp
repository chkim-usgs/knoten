#include "csm/csm_interface.hpp"
#include <stdexcept>

#ifdef MINISET_HAS_CSMAPI
#include <csm/SettableEllipsoid.h>
#endif

namespace csm {

#ifdef MINISET_HAS_CSMAPI
std::pair<double, double> getRadii(const ::csm::RasterGM* camera) {
    if (!camera) throw std::invalid_argument("Null camera");

    // Try to cast to SettableEllipsoid to get ellipsoid parameters
    const ::csm::SettableEllipsoid* settable =
        dynamic_cast<const ::csm::SettableEllipsoid*>(camera);

    if (settable) {
        ::csm::Ellipsoid ellipsoid = settable->getEllipsoid();
        return {ellipsoid.getSemiMajorRadius(), ellipsoid.getSemiMinorRadius()};
    }

    // Fallback to WGS84 if cast fails
    return {6378137.0, 6356752.314245};
}

SensorState getSensorState(const ::csm::RasterGM* sensor, double line, double sample) {
    if (!sensor) throw std::invalid_argument("Null sensor");
    
    SensorState state;
    ::csm::ImageCoord img_pt(line, sample);
    
    state.sensor_time = sensor->getImageTime(img_pt);
    
    ::csm::EcefLocus locus = sensor->imageToRemoteImagingLocus(img_pt);
    state.sensor_pos = Vec3(locus.point.x, locus.point.y, locus.point.z);
    state.look_vec = Vec3(locus.direction.x, locus.direction.y, locus.direction.z);
    
    return state;
}

::csm::RasterGM* createCsmFromISD(const std::string& isd_file) {
    // Placeholder - requires full CSM plugin loading
    throw std::runtime_error("createCsmFromISD not yet implemented");
}
#endif

} // namespace csm
