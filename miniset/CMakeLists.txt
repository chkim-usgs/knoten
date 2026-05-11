cmake_minimum_required(VERSION 3.15)
project(miniset VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build options
option(MINISET_BUILD_TESTS "Build tests" ON)
option(MINISET_BUILD_EXAMPLES "Build examples" ON)
option(MINISET_ENABLE_OPENMP "Enable OpenMP parallelization" ON)

# Performance flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-march=native -O3 -Wall -Wextra -Wpedantic)
elseif(MSVC)
    add_compile_options(/W4 /O2 /arch:AVX2)
endif()

# Find dependencies
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Optional dependencies - only error if trying to use features that need them
find_package(CSMAPI)
find_package(USGSCSM)
find_package(GDAL)
find_package(Ceres QUIET)  # Re-enabled to test with conda clang
find_package(PROJ)

if(MINISET_ENABLE_OPENMP)
    find_package(OpenMP)
endif()

# Library sources - Phases 1-6
set(MINISET_SOURCES
    # Phase 1: Core utilities
    src/core/math.cpp
    src/core/transforms.cpp

    # Phase 2: Surface models
    src/surface/ellipsoid.cpp
    src/surface/dem.cpp
    src/surface/intersection.cpp

    # Phase 3: CSM integration
    src/csm/csm_interface.cpp
    src/csm/camera_ops.cpp
    src/csm/triangulation.cpp

    # Phase 4: Sensor utilities
    src/sensor/angles.cpp
    src/sensor/resolution.cpp
    src/sensor/geometry.cpp

    # Phase 5: Bundle adjustment
    src/bundle/ceres_optimizer.cpp
    src/bundle/cost_functors.cpp
    src/bundle/partials.cpp

    # Phase 6: Coordinate transforms
    src/utils/coordinate_transforms.cpp
    src/utils/proj_wrapper.cpp
)

# Main library target
add_library(miniset SHARED ${MINISET_SOURCES})

target_include_directories(miniset
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link dependencies if found
if(CSMAPI_FOUND)
    target_link_libraries(miniset PUBLIC CSMAPI::csmapi)
    target_compile_definitions(miniset PUBLIC MINISET_HAS_CSMAPI)
endif()

if(USGSCSM_FOUND)
    target_link_libraries(miniset PUBLIC USGSCSM::usgscsm)
    target_compile_definitions(miniset PUBLIC MINISET_HAS_USGSCSM)
endif()

if(Ceres_FOUND)
    target_link_libraries(miniset PUBLIC Ceres::ceres)
    target_compile_definitions(miniset PUBLIC MINISET_HAS_CERES)
endif()

if(GDAL_FOUND)
    target_link_libraries(miniset PRIVATE GDAL::GDAL)
    target_compile_definitions(miniset PRIVATE MINISET_HAS_GDAL)
endif()

if(PROJ_FOUND)
    target_link_libraries(miniset PRIVATE PROJ::proj)
    target_compile_definitions(miniset PRIVATE MINISET_HAS_PROJ)
endif()

if(MINISET_ENABLE_OPENMP AND OpenMP_CXX_FOUND)
    target_link_libraries(miniset PRIVATE OpenMP::OpenMP_CXX)
    target_compile_definitions(miniset PRIVATE MINISET_HAS_OPENMP)
endif()

# Tests
if(MINISET_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Examples
if(MINISET_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Installation
install(TARGETS miniset
    EXPORT miniset-targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/ DESTINATION include
        FILES_MATCHING PATTERN "*.hpp")

install(EXPORT miniset-targets
    FILE miniset-config.cmake
    NAMESPACE miniset::
    DESTINATION lib/cmake/miniset
)
