#include <gtest/gtest.h>
#include "surface/ellipsoid.hpp"
#include "core/math.hpp"
#include <cmath>

namespace surface::tests {

class EllipsoidTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Sphere with radius 10
        sphere = new Ellipsoid(10.0, 10.0);
    }

    void TearDown() override {
        delete sphere;
    }

    Ellipsoid* sphere;
};

TEST_F(EllipsoidTest, GetSurfaceNormal) {
    Vec3 ground_pt{1.0, 0.0, 0.0};
    Vec3 normal = sphere->getSurfaceNormal(ground_pt);

    EXPECT_NEAR(normal.x, 1.0, 1e-12);
    EXPECT_NEAR(normal.y, 0.0, 1e-12);
    EXPECT_NEAR(normal.z, 0.0, 1e-12);
}

TEST_F(EllipsoidTest, IntersectSurface) {
    Vec3 sensor_pos{100.0, 0.0, 0.0};
    Vec3 look_vec{-1.0, 0.0, 0.0};

    Vec3 intersection = sphere->intersectSurface(sensor_pos, look_vec);

    EXPECT_NEAR(intersection.x, 10.0, 1e-10);
    EXPECT_NEAR(intersection.y, 0.0, 1e-10);
    EXPECT_NEAR(intersection.z, 0.0, 1e-10);
}

TEST_F(EllipsoidTest, IntersectSurfaceAngled) {
    // Test intersection from above - ray pointing down in +x direction
    Vec3 sensor_pos{5.0, 0.0, 20.0};
    Vec3 look_vec{0.0, 0.0, -1.0};  // Straight down

    Vec3 intersection = sphere->intersectSurface(sensor_pos, look_vec);

    // Should intersect at approximately (5, 0, sqrt(75))
    double radius = math::magnitude(intersection);
    EXPECT_NEAR(radius, 10.0, 1e-9);
}

TEST_F(EllipsoidTest, GetRadiusAtEquator) {
    // For a sphere, radius should be constant
    double radius = sphere->getRadiusAt(0.0, 0.0);
    EXPECT_DOUBLE_EQ(radius, 10.0);
}

TEST_F(EllipsoidTest, GetRadiusAtPole) {
    double radius = sphere->getRadiusAt(M_PI / 2.0, 0.0);
    EXPECT_DOUBLE_EQ(radius, 10.0);
}

// Test with oblate ellipsoid (Mars-like)
TEST(EllipsoidOblatTest, RadiusAtEquator) {
    Ellipsoid mars(3396190.0, 3376200.0);

    double radius_eq = mars.getRadiusAt(0.0, 0.0);
    EXPECT_NEAR(radius_eq, 3396190.0, 1.0);
}

TEST(EllipsoidOblatTest, RadiusAtPole) {
    Ellipsoid mars(3396190.0, 3376200.0);

    double radius_pole = mars.getRadiusAt(M_PI / 2.0, 0.0);
    EXPECT_NEAR(radius_pole, 3376200.0, 1.0);
}

TEST(EllipsoidOblatTest, IntersectOblate) {
    Ellipsoid mars(3396190.0, 3376200.0);

    // Ray from above equator
    Vec3 sensor_pos{0.0, 0.0, 4000000.0};
    Vec3 look_vec{0.0, 0.0, -1.0};

    Vec3 intersection = mars.intersectSurface(sensor_pos, look_vec);

    // Should hit the pole
    EXPECT_NEAR(intersection.x, 0.0, 1e-6);
    EXPECT_NEAR(intersection.y, 0.0, 1e-6);
    EXPECT_NEAR(intersection.z, 3376200.0, 1.0);
}

} // namespace surface::tests
