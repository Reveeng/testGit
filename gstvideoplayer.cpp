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
    m_source(""),
    m_pipeline(nullptr),
    m_appsink(nullptr),
    m_sinkName("sink0")
{
    gst_init(NULL, NULL);
    m_launchstring = "rtspsrc location=__URL__ latency=0 ! "
                     "rtpvrawdepay ! queue ! "
                     "appsink max-buffers=3 drop=true emit-signals=true name="+m_sinkName;
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

void GstVideoPlayer::setSurface (QAbstractVideoSurface *surface){
    if (m_surface)
        closeSurface();
    m_surface = surface;
}

void GstVideoPlayer::closeSurface(){
    if (m_surface && m_surface->isActive())
        m_surface->stop();
}

QAbstractVideoSurface* GstVideoPlayer::surface() const{
    return m_surface;
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
//    QString launch = m_launchstring.replace("__URL__",m_url);
    qDebug() << m_source;
    if (m_source == "")
    {
        setErr("Source cant be empty");
        return;
    }
    m_pipeline = gst_parse_launch(qPrintable(m_source),NULL);
    m_appsink = gst_bin_get_by_name(GST_BIN(m_pipeline), qPrintable(m_sinkName));

    if (!m_pipeline){
        setErr("Failed to start pipeline");
        return;
    }
    if (!m_appsink){
        setErr("Failed to get appsrc");
        return;
    }

    GstStateChangeReturn res = gst_element_set_state(GST_ELEMENT(m_pipeline), GST_STATE_PLAYING);
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
    GstMapInfo *info;
//    QSize size;
    /* Retrieve the buffer */
    g_signal_emit_by_name (m_appsink, "pull-sample", &sample);
    if (! sample) {
        setErr("Cant get frame from sample");
        return GST_FLOW_ERROR;
    }
    GstCaps * caps = gst_sample_get_caps(sample);
    guint capsize = gst_caps_get_size(caps);
    gchar *StringCaps = gst_caps_to_string(caps);
    qDebug() << StringCaps;
    qDebug() << capsize;
//    for (guint i = 0; i < capsize; i++){
//        GstStructure * oneStruct = gst_caps_get_structure(caps,i);
//        QString structureName = QString(gst_structure_get_name(oneStruct));
//        if (structureName == "width" ){
//            const GValue * gwidth = gst_structure_get_value(oneStruct,"width");
//            size.setWidth(*(int*) gwidth);
//        }
//        if (structureName == "height"){
//            const GValue * gheight = gst_structure_get_value(oneStruct,"height");
//            size.setHeight(* (int *) gheight);
//        }
//    }
    buf = gst_sample_get_buffer(sample);
    gst_buffer_map(buf,info, GST_MAP_READ);
//    bool isMapped = frame->map(QAbstractVideoBuffer::ReadWrite);
//    if (!isMapped){
//        setErr("Unable to map QVideoFrame");
//        return GST_FLOW_ERROR;
//    }
//    uchar *frameBuf = frame->bits();

    /* Get mapped data*/
    signed short *dataptr = (signed short *) info->data;
//    memccpy(frameBuf,dataptr,info->size);
    /*Procces mapped data*/

//    setLastTimestamp(GST_BUFFER_TIMESTAMP(buf));
//    emit newFrame();
//    bool isPresented = m_surface->present(*frame);
//    if (!isPresented){
//        setErr("Unable to present frame in surface");
//        frame->unmap();
//        gst_buffer_unmap(buf,info);
//        gst_sample_unref(sample);
//        return GST_FLOW_ERROR;
//    }
//    frame->unmap();
    gst_buffer_unmap(buf,info);
    gst_sample_unref (sample);
    return GST_FLOW_OK;
}
