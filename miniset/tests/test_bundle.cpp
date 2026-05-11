#include <gtest/gtest.h>
#include "bundle/ceres_optimizer.hpp"
#include "core/math.hpp"
#include <cmath>

namespace bundle::tests {

// Test closest approach algorithm
TEST(BundleTest, ClosestApproachIntersect) {
    // Three rays that should intersect at (0, 1, 2)
    std::vector<Vec3> points = {
        {-1.0, 1.0, 2.0},
        {0.0, 2.0, 2.0},
        {0.0, 1.0, 3.0}
    };

    std::vector<Vec3> directions = {
        {1.0, 0.0, 0.0},
        {0.0, 2.0, 0.0},
        {0.0, 0.0, -1.0}
    };

    auto [result, covar] = closestApproach(points, directions);

    EXPECT_NEAR(result.x, 0.0, 1e-10);
    EXPECT_NEAR(result.y, 1.0, 1e-10);
    EXPECT_NEAR(result.z, 2.0, 1e-10);

    // Covariance should be a 3x3 matrix
    EXPECT_EQ(covar.size(), 9);
}

TEST(BundleTest, ClosestApproachNoIntersect) {
    // Three rays that don't exactly intersect, but closest approach is near (0, 1, 2)
    double sqrt3 = std::sqrt(3.0);

    std::vector<Vec3> points = {
        {-1.0, 1.0, 2.0},
        {0.5, 1.0 - sqrt3 / 2.0, 2.0},
        {0.5, 1.0 + sqrt3 / 2.0, 4.0}
    };

    std::vector<Vec3> directions = {
        {0.0, 1.0, 0.0},
        {sqrt3 / 2.0, 0.5, 0.0},
        {0.0, 0.0, 1.0}
    };

    auto [result, covar] = closestApproach(points, directions);

    EXPECT_NEAR(result.x, 0.0, 1e-10);
    EXPECT_NEAR(result.y, 1.0, 1e-10);
    EXPECT_NEAR(result.z, 2.0, 1e-10);
}

// Test basic bundle adjustment data structures
TEST(BundleTest, BundleObservationsCreation) {
    BundleObservations obs;
    obs.observed_line = {100.0, 200.0, 300.0};
    obs.observed_sample = {50.0, 150.0, 250.0};
    obs.point_id = {0, 0, 1};
    obs.image_id = {0, 1, 0};
    obs.num_observations = 3;

    EXPECT_EQ(obs.num_observations, 3);
    EXPECT_EQ(obs.observed_line.size(), 3);
    EXPECT_EQ(obs.point_id[0], 0);
    EXPECT_EQ(obs.point_id[2], 1);
}

TEST(BundleTest, BundlePointsCreation) {
    BundlePoints points;
    points.x = {1000.0, 2000.0};
    points.y = {500.0, 1500.0};
    points.z = {0.0, 100.0};
    points.point_type = {2, 2};  // Free points
    points.num_points = 2;

    EXPECT_EQ(points.num_points, 2);
    EXPECT_DOUBLE_EQ(points.x[0], 1000.0);
    EXPECT_DOUBLE_EQ(points.z[1], 100.0);
}

TEST(BundleTest, BundlePointTypeFree) {
    BundlePoints points;
    points.point_type = {2};  // 2 = free point

    EXPECT_EQ(points.point_type[0], 2);
}

TEST(BundleTest, BundlePointTypeConstrained) {
    BundlePoints points;
    points.point_type = {3};  // 3 = constrained point

    EXPECT_EQ(points.point_type[0], 3);
}

TEST(BundleTest, BundlePointTypeFixed) {
    BundlePoints points;
    points.point_type = {4};  // 4 = fixed point

    EXPECT_EQ(points.point_type[0], 4);
}

// Test bundle configuration
TEST(BundleTest, BundleConfigDefaults) {
    BundleConfig config;

    EXPECT_EQ(config.max_iterations, 50);
    EXPECT_DOUBLE_EQ(config.function_tolerance, 1e-6);
    EXPECT_DOUBLE_EQ(config.parameter_tolerance, 1e-8);
    EXPECT_TRUE(config.use_sparse_solver);
    EXPECT_EQ(config.num_threads, 1);
}

TEST(BundleTest, BundleConfigCustom) {
    BundleConfig config;
    config.max_iterations = 100;
    config.num_threads = 4;
    config.use_sparse_solver = false;

    EXPECT_EQ(config.max_iterations, 100);
    EXPECT_EQ(config.num_threads, 4);
    EXPECT_FALSE(config.use_sparse_solver);
}

// Note: Full bundle adjustment tests require CSM sensors and real data.
// These are placeholders for the test infrastructure.

#ifdef MINISET_HAS_CERES
TEST(BundleOptimizationTest, PlaceholderForIntegrationTests) {
    GTEST_SKIP() << "Full bundle adjustment tests require CSM sensors and real observation data";
}
#endif

} // namespace bundle::tests
