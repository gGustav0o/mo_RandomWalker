#pragma once

#include <QImage>
#include <Eigen/Core>

class ImageData
{
public:
    ImageData() = default;

    void update_from(const QImage& image);
    void clear() noexcept;

    [[nodiscard]] Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic> matrix() const;

private:
    Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic> matrix_;
    bool is_matrix_valid_ = false;
};