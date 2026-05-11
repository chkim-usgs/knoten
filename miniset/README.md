# Miniset

A high-performance C++ implementation of the Knoten library following data-oriented programming principles.

## Overview

Miniset is a geospatial sensor modeling framework that provides:

- **CSM Integration**: Community Sensor Model (CSM) sensor operations and camera models
- **Bundle Adjustment**: Ceres Solver-based optimization for refining sensor positions
- **Photometric Calculations**: Phase, emission, and incidence angle computations
- **Coordinate Transformations**: Image↔ground transformations with DEM support
- **Surface Models**: Ellipsoid and DEM surface representations
- **Ray Tracing**: Fast ray-surface intersection algorithms

## Design Philosophy

This library follows **data-oriented programming (DOP)** principles:

- **Structure-of-Arrays (SoA)**: Cache-friendly data layouts for batch processing
- **Pure Functions**: Transformation pipelines with immutable data
- **Parallelization**: OpenMP-based CPU parallelization for critical operations
- **Zero-Cost Abstractions**: Modern C++17 with minimal runtime overhead

## Building

### Requirements

- C++17 compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.15+
- Optional dependencies:
  - CSMAPI (for CSM sensor models)
  - USGSCSM (for USGS CSM implementations)
  - Ceres Solver (for bundle adjustment)
  - GDAL (for DEM I/O)
  - PROJ (for coordinate transformations)
  - GoogleTest (for tests)
  - OpenMP (for parallelization)

### Build Instructions

```bash
cd miniset
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### Build Options

```bash
cmake .. \
  -DKNOTEN_BUILD_TESTS=ON \
  -DKNOTEN_BUILD_EXAMPLES=ON \
  -DKNOTEN_ENABLE_OPENMP=ON
```

### Running Tests

```bash
cd build
ctest --output-on-failure
```

## Project Status

**Phase 1: Foundation** (In Progress)
- ✅ Core data structures (SoA layouts)
- ✅ Vector math utilities
- ✅ Build system (CMake)
- ✅ Test infrastructure (GoogleTest)
- 🔄 CMake finder modules

**Phase 2-8**: See implementation plan in project documentation

## Directory Structure

```
miniset/
├── include/knoten/     # Public headers
│   ├── core/           # Core types and transforms
│   ├── csm/            # CSM integration
│   ├── bundle/         # Bundle adjustment
│   ├── sensor/         # Sensor utilities
│   ├── surface/        # Surface models
│   └── utils/          # Utility functions
├── src/                # Implementation files
├── tests/              # Unit tests (GoogleTest)
├── examples/           # Usage examples
├── cmake/              # CMake modules
└── docs/               # Documentation
```

## Usage Example

```cpp
#include <knoten/core/math.hpp>

using namespace knoten;

int main() {
    // Create vectors
    Vec3 a{1.0, 0.0, 0.0};
    Vec3 b{0.0, 1.0, 0.0};
    
    // Compute separation angle
    double angle = math::separationAngle(a, b);
    
    // angle ≈ π/2 (90 degrees)
    
    return 0;
}
```

## Performance

Expected performance improvements over Python implementation:

- **Vector operations**: 50-100x faster
- **Bundle adjustment**: 10-50x faster (with Ceres + OpenMP)
- **Batch transformations**: 20-100x faster (SIMD + parallelization)

## License

See main Knoten repository for license information.

## Contributing

This is a rewrite of the Python Knoten library. See the main repository for contribution guidelines.

## References

- [Knoten (Python)](../README.md)
- [CSM (usgscsm)](https://github.com/DOI-USGS/usgscsm)
- [ALE](https://github.com/DOI-USGS/ale)
- [Ceres Solver](http://ceres-solver.org/)
