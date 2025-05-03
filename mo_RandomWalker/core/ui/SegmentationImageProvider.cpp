#include "SegmentationImageProvider.hpp"

SegmentationImageProvider::SegmentationImageProvider(SceneManager* manager)
    : QQuickImageProvider(QQuickImageProvider::Image), manager_(manager) {}

QImage SegmentationImageProvider::requestImage(const QString& id, QSize* size, const QSize& requestedSize)
{
    const QImage result = manager_->segmentation_result();

    if (result.isNull()) {
        qDebug() << "[SegmentationImageProvider] Empty result! Returning transparent dummy.";
        QImage dummy(64, 64, QImage::Format_ARGB32);
        dummy.fill(Qt::transparent);
        if (size) *size = dummy.size();
        return dummy;
    }

    if (size)
        *size = result.size();

    qDebug() << "[SegmentationImageProvider] Returning image of size:" << result.size();
    return result;
}
