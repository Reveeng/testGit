#ifndef GSTVIDEOPLAYER_H
#define GSTVIDEOPLAYER_H

#include <gst/gst.h>
#include <QObject>
#include <QAbstractVideoSurface>
#include <QVideoSurfaceFormat>


class GstVideoPlayer : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QAbstractVideoSurface * videoSurface READ videoSurface WRITE setVideoSurface)
    Q_PROPERTY(QString source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QString err READ err WRITE setErr NOTIFY errChanged)


public:
    explicit GstVideoPlayer( QObject * parent = 0);
    ~GstVideoPlayer();

    static void registerQmlType();

    QAbstractVideoSurface * videoSurface() const;

    QString source() const;

    QString err() const;

    int pullAppsinkFrame();
public slots:
    void setVideoSurface(QAbstractVideoSurface * surface);

    void setSource(QString url);

    void start();

    void stop();

    void updateFrame(QVideoFrame frame);

    void setErr(QString err);

signals:
    void sourceChanged(QString url);
    void started();
    void stopped();
    void newFrame(QVideoFrame frame);
    void errChanged(QString err);

private:
    void closeSurface();

private:
    QAbstractVideoSurface * m_videoSurface;
    QVideoSurfaceFormat  m_format;
    QString m_source;

    QString m_err;

protected:
    GstElement * m_pipeline;
    GstElement * m_appsink;
    QString m_launchstring;
    QString m_sinkName;
};


#endif // GSTVIDEOPLAYER_H
