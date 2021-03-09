#include "onvif.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QRegularExpression>

onvif::onvif(QObject *parent): QObject(parent){
    manager = new QNetworkAccessManager(this);
    setTimeoutCounter(true);
    this->makeEnvelopeBegin("");
}

onvif::~onvif(){
    delete manager;
}

void onvif::makeEnvelopeBegin(QString securityHeader){
    QString soap = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<SOAP-ENV:Envelope"
            "   xmlns:SOAP-ENV=\"http://www.w3.org/2003/05/soap-envelope\""
            "   xmlns:SOAP-ENC=\"http://www.w3.org/2003/05/soap-encoding\""
            "   xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\""
            "   xmlns:xsd=\"http://www.w3.org/2001/XMLSchema\""
            "   xmlns:chan=\"http://schemas.microsoft.com/ws/2005/02/duplex\""
            "   xmlns:wsa5=\"http://www.w3.org/2005/08/addressing\""
            "   xmlns:wsdd=\"http://schemas.xmlsoap.org/ws/2005/04/discovery\""
            "   xmlns:xmime=\"http://tempuri.org/xmime.xsd\""
            "   xmlns:xop=\"http://www.w3.org/2004/08/xop/include\""
            "   xmlns:tt=\"http://www.onvif.org/ver10/schema\""
            "   xmlns:wsnt=\"http://docs.oasis-open.org/wsn/b-2\""
            "   xmlns:wsrfbf=\"http://docs.oasis-open.org/wsrf/bf-2\""
            "   xmlns:wstop=\"http://docs.oasis-open.org/wsn/t-1\""
            "   xmlns:ns1=\"http://www.onvif.org/ver10/thermal/wsdl\""
            "   xmlns:tds=\"http://www.onvif.org/ver10/device/wsdl\""
            "   xmlns:tptz=\"http://www.onvif.org/ver20/ptz/wsdl\""
            "   xmlns:trt=\"http://www.onvif.org/ver10/media/wsdl\""
            "   xmlns:wsse=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-secext-1.0.xsd\""
            "   xmlns:wsu=\"http://docs.oasis-open.org/wss/2004/01/oasis-200401-wss-wssecurity-utility-1.0.xsd\">"
            "<SOAP-ENV:Header>";
            soap +=    securityHeader;
            soap += "</SOAP-ENV:Header>";
            soap += "<SOAP-ENV:Body>";
            this->soap = soap;
//            return soap;
}


void onvif::sendRequest(QString servicePath,QString soapReqBody, QString cmd){
//    QString securityHeader = "";
    QString soapReq = /*makeEnvelopeBegin(securityHeader)*/ this->soap + soapReqBody + "</SOAP-ENV:Body></SOAP-ENV:Envelope>";
    QNetworkRequest request;
    request.setUrl(QUrl(servicePath));
    request.setHeader(QNetworkRequest::ContentTypeHeader,"text/xml");
    QNetworkReply *reply;
    QByteArray data = soapReq.toLocal8Bit();
    reply = manager->post(request,soapReq.toLocal8Bit());
    QTimer timer;
    timer.setSingleShot(true);
    qDebug() << soapReq;
    QEventLoop loop;
    connect(&timer, SIGNAL(timeout()), &loop, SLOT(quit()));
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    timer.start(3000);
    loop.exec();
    if (timer.isActive()){
        if(reply->error() == QNetworkReply::NoError){
            int code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
            if ((code >= 200) && (code < 300)) {
                QString replytext = QString(reply->readAll());
                QRegularExpression re("\\[(.*)\\]");
                QRegularExpressionMatch match = re.match(replytext);
                if (match.hasMatch()){
                    QString reply = match.captured(1);
                    emit sendToLog(cmd,reply);
                }
                qDebug() << replytext;
                setTimeoutCounter(true);
                emit successRequest(replytext);
            }
        }
        else{
            emit sendToLog(cmd, QString(reply->error()));
            if (reply->error() == QNetworkReply::UnknownNetworkError){
                emit timeout();
                if (timeoutCounter())
                    setTimeoutCounter(false);
            }
            qDebug() << reply->error();
        }

    }
    else  {
        disconnect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
        emit sendToLog(cmd, "timeout");
        emit timeout();
        if (timeoutCounter()){
            qDebug() << "зашел в таймаут";
            setTimeoutCounter(false);
        }
        reply->abort();
    }

}

void onvif::sendDeviceCommand(QString cmd){
    QString req = "<tds:SendAuxiliaryCommand><tds:AuxiliaryCommand>infrtst:DeviceCommand[";
    req+=cmd + "]</tds:AuxiliaryCommand></tds:SendAuxiliaryCommand>";
    onvif::sendRequest("http://"+onvif::adress()+"/onvif/device_service",req,cmd);
}

void onvif::getRefPoints(QString message){
    QStringList bbList = message.split(QLatin1Char(' '));
    QStringList hotBB;
    QStringList coldBB;
    if (bbList.size() != 1){
        bbList.removeFirst();
        hotBB = bbList[0].split(QLatin1Char(','));
        (hotBB[2].indexOf('.') == -1) ? hotBB << "mac" : hotBB << "temp";
        if (bbList.size() != 1){
            coldBB = bbList[1].split(QLatin1Char(','));
            (coldBB[2].indexOf('.') == -1) ? coldBB << "mac" : coldBB << "temp";
        }
        else
            coldBB << "0";
//        snapshot::bbsReady(hotBB,coldBB);
    }
}

