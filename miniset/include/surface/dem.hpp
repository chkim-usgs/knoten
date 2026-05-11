#ifndef MINISET_SURFACE_DEM_HPP
#define MINISET_SURFACE_DEM_HPP

#include "surface/ellipsoid.hpp"
#include <string>

#ifdef MINISET_HAS_GDAL
#include <gdal_priv.h>
#endif

namespace surface {

// DEM type
enum class DEMType {
    HEIGHT,  // Values are heights above ellipsoid
    RADIUS   // Values are radii from center
};

// Base ellipsoid DEM (no raster data)
class EllipsoidDEM {
public:
    EllipsoidDEM(double semi_major, double semi_minor = 0.0);
    virtual ~EllipsoidDEM() = default;

    // Get height above ellipsoid (always 0 for pure ellipsoid)
    virtual double getHeight(double lat_rad, double lon_rad) const;

    // Get radius from center
    virtual double getRadius(double lat_rad, double lon_rad) const;

    double getSemiMajorA() const { return a_; }
    double getSemiMajorB() const { return b_; }
    double getSemiMinorC() const { return c_; }

protected:
    double a_, b_, c_;  // Ellipsoid radii
};

#ifdef MINISET_HAS_GDAL
// GDAL-backed DEM
class GdalDEM : public EllipsoidDEM {
public:
    GdalDEM(const std::string& dem_file, double semi_major, double semi_minor = 0.0,
            DEMType dem_type = DEMType::HEIGHT);
    ~GdalDEM();

    // Disable copy (GDAL dataset is not copyable)
    GdalDEM(const GdalDEM&) = delete;
    GdalDEM& operator=(const GdalDEM&) = delete;

    // Get height above ellipsoid
    double getHeight(double lat_rad, double lon_rad) const override;

    // Get radius from center
    double getRadius(double lat_rad, double lon_rad) const override;

    // Get raw raster value
    double getRasterValue(double lat_rad, double lon_rad) const;

private:
    GDALDataset* dataset_;
    DEMType dem_type_;
    double no_data_value_;
    double geotransform_[6];

    void latLonToPixel(double lat_rad, double lon_rad, int& pixel_x, int& pixel_y) const;
};
#endif

} // namespace surface

#endif // MINISET_SURFACE_DEM_HPP
