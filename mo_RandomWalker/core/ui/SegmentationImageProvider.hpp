#pragma once

#include <QQuickImageProvider>

#include "../scene/SceneManager.hpp"

class SegmentationImageProvider : public QQuickImageProvider
{
public:
    SegmentationImageProvider(SceneManager* manager);

    QImage requestImage(const QString& id, QSize* size, const QSize& requestedSize) override;

private:
    SceneManager* manager_;
};
