
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "screensaver.h"


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    qputenv("QT_QUICK_CONTROLS_STYLE", QByteArray("Material"));
//    qputenv("QT_QUICK_CONTROLS_MATERIAL_THEME", QByteArray("Dark"));

    QGuiApplication app(argc, argv);


    QQmlApplicationEngine engine;
    auto ctx = engine.rootContext();

//    ctx->setContextProperty("imageModel", new ImageModel(&app));
//    ctx->setContextProperty("screenSaver", new ScreenSaver(&app));
    qmlRegisterSingletonType<ScreenSaver>("ScreenSaver", 1, 0, "ScreenSaver", &ScreenSaver::qmlInstance);
    const QUrl url(QStringLiteral("qrc:/main.qml"));


    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
