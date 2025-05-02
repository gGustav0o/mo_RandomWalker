#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "core/app/AppContext.hpp"

static void connect_components(const AppContext& ctx)
{
    QObject::connect(
        ctx.image_loader, &ImageLoader::image_changed,
        ctx.scene_manager, [ctx]() {
            ctx.scene_manager->on_image_changed(ctx.image_loader->image());
        }
    );

    QObject::connect(
        ctx.scene_manager, &SceneManager::segmentation_ready,
        ctx.image_provider, [ctx](const QImage& result) {
            ctx.image_provider->set_image(result);
        }
    );
}

static void register_qml_context(const AppContext& ctx)
{
    ctx.engine->rootContext()->setContextProperty("ImageLoader", ctx.image_loader);
    ctx.engine->rootContext()->setContextProperty("SceneManager", ctx.scene_manager);
}
