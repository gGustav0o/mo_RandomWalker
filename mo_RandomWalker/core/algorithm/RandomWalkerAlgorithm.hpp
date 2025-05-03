#pragma once

#include <Eigen/Core>
#include <Eigen/Sparse>

#include <QPoint>
#include <vector>

namespace algorithm
{

    class RandomWalkerAlgorithm
    {
    public:
        RandomWalkerAlgorithm(
            const Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic>& image,
            const std::vector<QPoint>& background_seeds,
            const std::vector<QPoint>& object_seeds);

        [[nodiscard]] Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic> run() const;
        [[nodiscard]] Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> run_probabilities() const;

    private:
        Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic> image_;
        std::vector<QPoint> background_seeds_;
        std::vector<QPoint> object_seeds_;
    };

} // namespace segmentation
