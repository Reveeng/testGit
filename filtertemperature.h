#ifndef FILTERTEMPERATURE_H
#define FILTERTEMPERATURE_H

#include <QObject>
#include <QAbstractVideoFilter>
#include <QVideoFilterRunnable>
#include <tfmtemperature.h>

class FilterTemperature : public QAbstractVideoFilter
{
    Q_OBJECT
    Q_PROPERTY(TfmTemperature* temperature READ temperature NOTIFY temperatureChanged)
    TfmTemperature* m_temperature;

public:
    explicit FilterTemperature(QObject *parent = nullptr);
    QVideoFilterRunnable* createFilterRunnable();
    TfmTemperature* temperature() const;

signals:
    void temperatureChanged(TfmTemperature* temperature);
};

class FilterTemperatureRunnable : public QVideoFilterRunnable {
    FilterTemperature *m_filter;
public:
    FilterTemperatureRunnable(FilterTemperature *filter);
    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags);
};

#endif // FILTERTEMPERATURE_H
