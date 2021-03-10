#include "gstvideoplayer.h"
#include <QtQml/qqml.h>
#include <QDebug>

static GstFlowReturn OnAppsinkEOS(GstElement *sink, GstVideoPlayer *videoplayer)
{
    Q_UNUSED(sink)
    videoplayer->setErr("EOS from appsink - stop pipeline");
    videoplayer->stop();
    return GST_FLOW_ERROR;
}

static GstFlowReturn OnAppsinkNewFrame(GstElement *sink, GstVideoPlayer *videoplayer)
{
    Q_UNUSED(sink)
    return (GstFlowReturn)videoplayer->pullAppsinkFrame();
}

GstVideoPlayer::GstVideoPlayer(QObject * parent) : QObject(parent),
    m_videoSurface(0),
    m_source(""),
    m_tfmtemperature(new TfmTemperature(this)),
    m_pipeline(nullptr),
    m_appsink(nullptr),
    m_sinkName("sink0"),
    m_fps(0),
    m_lastTimestamp(0)
{
    gst_init(NULL, NULL);
    m_deconv = new Deconv(1024,0,this);
    contraster = new TfmContraster(this);
    connect(this, &GstVideoPlayer::newFrame, this, &GstVideoPlayer::updateFrame);
    connect(m_tfmtemperature->stat(), SIGNAL(maxChanged(float)), this, SIGNAL(maxTempInRoiChanged(float)));
}
GstVideoPlayer::~GstVideoPlayer(){
    closeSurface();
}

void GstVideoPlayer::registerQmlType()
{
    qmlRegisterType<GstVideoPlayer>(
            "GstVideoPlayer", 0, 1,
            "GstVideoPlayer" );
    qmlRegisterType<TfmTemperature>("TfmTemperature",0,1,"TfmTemperature");
    qmlRegisterType<TfmTemperatureStatistics>("TfmTemperatureStatistics",0,1,"TfmTemperatureStatistics");
    qmlRegisterType<TfmRefpoint>("RefPoint", 1, 0, "RefPoint");
}

TfmTemperature* GstVideoPlayer::tfmtemperature() const{
    return m_tfmtemperature;
}

void GstVideoPlayer::setVideoSurface (QAbstractVideoSurface *surface){
    if (m_videoSurface)
        closeSurface();
    m_videoSurface = surface;
}

void GstVideoPlayer::closeSurface(){
    if (m_videoSurface && m_videoSurface->isActive())
        m_videoSurface->stop();
}

QAbstractVideoSurface* GstVideoPlayer::videoSurface() const{
    return m_videoSurface;
}

int GstVideoPlayer::fps() const{
    return m_fps;
}

void GstVideoPlayer::setFps(int fps){
    if (fps == m_fps)
        return;
    m_fps = fps;
    emit fpsChanged(m_fps);
}

QString GstVideoPlayer::source() const{
    return m_source;
}

void GstVideoPlayer::setSource(QString source){
    if  (m_source == source)
        return;
    else{
        m_source = source;
        emit sourceChanged(m_source);
    }
}

QString GstVideoPlayer::err() const{
    return m_err;
}

void GstVideoPlayer::setErr(QString err){
    if (m_err == err)
        return;
    else{
        m_err = err;
        emit errChanged(m_err);
    }
}

void GstVideoPlayer::start(){
    if (m_source == "")
    {
        setErr("Source cant be empty");
        return;
    }
    m_pipeline = gst_parse_launch(qPrintable(m_source),NULL);

    if (!m_pipeline){
        setErr("Failed to start pipeline");
        return;
    }

    m_appsink = gst_bin_get_by_name(GST_BIN(m_pipeline), qPrintable(m_sinkName));


    if (!m_appsink){
        setErr("Failed to get appsrc");
        return;
    }

    GstStateChangeReturn res = gst_element_set_state(GST_ELEMENT(m_pipeline), GST_STATE_PLAYING);
    g_signal_connect (m_appsink, "new-sample", G_CALLBACK (OnAppsinkNewFrame), this);
    g_signal_connect (m_appsink, "eos", G_CALLBACK (OnAppsinkEOS), this);
    if (res == GST_STATE_CHANGE_FAILURE)
    {
        setErr("Unable to set the pipeline to the playing state");
        gst_object_unref(m_pipeline);
        return;
    }
    emit started();

}

void GstVideoPlayer::stop(){
    gst_element_set_state(GST_ELEMENT(m_pipeline), GST_STATE_NULL);

    if (m_appsink) {
        gst_object_unref(m_appsink);
        m_appsink = nullptr;
    }

    if(m_pipeline) {
        gst_object_unref(m_pipeline);
        m_pipeline = nullptr;
    }

    emit stopped();
}

