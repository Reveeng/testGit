#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "gstvideoplayer.h"
#include "snapshot.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    GstVideoPlayer::registerQmlType();
    snapshot sst;
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.rootContext()->setContextProperty("snapshot", &sst);
    engine.load(url);
    return app.exec();
}
