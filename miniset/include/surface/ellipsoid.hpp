#ifndef MINISET_SURFACE_ELLIPSOID_HPP
#define MINISET_SURFACE_ELLIPSOID_HPP

#include "core/types.hpp"

namespace surface {

// Ellipsoid surface model (biaxial or triaxial)
class Ellipsoid {
public:
    // Constructors
    Ellipsoid(double semi_major, double semi_minor);
    Ellipsoid(double semi_major, double semi_median, double semi_minor);

    // Factory method from CSM sensor
#ifdef MINISET_HAS_CSMAPI
    static Ellipsoid fromCsmSensor(const void* sensor);
#endif

    // Get radii
    double getSemiMajorA() const { return a_; }
    double getSemiMedianB() const { return b_; }
    double getSemiMinorC() const { return c_; }

    // Get surface normal at a point
    Vec3 getSurfaceNormal(const Vec3& ground_pt) const;

    // Ray-ellipsoid intersection
    Vec3 intersectSurface(const Vec3& sensor_pos, const Vec3& look_vec) const;

    // Get radius at geodetic point
    double getRadiusAt(double lat_rad, double lon_rad) const;

private:
    double a_;  // Semi-major axis (equatorial, x-axis)
    double b_;  // Semi-median axis (equatorial, y-axis)
    double c_;  // Semi-minor axis (polar, z-axis)
};

} // namespace surface

#endif // MINISET_SURFACE_ELLIPSOID_HPP
