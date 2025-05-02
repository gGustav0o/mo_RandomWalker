#include "ImageProvider.hpp"

ImageProvider::ImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Image)
    , image_(QImage(1, 1, QImage::Format_Grayscale8))
{
    image_.fill(Qt::black);
}

void ImageProvider::set_image(const QImage& image)
{
    QMutexLocker locker(&mutex_);
    image_ = image;
}

[[nodiscard]] QImage ImageProvider::requestImage(const QString&, QSize* size, const QSize& requestedSize)
{
    QMutexLocker locker(&mutex_);
    if (size)
        *size = image_.size();
    return requestedSize.isValid()
        ? image_.scaled(requestedSize, Qt::KeepAspectRatio, Qt::SmoothTransformation)
        : image_;
}
