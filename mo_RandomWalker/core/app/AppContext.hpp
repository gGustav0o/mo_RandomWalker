#pragma once

#include <QQmlApplicationEngine>

#include "core/ui/ImageLoader.hpp"
#include "core/ui/ImageProvider.hpp"
#include "core/scene/SceneManager.hpp"

struct AppContext
{
    ImageLoader* image_loader = nullptr;
    SceneManager* scene_manager = nullptr;
    ImageProvider* image_provider = nullptr;
    QQmlApplicationEngine* engine = nullptr;
};
