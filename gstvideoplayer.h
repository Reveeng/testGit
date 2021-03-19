#ifndef GSTVIDEOPLAYER_H
#define GSTVIDEOPLAYER_H

#include <gst/gst.h>
#include <QObject>
#include <QAbstractVideoSurface>
#include <QVideoSurfaceFormat>
#include "../deconv-fft-lib/deconv.h"
#include "../tfm-contraster-lib/tfmcontraster.h"
#include "../tfm-temperature-lib/tfmtemperature.h"

class GstVideoPlayer : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int fps READ fps WRITE setFps NOTIFY fpsChanged)
    Q_PROPERTY(QAbstractVideoSurface * videoSurface READ videoSurface WRITE setVideoSurface)
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QString err READ err WRITE setErr NOTIFY errChanged)
    Q_PROPERTY(TfmTemperature *tfmtemperature READ tfmtemperature)


public:
    explicit GstVideoPlayer( QObject * parent = 0);
    ~GstVideoPlayer();

    static void registerQmlType();

    int fps() const;

    QAbstractVideoSurface * videoSurface() const;

    QString source() const;

    QString err() const;

    TfmTemperature * tfmtemperature() const;

    int pullAppsinkFrame();

    TfmRefpoint m_pointCool;

    TfmRefpoint m_pointHot;

    QRect roi;
public slots:
    void setFps(int fps);

    void setVideoSurface(QAbstractVideoSurface * surface);

    void setSource(QString url);

    void start();

    void stop();

    void updateFrame(QVideoFrame frame);

    void setErr(QString err);

    void setRefPoints(int x, int y, float t, bool isCool);

    void setRoiToStatistic(int x, int y, int height, int width);

    void onDeconvReady();

protected slots:
    void setLastTimestamp(ulong timestamp);

signals:
    void sourceChanged(QString url);
    void started();
    void stopped();
    void newFrame(QVideoFrame frame);
    void errChanged(QString err);
    void fpsChanged(int fps);
    void maxTempInRoiChanged(float max);
    void everythingOkay();

private slots:
    void closeSurface();
    void copyToDeconv(int16_t * buf, int width, int height);
    void copyFromDeconv(int16_t * buf, int width, int heigt);

private:
    QAbstractVideoSurface * m_videoSurface;
    QVideoSurfaceFormat  m_format;
    QString m_source;
    Deconv * m_deconv;
    TfmContraster * contraster;
    TfmTemperature * m_tfmtemperature;
    QString m_err;

protected:
    GstElement * m_pipeline;
    GstElement * m_appsink;
    QString m_launchstring;
    QString m_sinkName;
    int m_fps;
    ulong m_lastTimestamp;
};


#endif // GSTVIDEOPLAYER_H
