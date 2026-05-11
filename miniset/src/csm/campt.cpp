#include "csm/campt.hpp"

#ifdef MINISET_HAS_CSMAPI
#include "surface/ellipsoid.hpp"
#include "utils/coordinate_transforms.hpp"
#include "core/math.hpp"
#include <csm/csm.h>
#include <cmath>

namespace csm {

CamptInfo campt(const ::csm::RasterGM* sensor,
                double sample, double line,
                const Ellipsoid3& ellipsoid,
                double height) {
    CamptInfo info;

    // Store input
    info.sample = sample;
    info.line = line;

    // Get sensor state
    ::csm::ImageCoord img_coord(line, sample);
    info.sensor_time = sensor->getImageTime(img_coord);

    // Get sensor position
    ::csm::EcefCoord sensor_pos_csm = sensor->getSensorPosition(img_coord);
    info.sensor_position = Vec3(sensor_pos_csm.x, sensor_pos_csm.y, sensor_pos_csm.z);

    // Get look direction
    ::csm::EcefLocus locus = sensor->imageToRemoteImagingLocus(img_coord);
    info.look_direction = Vec3(locus.direction.x, locus.direction.y, locus.direction.z);

    // Compute ground intersection
    ::csm::EcefCoord ground_csm = sensor->imageToGround(img_coord, height);
    info.ground_point = Vec3(ground_csm.x, ground_csm.y, ground_csm.z);

    // Convert to lat/lon (planetocentric and planetographic)
    LatLon latlon = utils::ecefToLatLon(info.ground_point, ellipsoid.a, ellipsoid.c);
    info.planetocentric_lat = latlon.lat;
    info.positive_east_lon = latlon.lon;
    if (info.positive_east_lon < 0) info.positive_east_lon += 2.0 * M_PI;

    // Planetographic latitude
    double f = (ellipsoid.a - ellipsoid.c) / ellipsoid.a;  // flattening
    info.planetographic_lat = std::atan(std::tan(info.planetocentric_lat) / ((1.0 - f) * (1.0 - f)));
    info.height = height;

    // Compute photometric angles
    // Surface normal
    Vec3 normal = ellipsoid::getSurfaceNormal(ellipsoid, info.ground_point);

    // Vector from ground to sensor
    Vec3 to_sensor = {
        info.sensor_position.x - info.ground_point.x,
        info.sensor_position.y - info.ground_point.y,
        info.sensor_position.z - info.ground_point.z
    };

    // Emission angle (observer angle)
    double cos_emission = math::dotProduct(normal, math::unitVector(to_sensor));
    info.emission_angle = std::acos(std::clamp(cos_emission, -1.0, 1.0)) * 180.0 / M_PI;

    // For phase and incidence, we'd need sun position
    // Placeholder: assume zenith illumination for now
    double cos_incidence = normal.z / math::magnitude(normal);
    info.incidence_angle = std::acos(std::clamp(cos_incidence, -1.0, 1.0)) * 180.0 / M_PI;

    // Phase angle (simplified - angle between sun and sensor as seen from ground)
    info.phase_angle = std::abs(info.emission_angle - info.incidence_angle);

    // Compute resolution
    // Use finite difference: move 1 pixel and measure ground distance
    ::csm::ImageCoord img_plus_sample(line, sample + 1.0);
    ::csm::ImageCoord img_plus_line(line + 1.0, sample);

    ::csm::EcefCoord ground_plus_sample = sensor->imageToGround(img_plus_sample, height);
    ::csm::EcefCoord ground_plus_line = sensor->imageToGround(img_plus_line, height);

    Vec3 gp_sample(ground_plus_sample.x, ground_plus_sample.y, ground_plus_sample.z);
    Vec3 gp_line(ground_plus_line.x, ground_plus_line.y, ground_plus_line.z);

    info.sample_resolution = math::distance(info.ground_point, gp_sample);
    info.line_resolution = math::distance(info.ground_point, gp_line);
    info.pixel_resolution = (info.sample_resolution + info.line_resolution) / 2.0;

    // Compute distances
    info.slant_distance = math::distance(info.sensor_position, info.ground_point);
    info.target_center_distance = math::magnitude(info.sensor_position);
    info.local_radius = math::magnitude(info.ground_point);

    // Sub-spacecraft point (nadir point)
    Vec3 nadir = ellipsoid::intersectSurface(ellipsoid, info.sensor_position,
                                              math::scaleVector(info.sensor_position, -1.0));
    LatLon sub_sc = utils::ecefToLatLon(nadir, ellipsoid.a, ellipsoid.c);
    info.sub_spacecraft_lat = sub_sc.lat;
    info.sub_spacecraft_lon = sub_sc.lon;
    if (info.sub_spacecraft_lon < 0) info.sub_spacecraft_lon += 2.0 * M_PI;

    // Spacecraft altitude
    info.spacecraft_altitude = math::distance(info.sensor_position, nadir);

    // Off-nadir angle
    Vec3 nadir_vec = math::unitVector(math::scaleVector(info.sensor_position, -1.0));
    double cos_off_nadir = math::dotProduct(info.look_direction, nadir_vec);
    info.off_nadir_angle = std::acos(std::clamp(cos_off_nadir, -1.0, 1.0)) * 180.0 / M_PI;

    // North azimuth (azimuth of local north direction)
    Vec3 north_pole = {0, 0, 1};
    Vec3 east_dir = math::crossProduct(north_pole, normal);
    east_dir = math::unitVector(east_dir);
    Vec3 north_dir = math::crossProduct(normal, east_dir);
    north_dir = math::unitVector(north_dir);

    Vec3 ground_to_sensor_horiz = {
        to_sensor.x - math::dotProduct(to_sensor, normal) * normal.x,
        to_sensor.y - math::dotProduct(to_sensor, normal) * normal.y,
        to_sensor.z - math::dotProduct(to_sensor, normal) * normal.z
    };
    ground_to_sensor_horiz = math::unitVector(ground_to_sensor_horiz);

    double cos_north = math::dotProduct(ground_to_sensor_horiz, north_dir);
    double sin_north = math::dotProduct(ground_to_sensor_horiz, east_dir);
    info.north_azimuth = std::atan2(sin_north, cos_north) * 180.0 / M_PI;
    if (info.north_azimuth < 0) info.north_azimuth += 360.0;

    // Spacecraft azimuth and ground azimuth
    info.spacecraft_azimuth = info.north_azimuth;
    info.sub_spacecraft_ground_azimuth = info.spacecraft_azimuth;

    // Oblique resolution (slant resolution perpendicular to look direction)
    info.oblique_resolution = info.pixel_resolution / std::cos(info.emission_angle * M_PI / 180.0);

    // Sun information - try to get from CSM if available
    // For now, set defaults (would need SPICE or additional ISD data)
    info.sun_position = {0, 0, 0};
    info.sub_solar_lat = 0;
    info.sub_solar_lon = 0;
    info.solar_distance = 0;
    info.sub_solar_azimuth = 0;
    info.sub_solar_ground_azimuth = 0;
    info.solar_longitude = 0;
    info.local_solar_time = 0;

    // Celestial coordinates (RA/Dec) - would need J2000 transformation
    info.right_ascension = 0;
    info.declination = 0;

    // Look direction in different frames
    info.look_j2000 = {0, 0, 0};  // Would need frame transformation
    info.look_camera = {0, 0, 0}; // Would need camera model info

    return info;
}

std::vector<CamptInfo> camptBatch(const ::csm::RasterGM* sensor,
                                  const std::vector<::ImageCoord>& coords,
                                  const Ellipsoid3& ellipsoid,
                                  double height) {
    std::vector<CamptInfo> results;
    results.reserve(coords.size());

    // Note: miniset::ImageCoord uses 'sample', ::csm::ImageCoord uses 'samp'
    for (const auto& coord : coords) {
        results.push_back(campt(sensor, coord.sample, coord.line, ellipsoid, height));
    }

    return results;
}

} // namespace csm

#endif // MINISET_HAS_CSMAPI
