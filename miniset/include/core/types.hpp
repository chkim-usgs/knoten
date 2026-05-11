#ifndef MINISET_CORE_TYPES_HPP
#define MINISET_CORE_TYPES_HPP

#include <vector>
#include <cstddef>


// Simple 3D vector type
struct Vec3 {
    double x, y, z;

    Vec3() : x(0.0), y(0.0), z(0.0) {}
    Vec3(double x_, double y_, double z_) : x(x_), y(y_), z(z_) {}
};

// Batch ground points (ECEF coordinates) - Structure of Arrays
struct GroundPointsBatch {
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> z;
    size_t count;

    GroundPointsBatch() : count(0) {}

    void resize(size_t n) {
        x.resize(n);
        y.resize(n);
        z.resize(n);
        count = n;
    }

    void clear() {
        x.clear();
        y.clear();
        z.clear();
        count = 0;
    }
};

// Batch image coordinates - Structure of Arrays
struct ImageCoordsBatch {
    std::vector<double> line;
    std::vector<double> sample;
    size_t count;

    ImageCoordsBatch() : count(0) {}

    void resize(size_t n) {
        line.resize(n);
        sample.resize(n);
        count = n;
    }

    void clear() {
        line.clear();
        sample.clear();
        count = 0;
    }
};

// Sensor states for batch processing - Structure of Arrays
struct SensorStatesBatch {
    std::vector<double> sensor_pos_x;
    std::vector<double> sensor_pos_y;
    std::vector<double> sensor_pos_z;
    std::vector<double> look_vec_x;
    std::vector<double> look_vec_y;
    std::vector<double> look_vec_z;
    std::vector<double> sensor_time;
    size_t count;

    SensorStatesBatch() : count(0) {}

    void resize(size_t n) {
        sensor_pos_x.resize(n);
        sensor_pos_y.resize(n);
        sensor_pos_z.resize(n);
        look_vec_x.resize(n);
        look_vec_y.resize(n);
        look_vec_z.resize(n);
        sensor_time.resize(n);
        count = n;
    }

    void clear() {
        sensor_pos_x.clear();
        sensor_pos_y.clear();
        sensor_pos_z.clear();
        look_vec_x.clear();
        look_vec_y.clear();
        look_vec_z.clear();
        sensor_time.clear();
        count = 0;
    }
};

// Bundle adjustment observation data
struct BundleObservations {
    std::vector<double> observed_line;
    std::vector<double> observed_sample;
    std::vector<int> point_id;
    std::vector<int> image_id;
    size_t num_observations;

    BundleObservations() : num_observations(0) {}

    void resize(size_t n) {
        observed_line.resize(n);
        observed_sample.resize(n);
        point_id.resize(n);
        image_id.resize(n);
        num_observations = n;
    }

    void clear() {
        observed_line.clear();
        observed_sample.clear();
        point_id.clear();
        image_id.clear();
        num_observations = 0;
    }
};

// Bundle adjustment ground points
struct BundlePoints {
    std::vector<double> x;
    std::vector<double> y;
    std::vector<double> z;
    std::vector<int> point_type;  // 2=free, 4=fixed
    std::vector<double> covariance;  // 6 values per point (upper triangle)
    size_t num_points;

    BundlePoints() : num_points(0) {}

    void resize(size_t n) {
        x.resize(n);
        y.resize(n);
        z.resize(n);
        point_type.resize(n);
        covariance.resize(n * 6);  // 6 values per point
        num_points = n;
    }

    void clear() {
        x.clear();
        y.clear();
        z.clear();
        point_type.clear();
        covariance.clear();
        num_points = 0;
    }
};

// Bundle adjustment image parameters
struct BundleImageParams {
    std::vector<std::vector<double>> params;  // Per-image parameter vectors
    std::vector<int> param_indices;           // Which parameters are adjustable
    size_t num_images;

    BundleImageParams() : num_images(0) {}

    void resize(size_t n) {
        params.resize(n);
        num_images = n;
    }

    void clear() {
        params.clear();
        param_indices.clear();
        num_images = 0;
    }
};

// Latitude/Longitude in radians
struct LatLon {
    double lat;
    double lon;

    LatLon() : lat(0.0), lon(0.0) {}
    LatLon(double lat_, double lon_) : lat(lat_), lon(lon_) {}
};

// Spherical coordinates
struct Sphere {
    double lat;    // radians
    double lon;    // radians
    double radius; // meters

    Sphere() : lat(0.0), lon(0.0), radius(0.0) {}
    Sphere(double lat_, double lon_, double radius_)
        : lat(lat_), lon(lon_), radius(radius_) {}
};


#endif // MINISET_CORE_TYPES_HPP
