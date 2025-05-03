#pragma once

#include <QQmlApplicationEngine>

#include "core/ui/ImageLoader.hpp"
#include "core/ui/ImageProvider.hpp"
#include "core/ui/SegmentationImageProvider.hpp"
#include "core/scene/SceneManager.hpp"
#include "core/interface/RandomWalkerRunner.hpp"

struct AppContext
{
    ImageLoader* image_loader = nullptr;
    SceneManager* scene_manager = nullptr;
    ImageProvider* image_provider = nullptr;
	SegmentationImageProvider* segmentation_image_provider = nullptr;
	RandomWalkerRunner* random_walker_runner = nullptr;
    QQmlApplicationEngine* engine = nullptr;
};
