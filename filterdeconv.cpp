#include "filterdeconv.h"

FilterDeconv::FilterDeconv(QObject *parent) : QAbstractVideoFilter(parent),
    m_deconv(new Deconv(1024, 0, this))
{

}

QVideoFilterRunnable *FilterDeconv::createFilterRunnable()
{
    return new FilterDeconvRunnable(this);
}

Deconv *FilterDeconv::deconv()
{
    return m_deconv;
}


void FilterDeconv::copyToDeconv(int width, int height, int16_t *buf)
{
    int16_t offset = m_deconv->fftDataOffset();
    int16_t *pData = buf;
    float    *pFft = m_deconv->fftInputBuffer();
    pFft +=  offset + (m_deconv->fftSize()*offset);

    for (int y=0; y < height; y++) {
        for (int x=0; x < width; x++) {
            pFft[x] = pData[x] * 1.0;
        }
        pData += width;
        pFft += m_deconv->fftSize();
    }
}

void FilterDeconv::copyFromDeconv(int width, int height, int16_t *buf)
{
    float coef = m_deconv->fftSize() * m_deconv->fftSize();
    int16_t *pData = buf;
    float    *pFft = m_deconv->fftOutputBuffer();

    for (int y=0; y < height; y++) {
        for (int x=0; x < width; x++) {
            pData[x] = (int16_t)( pFft[x] / coef );
        }
        pData += width;
        pFft += m_deconv->fftSize();
    }
}

FilterDeconvRunnable::FilterDeconvRunnable(FilterDeconv *filter) : QVideoFilterRunnable(),
    m_filter(filter)
{

}

QVideoFrame FilterDeconvRunnable::run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, QVideoFilterRunnable::RunFlags flags)
{
    emit m_filter->frameReceived(*input);

    if (!input->map(QAbstractVideoBuffer::ReadOnly)) {
        qWarning("Failed to map input");
        return *input;
    }

    QVideoFrame output(input->mappedBytes(),
                      QSize(input->width(), input->height()),
                      input->bytesPerLine(),
                      QVideoFrame::Format_UYVY);

    if (!output.map(QAbstractVideoBuffer::ReadWrite)) {
        qWarning("Failed to map output");
        input->unmap();
        return *input;
    }

    m_filter->copyToDeconv(input->width(), input->height(), (int16_t*)input->bits());
    m_filter->deconv()->calculate();
    m_filter->copyFromDeconv(output.width(), output.height(), (int16_t*)output.bits());


    input->unmap();
    output.unmap();
    emit m_filter->frameReady(output);
    return output;
}
