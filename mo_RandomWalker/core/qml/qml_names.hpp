#pragma once

#include <QString>

namespace qml_names
{
#define QML_LITERAL(name, str) inline const auto name = QStringLiteral(str)

    QML_LITERAL(kImageLoader, "ImageLoader");
    QML_LITERAL(kSceneManager, "SceneManager");
    QML_LITERAL(kRandomWalkerRunner, "randomWalkerRunner");
    QML_LITERAL(kProviderLoader, "loader");
    QML_LITERAL(kProviderSegmentation, "segmentation");
}