#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "gstvideoplayer.h"
#include "snapshot.h"
#include "filterdeconv.h"
#include "filtercontrast.h"
#include "filtertemperature.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    GstVideoPlayer::registerQmlType();
    qmlRegisterType<FilterDeconv>("infratest.filters", 1, 0, "FilterDeconv");
    qmlRegisterType<FilterTemperature>("infratest.filters", 1, 0, "FilterTemperature");
    qmlRegisterType<FilterContrast>("infratest.filters", 1, 0, "FilterContrast");
    qmlRegisterType<TfmContraster>("infratest.filters", 1, 0, "Contraster");
    qmlRegisterType<TfmTemperature>("infratest.filters", 1, 0, "Temperature");
    qmlRegisterType<TfmRefpoint>("infratest.filters", 1, 0, "Refpoint");
    qmlRegisterType<TfmTemperatureStatistics>("infratest.filters", 1, 0, "TemperatureStatistics");
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

    return app.exec();
}
