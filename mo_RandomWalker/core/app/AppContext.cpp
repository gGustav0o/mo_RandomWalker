#include <QQmlContext>

#include "AppContext.hpp"
#include "../qml/qml_names.hpp"

_cold_ void AppContext::connect_components()
{
    QObject::connect(
        image_loader, &ImageLoader::image_changed,
        scene_manager, [this]() {
            scene_manager->on_image_changed(image_loader->image());
        }
    );
}

_cold_ void AppContext::register_qml_context()
{
    engine->rootContext()->setContextProperty(qml_names::kImageLoader, image_loader);
    engine->rootContext()->setContextProperty(qml_names::kSceneManager, scene_manager);
    engine->rootContext()->setContextProperty(qml_names::kRandomWalkerRunner, random_walker_runner);
}

_cold_ void AppContext::initialize(QQmlApplicationEngine* engine_)
{
    engine = engine_;

    image_provider = new ImageProvider();
    scene_manager = new SceneManager();
    image_loader = new ImageLoader();
    random_walker_runner = new RandomWalkerRunner(scene_manager);
    segmentation_image_provider = new SegmentationImageProvider(scene_manager);

    image_loader->set_image_provider(image_provider);

    engine->addImageProvider(qml_names::kProviderLoader, image_provider);
    engine->addImageProvider(qml_names::kProviderSegmentation, segmentation_image_provider);

    connect_components();
    register_qml_context();
}
