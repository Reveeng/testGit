#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "gstvideoplayer.h"
#include "snapshot.h"
#include "filterdeconv.h"
#include "filtercontrast.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    GstVideoPlayer::registerQmlType();
    snapshot sst(app.applicationDirPath());
    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.rootContext()->setContextProperty("snapshot", &sst);
    engine.load(url);
    qmlRegisterType<FilterDeconv>("infratest.filters", 1, 0, "FilterDeconv");
    qmlRegisterType<FilterContrast>("infratest.filters", 1, 0, "FilterContrast");
    qmlRegisterType<TfmContraster>("infratest.filters", 1, 0, "Contraster");
    return app.exec();
}
