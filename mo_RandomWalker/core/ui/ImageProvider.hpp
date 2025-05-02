#pragma once

#include <QImage>
#include <QQuickImageProvider>
#include <QMutex>

#include "../../MetaAnnotations.hpp"

class ImageProvider : public QQuickImageProvider
{
    _no_copy_move_(ImageProvider)
public:
    explicit ImageProvider();

    void set_image(_In_ const QImage& image);
    [[nodiscard]] QImage requestImage(const QString& id, QSize* size, const QSize& requestedSize) override;

private:
    QImage image_;
    QMutex mutex_;
};
