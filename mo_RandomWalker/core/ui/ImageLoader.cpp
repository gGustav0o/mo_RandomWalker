#include <QFileInfo>

#include "ImageLoader.hpp"
#include "ImageProvider.hpp"

ImageLoader::ImageLoader(QObject* parent)
    : QObject(parent)
{
}

[[nodiscard]] bool ImageLoader::load_image(const QString& path) noexcept
{
    const QUrl url(path);
    const QString local_path = url.isLocalFile() ? url.toLocalFile() : path;

    const QFileInfo file_info(local_path);
    if (!file_info.exists() || !file_info.isFile())
        return false;

    QImage loaded;
    if (!loaded.load(local_path))
        return false;

    image_ = loaded.convertToFormat(QImage::Format_Grayscale8);

    if (provider_)
        provider_->set_image(image_);

    emit image_changed();
    return true;
}
[[nodiscard]] QImage ImageLoader::image() const noexcept
{
    return image_;
}

void ImageLoader::clear_image() noexcept
{
    image_ = QImage();
    if (provider_)
        provider_->set_image(image_);
    emit image_changed();
}

int ImageLoader::image_width() const noexcept
{
    return image_.width();
}

int ImageLoader::image_height() const noexcept
{
    return image_.height();
}

bool ImageLoader::has_image() const noexcept
{
    return !image_.isNull();
}

void ImageLoader::set_image_provider(ImageProvider* provider) noexcept
{
    provider_ = provider;
}
