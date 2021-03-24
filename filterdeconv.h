#ifndef FILTERDECONV_H
#define FILTERDECONV_H

#include <QObject>
#include <QAbstractVideoFilter>
#include <QVideoFilterRunnable>
#include "deconv.h"


class FilterDeconv : public QAbstractVideoFilter
{
    Q_OBJECT
public:
    explicit FilterDeconv(QObject *parent = nullptr);
    QVideoFilterRunnable* createFilterRunnable();
    Deconv* deconv();

    void copyToDeconv(int width, int height, int16_t *buf);
    void copyFromDeconv(int width, int height, int16_t *buf);

signals:
    void frameReceived(QVideoFrame frame);
    void frameReady(QVideoFrame frame);

protected:
    Deconv *m_deconv;
};

class FilterDeconvRunnable : public QVideoFilterRunnable {
    FilterDeconv *m_filter;
public:
    FilterDeconvRunnable(FilterDeconv *filter);
    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags);
};


#endif // FILTERDECONV_H
