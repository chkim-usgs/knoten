#include "surface/dem.hpp"
#include <cmath>
#include <stdexcept>

namespace surface {

EllipsoidDEM::EllipsoidDEM(double semi_major, double semi_minor)
    : a_(semi_major), b_(semi_major), c_(semi_minor > 0 ? semi_minor : semi_major) {
    if (a_ <= 0 || c_ <= 0) {
        throw std::invalid_argument("Ellipsoid radii must be positive");
    }
}

double EllipsoidDEM::getHeight(double lat_rad, double lon_rad) const {
    // Pure ellipsoid has no height above itself
    return 0.0;
}

double EllipsoidDEM::getRadius(double lat_rad, double lon_rad) const {
    // Same calculation as Ellipsoid::getRadiusAt
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
        return a_;
    }

    return (a_ * b_ * c_) / denom;
}

#ifdef MINISET_HAS_GDAL
GdalDEM::GdalDEM(const std::string& dem_file, double semi_major, double semi_minor, DEMType dem_type)
    : EllipsoidDEM(semi_major, semi_minor), dataset_(nullptr), dem_type_(dem_type), no_data_value_(0.0) {

    GDALAllRegister();
    dataset_ = (GDALDataset*)GDALOpen(dem_file.c_str(), GA_ReadOnly);

    if (!dataset_) {
        throw std::runtime_error("Failed to open DEM file: " + dem_file);
    }

    // Get geotransform
    if (dataset_->GetGeoTransform(geotransform_) != CE_None) {
        throw std::runtime_error("Failed to get geotransform from DEM");
    }

    // Get no-data value
    GDALRasterBand* band = dataset_->GetRasterBand(1);
    if (band) {
        int has_no_data;
        no_data_value_ = band->GetNoDataValue(&has_no_data);
        if (!has_no_data) {
            no_data_value_ = -9999.0;  // Default
        }
    }
}

GdalDEM::~GdalDEM() {
    if (dataset_) {
        GDALClose(dataset_);
    }
}

void GdalDEM::latLonToPixel(double lat_rad, double lon_rad, int& pixel_x, int& pixel_y) const {
    // Convert radians to degrees
    double lon_deg = lon_rad * 180.0 / M_PI;
    double lat_deg = lat_rad * 180.0 / M_PI;

    // Apply inverse geotransform
    // geotransform: [0]=top-left x, [1]=pixel width, [2]=rotation, [3]=top-left y, [4]=rotation, [5]=pixel height
    double det = geotransform_[1] * geotransform_[5] - geotransform_[2] * geotransform_[4];

    if (std::abs(det) < 1e-10) {
        pixel_x = pixel_y = -1;
        return;
    }

    double dx = lon_deg - geotransform_[0];
    double dy = lat_deg - geotransform_[3];

    pixel_x = static_cast<int>((geotransform_[5] * dx - geotransform_[2] * dy) / det);
    pixel_y = static_cast<int>((geotransform_[1] * dy - geotransform_[4] * dx) / det);
}

double GdalDEM::getRasterValue(double lat_rad, double lon_rad) const {
    int pixel_x, pixel_y;
    latLonToPixel(lat_rad, lon_rad, pixel_x, pixel_y);

    // Check bounds
    int width = dataset_->GetRasterXSize();
    int height = dataset_->GetRasterYSize();

    if (pixel_x < 0 || pixel_x >= width || pixel_y < 0 || pixel_y >= height) {
        return no_data_value_;
    }

    // Read pixel value
    GDALRasterBand* band = dataset_->GetRasterBand(1);
    if (!band) {
        return no_data_value_;
    }

    float value;
    CPLErr err = band->RasterIO(GF_Read, pixel_x, pixel_y, 1, 1, &value, 1, 1, GDT_Float32, 0, 0);

    if (err != CE_None) {
        return no_data_value_;
    }

    // Check for no-data
    if (std::abs(value - no_data_value_) < 1e-6) {
        return no_data_value_;
    }

    return static_cast<double>(value);
}

double GdalDEM::getHeight(double lat_rad, double lon_rad) const {
    double raster_value = getRasterValue(lat_rad, lon_rad);

    if (std::abs(raster_value - no_data_value_) < 1e-6) {
        return 0.0;  // No data - return 0 height
    }

    if (dem_type_ == DEMType::HEIGHT) {
        return raster_value;
    } else {  // DEMType::RADIUS
        double ellipsoid_radius = EllipsoidDEM::getRadius(lat_rad, lon_rad);
        return raster_value - ellipsoid_radius;
    }
}

double GdalDEM::getRadius(double lat_rad, double lon_rad) const {
    double raster_value = getRasterValue(lat_rad, lon_rad);

    if (std::abs(raster_value - no_data_value_) < 1e-6) {
        return EllipsoidDEM::getRadius(lat_rad, lon_rad);  // No data - return ellipsoid radius
    }

    if (dem_type_ == DEMType::RADIUS) {
        return raster_value;
    } else {  // DEMType::HEIGHT
        double ellipsoid_radius = EllipsoidDEM::getRadius(lat_rad, lon_rad);
        return ellipsoid_radius + raster_value;
    }
}
#endif

} // namespace surface
