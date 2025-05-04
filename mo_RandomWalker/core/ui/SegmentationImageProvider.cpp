#include "SegmentationImageProvider.hpp"

SegmentationImageProvider::SegmentationImageProvider(SceneManager* manager)
    : QQuickImageProvider(QQuickImageProvider::Image), manager_(manager) {}

QImage SegmentationImageProvider::requestImage(const QString& id, QSize* size, const QSize& requestedSize)
{
    const QImage result = manager_->segmentation_result();

    if (result.isNull()) {
        qDebug() << "[SegmentationImageProvider] Empty result! Returning transparent dummy.";
        const QImage& dummy = get_dummy_image();
        if (size) *size = dummy.size();
        return dummy;
    }

    if (size)
        *size = result.size();

    qDebug() << "[SegmentationImageProvider] Returning image of size:" << result.size();
    return result;
}

[[nodiscard]] static const QImage& get_dummy_image()
{
    static const QImage dummy = [] {
        QImage img(64, 64, QImage::Format_ARGB32);
        img.fill(Qt::transparent);
        return img;
        }();
    return dummy;
}
