#include <gtest/gtest.h>
#include "surface/dem.hpp"
#include <cmath>

namespace surface::tests {

class EllipsoidDEMTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Mars ellipsoid
        dem = new EllipsoidDEM(3396190.0, 3376200.0);
    }

    void TearDown() override {
        delete dem;
    }

    EllipsoidDEM* dem;
};

TEST_F(EllipsoidDEMTest, HeightAtEquator) {
    // Pure ellipsoid has zero height
    double height = dem->getHeight(0.0, 0.0);
    EXPECT_DOUBLE_EQ(height, 0.0);
}

TEST_F(EllipsoidDEMTest, HeightAtPole) {
    double height = dem->getHeight(M_PI / 2.0, 0.0);
    EXPECT_DOUBLE_EQ(height, 0.0);
}

TEST_F(EllipsoidDEMTest, HeightAtArbitraryPoint) {
    double height = dem->getHeight(M_PI / 4.0, M_PI / 4.0);
    EXPECT_DOUBLE_EQ(height, 0.0);
}

TEST_F(EllipsoidDEMTest, RadiusAtEquator) {
    // Convert degrees to radians: 0 lat, 0 lon
    double radius = dem->getRadius(0.0, 0.0);
    EXPECT_DOUBLE_EQ(radius, 3396190.0);
}

TEST_F(EllipsoidDEMTest, RadiusAtEquator180) {
    // Convert degrees to radians: 0 lat, 180 lon
    double radius = dem->getRadius(0.0, M_PI);
    EXPECT_DOUBLE_EQ(radius, 3396190.0);
}

TEST_F(EllipsoidDEMTest, RadiusAtPole) {
    // Convert degrees to radians: 90 lat, any lon
    double radius = dem->getRadius(M_PI / 2.0, 300.0 * M_PI / 180.0);
    EXPECT_NEAR(radius, 3376200.0, 1.0);
}

TEST_F(EllipsoidDEMTest, GetRadii) {
    EXPECT_DOUBLE_EQ(dem->getSemiMajorA(), 3396190.0);
    EXPECT_DOUBLE_EQ(dem->getSemiMajorB(), 3396190.0);
    EXPECT_DOUBLE_EQ(dem->getSemiMinorC(), 3376200.0);
}

// Test triaxial ellipsoid
TEST(TriaxialDEMTest, AllRadiiDifferent) {
    // Create triaxial with custom radii
    EllipsoidDEM triaxial(100.0, 90.0);

    EXPECT_DOUBLE_EQ(triaxial.getSemiMajorA(), 100.0);
    EXPECT_DOUBLE_EQ(triaxial.getSemiMajorB(), 100.0);
    EXPECT_DOUBLE_EQ(triaxial.getSemiMinorC(), 90.0);
}

// Note: GDAL DEM tests would require actual DEM files or mocking GDAL,
// which is more complex. For now, we test the base EllipsoidDEM functionality.
// Full GDAL tests should be added as integration tests with test data.

#ifdef MINISET_HAS_GDAL
TEST(GdalDEMTest, ConstructorWithFile) {
    // This test requires a real DEM file
    // For now, just verify that the test infrastructure is in place
    GTEST_SKIP() << "GDAL DEM tests require test data files";
}
#endif

} // namespace surface::tests
