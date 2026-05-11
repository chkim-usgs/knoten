#include "csm/csm_interface.hpp"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <dlfcn.h>
#include <cstdlib>

#ifdef MINISET_HAS_CSMAPI
#include <csm/SettableEllipsoid.h>
#include <csm/Plugin.h>
#include <csm/Isd.h>
#include <csm/Model.h>
#include <csm/Warning.h>
#include <csm/Error.h>
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
    // Load USGSCSM plugin library if not already loaded
    static void* plugin_handle = nullptr;
    if (!plugin_handle) {
        // Build plugin path from CONDA_PREFIX environment variable
        std::string plugin_path;
        const char* conda_prefix = std::getenv("CONDA_PREFIX");

        if (conda_prefix) {
            // Default: $CONDA_PREFIX/lib/csmplugins/libusgscsm.dylib (or .so)
#ifdef __APPLE__
            plugin_path = std::string(conda_prefix) + "/lib/csmplugins/libusgscsm.dylib";
#else
            plugin_path = std::string(conda_prefix) + "/lib/csmplugins/libusgscsm.so";
#endif
            plugin_handle = dlopen(plugin_path.c_str(), RTLD_LAZY | RTLD_GLOBAL);
        }

        // Fallback paths if CONDA_PREFIX not set or plugin not found there
        if (!plugin_handle) {
            const char* fallback_paths[] = {
                "../lib/csmplugins/libusgscsm.dylib",
                "../lib/csmplugins/libusgscsm.so",
                "./libusgscsm.dylib",
                "./libusgscsm.so",
                "libusgscsm.dylib",
                "libusgscsm.so",
                nullptr
            };

            for (const char** path = fallback_paths; *path != nullptr; ++path) {
                plugin_handle = dlopen(*path, RTLD_LAZY | RTLD_GLOBAL);
                if (plugin_handle) break;
            }
        }

        if (!plugin_handle) {
            std::string error_msg = "Failed to load USGSCSM plugin library";
            if (conda_prefix) {
                error_msg += " (tried " + plugin_path + ")";
            }
            error_msg += ": " + std::string(dlerror());
            throw std::runtime_error(error_msg);
        }
    }

    // USGSCSM reads the JSON file directly when filename is set
    // The ISD object just needs the filename, and the plugin handles parsing
    ::csm::Isd isd(isd_file);

    // Find the USGS CSM plugin
    const ::csm::Plugin* plugin = ::csm::Plugin::findPlugin("UsgsAstroPluginCSM");
    if (!plugin) {
        throw std::runtime_error("Could not find UsgsAstroPluginCSM plugin");
    }

    // Try all available sensor models until one works
    // USGS CSM plugin supports: FRAME, LINE_SCANNER, PROJECTED, SAR, PUSH_FRAME
    const char* model_names[] = {
        "USGS_ASTRO_LINE_SCANNER_SENSOR_MODEL",  // Most common (CTX, HiRISE, etc.)
        "USGS_ASTRO_FRAME_SENSOR_MODEL",         // Frame cameras (MDIS, etc.)
        "USGS_ASTRO_PUSH_FRAME_SENSOR_MODEL",    // Push frame
        "USGS_ASTRO_PROJECTED_SENSOR_MODEL",     // Projected/ortho
        "USGS_ASTRO_SAR_SENSOR_MODEL",           // SAR
        nullptr
    };

    ::csm::Model* model = nullptr;
    ::csm::WarningList warnings;
    std::string last_error;

    for (const char** model_name = model_names; *model_name != nullptr; ++model_name) {
        try {
            // Check if this model can be constructed
            if (plugin->canModelBeConstructedFromISD(isd, *model_name)) {
                model = plugin->constructModelFromISD(isd, *model_name, &warnings);
                if (model) break;  // Success!
            }
        } catch (const ::csm::Error& e) {
            last_error = std::string(e.getMessage());
            continue;  // Try next model
        }
    }

    if (!model) {
        throw std::runtime_error("Failed to construct any sensor model from ISD. Last error: " + last_error);
    }

    if (!model) {
        throw std::runtime_error("Failed to construct model from ISD");
    }

    // Cast to RasterGM
    ::csm::RasterGM* raster_model = dynamic_cast<::csm::RasterGM*>(model);
    if (!raster_model) {
        delete model;
        throw std::runtime_error("Model is not a RasterGM");
    }

    // Print warnings if any
    for (const auto& warning : warnings) {
        // Could log these, but for now just ignore
        (void)warning;
    }

    return raster_model;
}
#endif

} // namespace csm
