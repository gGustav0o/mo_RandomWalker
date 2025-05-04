#pragma once

#include <QImage>
#include <Eigen/Core>

class ImageData
{
public:
    ImageData() = default;

    void update_from(const QImage& image);
    void clear() noexcept;

    [[nodiscard]] bool has_matrix() const noexcept;
    [[nodiscard]] std::optional<Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic>> maybe_matrix() const noexcept;
private:
    std::optional<Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic>> matrix_;
};