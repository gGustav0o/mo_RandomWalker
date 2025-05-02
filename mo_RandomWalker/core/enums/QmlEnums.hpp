#pragma once

#include <QObject>
#include <QtQml/qqmlregistration.h>

#include "SeedLabel.hpp"

class QmlEnums : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    enum SeedLabel_ {
        Background = static_cast<int>(SeedLabel::Background),
        Object = static_cast<int>(SeedLabel::Object)
    };
    Q_ENUM(SeedLabel_)
};
