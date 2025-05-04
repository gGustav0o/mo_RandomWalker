#pragma once

#include <QQmlEngine>
#include <qqml.h>
#include <core/enums/QmlEnums.hpp>

#include "../enums/SeedLabel.hpp" 

_cold_ static inline int register_qml_types()
{
    return qmlRegisterUncreatableType<QmlEnums>("App.Enums", 1, 0, "SeedLabel", "Enum only");
}