int GstVideoPlayer::pullAppsinkFrame(){
    GstSample *sample;
    GstBuffer *buf;
    GstMapInfo info;
    QSize size;
    bool ok;
    QVideoFrame frame;

//    TfmContraster contraster;
    /* Retrieve the buffer */
    g_signal_emit_by_name (m_appsink, "pull-sample", &sample);
    if (! sample) {
        setErr("Cant get frame from sample");
        return GST_FLOW_ERROR;
    }
    GstCaps * caps = gst_sample_get_caps(sample);
    gchar *StringCaps = gst_caps_to_string(caps);
    QString QstringCaps = QString(StringCaps);
    QStringList  QCaps = QstringCaps.split(QLatin1Char(' '));
    for (int i = 0; i < QCaps.length(); i++){
        if (QCaps[i].startsWith("width")){
            size.setWidth((QCaps[i].split(QLatin1Char(')'))[1].remove(-1,1)).toInt(&ok, 10));
        }
        if (QCaps[i].startsWith("height")){
            size.setHeight((QCaps[i].split(QLatin1Char(')'))[1].remove(-1,1)).toInt(&ok,10));
        }
    }

    //set size to tfmtemperature
    m_tfmtemperature->setHeight(size.height());
    m_tfmtemperature->setWidth(size.width());


    if (size.height() != m_format.frameSize().height() || size.width() != m_format.frameSize().width()){
        closeSurface();
        m_format = QVideoSurfaceFormat(size, QVideoFrame::Format_UYVY);
        m_videoSurface->start(m_format);
    }

    frame = QVideoFrame(size.height()*size.width()*2,size,size.width()*2,QVideoFrame::Format_UYVY);
    buf = gst_sample_get_buffer(sample);
    if (!buf){
        setErr("Unable to get buffer");
        return GST_FLOW_ERROR;
    }
    setLastTimestamp(GST_BUFFER_TIMESTAMP(buf));
    gst_buffer_map(buf,&info, GST_MAP_READ);
    frame.map(QAbstractVideoBuffer::ReadWrite);
    if (!frame.isMapped()){
        setErr("Unable to map QVideoFrame");
        return GST_FLOW_ERROR;
    }
    uchar *frameBuf = frame.bits();

    /* Get mapped data*/
    guint8 *dataptr = info.data;
    memcpy(frameBuf,dataptr,info.size);
//    qDebug() << info.size;
    /*Procces mapped data*/
//    copyToDeconv((int16_t *)dataptr, size.width(),size.height());
//    m_deconv->calculate();
//    copyFromDeconv((int16_t *)frameBuf, size.width(), size.height());
    m_tfmtemperature->calcFrame((int16_t *)frameBuf);

//    qDebug() << info.size;
    /*Procces mapped data*/
//    copyToDeconv((int16_t *)dataptr, size.width(),size.height());
//    m_deconv->calculate();
//    copyFromDeconv((int16_t *)frameBuf, size.width(), size.height());
    if (!(!m_tfmtemperature->refCool() | !m_tfmtemperature->refHot()))
        m_tfmtemperature->calcFrame((int16_t *)frameBuf);
    contraster->setBufIn((int16_t *)frameBuf);
    contraster->setBufOut((uint16_t *)frameBuf);
    contraster->process();
    emit newFrame(frame);
    frame.unmap();
    gst_buffer_unmap(buf,&info);
    gst_sample_unref (sample);

    return GST_FLOW_OK;
}

void GstVideoPlayer::updateFrame(QVideoFrame frame){
    bool isPresented = m_videoSurface->present(frame);
    if (!isPresented){
        setErr("Unable to present frame in surface");
    }
}

void GstVideoPlayer::setLastTimestamp(ulong timestamp){
    if (timestamp == m_lastTimestamp)
        return;
    m_fps = 1000000000/(timestamp - m_lastTimestamp);
    m_lastTimestamp = timestamp;
    emit fpsChanged(m_fps);
}

void GstVideoPlayer::copyToDeconv(int16_t * buf,int width, int height){
    float    *pFft = m_deconv->fftInputBuffer();
    pFft +=  m_deconv->fftDataOffset() + (m_deconv->fftSize()*m_deconv->fftDataOffset());


    for (int y=0; y < height; y++) {
        for (int x=0; x < width; x++) {
            pFft[x] = buf[x] * 1.0;
        }
        buf += width;
        pFft += m_deconv->fftSize();
    }

}

void GstVideoPlayer::copyFromDeconv(int16_t * buf, int width, int height){
    float coef = m_deconv->fftSize() * m_deconv->fftSize();
    float *pFft = m_deconv->fftOutputBuffer();

    for (int y=0; y < height; y++) {
        for (int x=0; x < width; x++) {
            buf[x] = (int16_t)( pFft[x] / coef );
        }
        buf += width;
        pFft += m_deconv->fftSize();
    }
}

void GstVideoPlayer::setRefPoints(int x, int y , float t, bool isCool){
    TfmRefpoint  point;
    point.setX(x);
    point.setY(y);
    point.setT(t);
    if (isCool)
        m_tfmtemperature->setRefCool(&point);
    else
        m_tfmtemperature->setRefHot(&point);
}

void GstVideoPlayer::setRoiToStatistic(int x, int y, int height, int width){
    roi.setX(x);
    roi.setY(y);
    roi.setHeight(height);
    roi.setWidth(width);
    m_tfmtemperature->stat()->setRoi(roi);
    qDebug() << m_tfmtemperature->stat()->roi();
}
