#include "bundle/ceres_optimizer.hpp"
#include "core/math.hpp"
#include <Eigen/Dense>

namespace bundle {

std::pair<Vec3, std::vector<double>> closestApproach(
    const std::vector<Vec3>& points,
    const std::vector<Vec3>& directions
) {
    size_t n = points.size();
    if (n < 2) {
        if (n == 1) {
            return {points[0], std::vector<double>(9, 0.0)};
        }
        return {Vec3{0, 0, 0}, std::vector<double>(9, 0.0)};
    }

    // Use least squares to find the point that minimizes distance to all rays
    // For each ray: p_i + t_i * d_i
    // We want to find point X that minimizes sum of squared distances

    Eigen::MatrixXd A(3 * n, 3 + n);
    Eigen::VectorXd b(3 * n);
    A.setZero();
    b.setZero();

    for (size_t i = 0; i < n; ++i) {
        // For each ray, we have: (X - p_i) - t_i * d_i = 0
        // Rearranged: X - p_i = t_i * d_i
        // Or: [I | -d_i] * [X; t_i] = p_i

        A.block<3, 3>(3 * i, 0) = Eigen::Matrix3d::Identity();
        A(3 * i + 0, 3 + i) = -directions[i].x;
        A(3 * i + 1, 3 + i) = -directions[i].y;
        A(3 * i + 2, 3 + i) = -directions[i].z;

        b(3 * i + 0) = points[i].x;
        b(3 * i + 1) = points[i].y;
        b(3 * i + 2) = points[i].z;
    }

    // Solve using least squares
    Eigen::VectorXd solution = A.colPivHouseholderQr().solve(b);

    Vec3 result{solution(0), solution(1), solution(2)};

    // Compute covariance (simplified - just use identity scaled by residuals)
    Eigen::VectorXd residuals = A * solution - b;
    double variance = residuals.squaredNorm() / (3 * n - 3 - n);

    // Return 3x3 covariance as 9-element vector (row-major)
    std::vector<double> covar(9);
    for (int i = 0; i < 9; ++i) {
        covar[i] = (i % 4 == 0) ? variance : 0.0;  // Diagonal elements only
    }

    return {result, covar};
}

#ifdef MINISET_HAS_CERES
BundleResult runBundleAdjustment(BundleObservations& observations,
                                 BundlePoints& points,
                                 BundleImageParams& image_params,
                                 const BundleConfig& config) {
    // Placeholder for Ceres implementation
    BundleResult result;
    result.num_iterations = 0;
    result.final_cost = 0.0;
    result.sigma0 = 0.0;
    result.converged = false;
    return result;
}
#endif

} // namespace bundle
