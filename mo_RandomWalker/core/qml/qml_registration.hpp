#pragma once

#include <QQmlEngine>
#include <qqml.h>
#include <core/enums/QmlEnums.hpp>

#include "../enums/SeedLabel.hpp" 

inline void register_qml_types()
{
    qmlRegisterUncreatableType<QmlEnums>("App.Enums", 1, 0, "SeedLabel", "Enum only");
}