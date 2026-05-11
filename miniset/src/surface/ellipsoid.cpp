#include "surface/ellipsoid.hpp"
#include "core/math.hpp"
#include <cmath>
#include <stdexcept>

namespace surface {

Ellipsoid::Ellipsoid(double semi_major, double semi_minor)
    : a_(semi_major), b_(semi_major), c_(semi_minor) {
    // Biaxial ellipsoid (sphere or oblate/prolate spheroid)
    if (a_ <= 0 || c_ <= 0) {
        throw std::invalid_argument("Ellipsoid radii must be positive");
    }
}

Ellipsoid::Ellipsoid(double semi_major, double semi_median, double semi_minor)
    : a_(semi_major), b_(semi_median), c_(semi_minor) {
    // Triaxial ellipsoid
    if (a_ <= 0 || b_ <= 0 || c_ <= 0) {
        throw std::invalid_argument("Ellipsoid radii must be positive");
    }
}

Vec3 Ellipsoid::getSurfaceNormal(const Vec3& ground_pt) const {
    // Surface normal for ellipsoid: grad(x²/a² + y²/b² + z²/c²)
    // = (2x/a², 2y/b², 2z/c²)
    Vec3 normal;
    normal.x = ground_pt.x / (a_ * a_);
    normal.y = ground_pt.y / (b_ * b_);
    normal.z = ground_pt.z / (c_ * c_);

    // Normalize to unit vector
    return math::unitVector(normal);
}

Vec3 Ellipsoid::intersectSurface(const Vec3& sensor_pos, const Vec3& look_vec) const {
    // Ray: P(t) = sensor_pos + t * look_vec
    // Ellipsoid: (x/a)² + (y/b)² + (z/c)² = 1
    //
    // Substituting ray into ellipsoid equation gives quadratic in t:
    // At² + Bt + C = 0

    double dx = look_vec.x / a_;
    double dy = look_vec.y / b_;
    double dz = look_vec.z / c_;

    double px = sensor_pos.x / a_;
    double py = sensor_pos.y / b_;
    double pz = sensor_pos.z / c_;

    double A = dx * dx + dy * dy + dz * dz;
    double B = 2.0 * (px * dx + py * dy + pz * dz);
    double C = px * px + py * py + pz * pz - 1.0;

    double discriminant = B * B - 4.0 * A * C;

    if (discriminant < 0) {
        // No intersection - return closest point on ray to origin
        double t_closest = -B / (2.0 * A);
        return Vec3(
            sensor_pos.x + t_closest * look_vec.x,
            sensor_pos.y + t_closest * look_vec.y,
            sensor_pos.z + t_closest * look_vec.z
        );
    }

    // Two solutions - take the closer one (smaller positive t)
    double sqrt_disc = std::sqrt(discriminant);
    double t1 = (-B - sqrt_disc) / (2.0 * A);
    double t2 = (-B + sqrt_disc) / (2.0 * A);

    double t = (t1 >= 0) ? t1 : t2;

    return Vec3(
        sensor_pos.x + t * look_vec.x,
        sensor_pos.y + t * look_vec.y,
        sensor_pos.z + t * look_vec.z
    );
}

double Ellipsoid::getRadiusAt(double lat_rad, double lon_rad) const {
    // Radius at geodetic latitude and longitude
    // r = (a*b*c) / sqrt((b*c*cos(lat)*cos(lon))² + (a*c*cos(lat)*sin(lon))² + (a*b*sin(lat))²)

    double cos_lat = std::cos(lat_rad);
    double sin_lat = std::sin(lat_rad);
    double cos_lon = std::cos(lon_rad);
    double sin_lon = std::sin(lon_rad);

    double bc_cos_lat_cos_lon = b_ * c_ * cos_lat * cos_lon;
    double ac_cos_lat_sin_lon = a_ * c_ * cos_lat * sin_lon;
    double ab_sin_lat = a_ * b_ * sin_lat;

    double denom = std::sqrt(
        bc_cos_lat_cos_lon * bc_cos_lat_cos_lon +
        ac_cos_lat_sin_lon * ac_cos_lat_sin_lon +
        ab_sin_lat * ab_sin_lat
    );

    if (denom < 1e-10) {
        return a_;  // Fallback to semi-major axis
    }

    return (a_ * b_ * c_) / denom;
}

} // namespace surface
