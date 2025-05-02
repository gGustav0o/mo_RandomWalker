#include "ImageData.hpp"
#include "../utils/ImageToEigen.hpp"

void ImageData::update_from(const QImage& image)
{
    matrix_ = image_to_matrix(image);
    is_matrix_valid_ = true;
}

void ImageData::clear() noexcept
{
    matrix_ = Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic>();
    is_matrix_valid_ = false;
}

Eigen::Matrix<uint8_t, Eigen::Dynamic, Eigen::Dynamic> ImageData::matrix() const
{
    return matrix_;
}
