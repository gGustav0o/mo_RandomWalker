#include "ImageData.hpp"
#include "../utils/ImageToEigen.hpp"

void ImageData::update_from(const QImage& image)
{
    matrix_ = image_to_matrix(image);
}

void ImageData::clear() noexcept
{
    matrix_ = std::nullopt;
}

bool ImageData::has_matrix() const noexcept
{
    return matrix_.has_value();
}

std::optional<Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic>> ImageData::maybe_matrix() const noexcept
{
    return matrix_;
}
