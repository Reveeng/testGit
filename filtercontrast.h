#ifndef FILTERCONTRAST_H
#define FILTERCONTRAST_H

#include <QObject>
#include <QAbstractVideoFilter>
#include <QVideoFilterRunnable>
#include <tfmcontraster.h>

class FilterContrast : public QAbstractVideoFilter
{
    Q_OBJECT
    Q_PROPERTY(TfmContraster* contraster READ contraster NOTIFY contrasterChanged)
public:
    explicit FilterContrast(QObject *parent = nullptr);
    QVideoFilterRunnable* createFilterRunnable();
    TfmContraster *contraster() const;
signals:
    void frameReceived(QVideoFrame frame);
    void frameReday(QVideoFrame frame);
    void contrasterChanged(TfmContraster* contraster);

protected:
    TfmContraster *m_contraster;
};


class FilterContrastRunnable : public QVideoFilterRunnable {
    QVideoFrame *m_testFrame;
    FilterContrast *m_filter;
public:
    FilterContrastRunnable(FilterContrast *filter);
    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags);
};

#endif // FILTERCONTRAST_H
