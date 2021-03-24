#include "filtercontrast.h"

FilterContrast::FilterContrast(QObject *parent) : QAbstractVideoFilter(parent),
    m_contraster(new TfmContraster(this))
{

}

QVideoFilterRunnable *FilterContrast::createFilterRunnable()
{
    return new FilterContrastRunnable(this);
}

TfmContraster *FilterContrast::contraster() const
{
    return m_contraster;
}

FilterContrastRunnable::FilterContrastRunnable(FilterContrast *filter) : QVideoFilterRunnable(),
    m_filter(filter)
{
    int16_t *ptr;
    int16_t pixVal;
    int width = 640;
    int height = 480;
    int bytesPerLine = width * 2;
    int size = bytesPerLine * height;
    m_testFrame = new QVideoFrame(size, QSize(width, height), bytesPerLine, QVideoFrame::Format_UYVY);
    m_testFrame->map(QAbstractVideoBuffer::ReadWrite);

    pixVal = 32000;
    ptr = (int16_t *)m_testFrame->bits();
    for (int y=0; y < height; y++) {
        for (int x=0; x < width; x++)
            *(ptr+x) = pixVal;
        ptr += width;
        pixVal -= 100;
    }

    m_testFrame->unmap();
}

QVideoFrame FilterContrastRunnable::run(QVideoFrame *input,
                                        const QVideoSurfaceFormat &surfaceFormat,
                                        QVideoFilterRunnable::RunFlags flags)
{
//    input = m_testFrame;
    emit m_filter->frameReceived(*input);

    if (!input->map(QAbstractVideoBuffer::ReadOnly)) {
        qWarning("Failed to map buffer");
        return *input;
    }

    QVideoFrame output(input->mappedBytes(),
                      QSize(input->width(), input->height()),
                      input->bytesPerLine(),
                      QVideoFrame::Format_UYVY);

    if (!output.map(QAbstractVideoBuffer::ReadWrite)) {
        qWarning("Failed to map buffer");
        input->unmap();
        return *input;
    }

    m_filter->contraster()->setWidth(input->width());
    m_filter->contraster()->setHeight(input->height());
    m_filter->contraster()->setBufIn((int16_t*)input->bits());
    m_filter->contraster()->setBufOut((uint16_t*)output.bits());
    m_filter->contraster()->process();

    input->unmap();
    output.unmap();
    emit m_filter->frameReday(output);
    return output;
}
