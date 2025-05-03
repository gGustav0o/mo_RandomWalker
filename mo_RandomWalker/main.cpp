#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "core/app/AppContext.hpp"
#include "core/app/connect_components.cpp"
#include "core/qml/qml_registration.hpp"

static void setup_high_dpi()
{
#if defined(Q_OS_WIN) && QT_VERSION_CHECK(5, 6, 0) <= QT_VERSION && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
}

int main(int argc, char* argv[])
{
    setup_high_dpi();
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    register_qml_types();

    AppContext ctx;
    ctx.engine = &engine;
    ctx.image_provider = new ImageProvider();
    ctx.scene_manager = new SceneManager();
    ctx.image_loader = new ImageLoader();
	ctx.random_walker_runner = new RandomWalkerRunner(ctx.scene_manager);
	ctx.segmentation_image_provider = new SegmentationImageProvider(ctx.scene_manager);
    ctx.image_loader->set_image_provider(ctx.image_provider);

    ctx.engine->addImageProvider("loader", ctx.image_provider);
	ctx.engine->addImageProvider("segmentation", ctx.segmentation_image_provider);
    connect_components(ctx);
    register_qml_context(ctx);

    ctx.engine->load(QUrl(QStringLiteral("qrc:/qt/qml/mo_randomwalker/qml/RandomWalker.qml")));
    if (ctx.engine->rootObjects().isEmpty())
        return -1;

    return app.exec();
}
