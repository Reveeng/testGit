#include "filtertemperature.h"

FilterTemperature::FilterTemperature(QObject *parent) : QAbstractVideoFilter(parent),
    m_temperature(new TfmTemperature(this))
{

}

QVideoFilterRunnable *FilterTemperature::createFilterRunnable()
{
    return new FilterTemperatureRunnable(this);
}

TfmTemperature *FilterTemperature::temperature() const
{
    return m_temperature;
}

FilterTemperatureRunnable::FilterTemperatureRunnable(FilterTemperature *filter) : QVideoFilterRunnable(),
    m_filter(filter)
{

}

QVideoFrame FilterTemperatureRunnable::run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, QVideoFilterRunnable::RunFlags flags)
{
    if (!input->map(QAbstractVideoBuffer::ReadOnly)) {
        qWarning("Failed to map buffer");
        return *input;
    }

    m_filter->temperature()->setWidth(input->width());
    m_filter->temperature()->setHeight(input->height());
    m_filter->temperature()->calcFrame((int16_t*)input->bits());


    input->unmap();
    return *input;
}
