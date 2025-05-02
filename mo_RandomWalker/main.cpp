#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "core/ImageLoader.hpp"
#include "core/ImageProvider.hpp"

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN) && QT_VERSION_CHECK(5, 6, 0) <= QT_VERSION && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    auto* provider = new ImageProvider();
    engine.addImageProvider("loader", provider);

    ImageLoader loader;
    loader.set_image_provider(provider);

    engine.rootContext()->setContextProperty("ImageLoader", &loader);

    engine.load(QUrl(QStringLiteral("qrc:/qt/qml/mo_randomwalker/qml/RandomWalker.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
