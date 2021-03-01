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
    m_pipeline(nullptr),
    m_appsink(nullptr),
    m_sinkName("sink0")

{
    gst_init(NULL, NULL);
    connect(this, &GstVideoPlayer::newFrame, this, &GstVideoPlayer::updateFrame);
}
GstVideoPlayer::~GstVideoPlayer(){
    closeSurface();
}

void GstVideoPlayer::registerQmlType()
{
    qmlRegisterType<GstVideoPlayer>(
            "GstVideoPlayer", 0, 1,
            "GstVideoPlayer" );
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
    gst_buffer_map(buf,&info, GST_MAP_READ);
    frame.map(QAbstractVideoBuffer::ReadWrite);
    if (!frame.isMapped()){
        setErr("Unable to map QVideoFrame");
        return GST_FLOW_ERROR;
    }
    uchar *frameBuf = frame.bits();

    /* Get mapped data*/
    guint8 *dataptr = info.data;
//    qDebug() << info.size;
    memcpy(frameBuf,dataptr,info.size);
    /*Procces mapped data*/

//    setLastTimestamp(GST_BUFFER_TIMESTAMP(buf));
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

