#include "snapshot.h"
#include "onvif.h"
#include "QtDebug"
#include "QDir"
#include <QStandardPaths>
#include <QTimer>
#include <QDateTime>
#include <QTimeZone>

void snapshot::initFunctionMap(){
    connect(this,SIGNAL(successRequest(QString)),this,SLOT(processAuxCommandResponse(QString)));
//    connect(this,SIGNAL(unsuccessRequest(QString)),this,SLOT(proccessErrResponce(QString)));
    functionMap.insert("MeterSaveScale",&snapshot::meterSaveScaleSuc);
    functionMap.insert("AllBlackbodyAddresses", &snapshot::allBlackBodyAdressSuc);
    functionMap.insert("CalibrRefPoints",&snapshot::calibrRefPointsSuc);
    functionMap.insert("GetRefPoints",&snapshot::getRefPointsSuc);
    functionMap.insert("TemperaturePalette", &snapshot::getPaletteSuc);
    functionMap.insert("TemperatureCorrection", &snapshot::getTempCorrectionSuc);
    functionMap.insert("MeasureRectangle",&snapshot::getRectangleSuc);
    this->setModeOneBB(true);
}


void snapshot::meterSaveScaleSuc(QString message){
    qDebug() << "OK" << message;
}
void snapshot::meterSaveScaleUnsuc(QString message){
    qDebug() << "Not Ok"<< message;
}
void snapshot::meterSaveScale(){
    this->sendDeviceCommand("MeterSaveScale");
}

void snapshot::getRefPointsSuc(QString message){
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
        snapshot::bbsReady(hotBB,coldBB);
    }
}

void snapshot::getRefPointsUnsuc(QString message){
   qDebug()<<"not ok";
}

void snapshot::getRefPoints(){
    this->sendDeviceCommand("GetRefPoints");
}

//void snapshot::getTempInPointSuc(QString message){
//    QRegularExpression re("^([0-9]+)");
//    QRegularExpressionMatch match = re.match(message);
//    if (!match.hasMatch())
//        qDebug()<< "Failed to get temperature in point: unexpected response";
//    else
//       qDebug() << QString((match.captured(0).toInt()/10))+"°C";
//}
//void snapshot::getTempInPointUnsuc(QString message){
//    qDebug() << "Failed to get temperature in point";
//}

//void snapshot::getTempInPoint(int x, int y){
//    QString cmd =  "meter get " + QString::number(x)+" "+QString::number(y);
//    this->sendIRMCommand(cmd);
//}

bool snapshot::catchError(QString reply){
        reply = reply.toLower();
        QRegularExpression re("fail.*");
        QRegularExpression re2(".*invalid.*");
        QRegularExpressionMatch match = re.match(reply);
        QRegularExpressionMatch match2 = re2.match(reply);
        if (match.hasMatch() || match2.hasMatch())
            return true;
        else
            return false;
}

//void snapshot::proccessErrResponce(QString replyerror){
//    if (replyerror == "QNetworkReply::UnknownNetworkError"){
//        qDebug() << "потеряна сеть";
//    }
//}

void snapshot::processAuxCommandResponse(QString replytext){
    QRegularExpression re("\\[(.*)\\]");
    QRegularExpressionMatch match = re.match(replytext);
    if (match.hasMatch()){
        QString reply = match.captured(1);
        QString funcname = reply.split(QLatin1Char(':'))[0];
        if (this->catchError(reply)){
            QString error = reply.remove(-1,1);
            error.remove(0,1);
            this->setReplyError(error);
            qDebug() << this->replyError();
        }
        else {
            QList<QString> funcList = functionMap.keys();
            if (funcList.contains(funcname))
                (this->*functionMap[funcname])(reply);
        }
    }
    else
        qDebug() << "empty responce";
}

void snapshot::aBBAdressWithDelay(){
    QTimer::singleShot(2000,this,SLOT(allBlackBodyAdress()));
}

void snapshot::allBlackBodyAdressSuc(QString message){
        QList<QString> MACandTempList = message.split(QLatin1Char(' '));
        MACandTempList.removeFirst();
        QStringList MACs;
        QStringList Temps;
        if (MACandTempList[0] != ""){
            for (int iter = 0;iter < MACandTempList.size();iter++){
                QList<QString> MACandTemp = MACandTempList[iter].split(QLatin1Char(','));
                MACs << MACandTemp[0];
                Temps << MACandTemp[1];
            }
        }
//        qDebug() << MACs;
        snapshot::setMacs(MACs);
        snapshot::setTemps(Temps);
        emit getABBadress();

}

void snapshot::allBlackBodyAdressUnsuc(QString message){
    qDebug() << "Not ok";
}

void snapshot::allBlackBodyAdress(){
    this->sendDeviceCommand("AllBlackbodyAddresses");
}

void snapshot::calibrRefPointsSuc(QString message){
    qDebug()<< "ok";
}
void snapshot::calibrRefPointsUnsuc(QString message){
    qDebug() << "not ok";
}


void snapshot::calibrRefPoints(QStringList bb1, QStringList bb2){
    QString cmd = "CalibrRefPoints";
    QString mes1 = this->listen1() ? bb1[3] : bb1[2];
    QString mes2;
    if (this->modeOneBB())
        cmd +=" "+bb1[0]+","+bb1[1]+","+mes1;
    else{
        mes2 = this->listen2() ? bb2[3] : bb2[2];
        cmd+=" "+bb1[0]+","+bb1[1]+","+mes1+" "+bb2[0]+","+bb2[1]+","+mes2;
    }
    if (this->modeOneBB() && mes1 == mes2)
        qDebug() << "Temperatures(addresses) of black bodyes can not be equal";
    qDebug()<< cmd;
    this->sendDeviceCommand(cmd);

}

void snapshot::shutterCalibr(){
    this->sendDeviceCommand("CalibrShutter");
}

void snapshot::updateIrRefpoint(){
    this->sendDeviceCommand("UpdateIrRefpoint");
}
void snapshot::setPalleteClick(QString low,QString alarm, QString high,QString type){
    QString cmd = "TemperaturePalette ";
    cmd+= low+" "+alarm+" "+high+" "+type;
    this->sendDeviceCommand(cmd);
}

void snapshot::getPaletteSuc(QString message){
    qDebug() << message;
    QStringList palette = message.split(QLatin1Char(' '));
    palette.removeFirst();
    qDebug() << "pallete" << palette;
    if (palette.size() != 1)
        emit palleteReady(palette);
}

void snapshot::getPalette(){
    QString cmd = "TemperaturePalette get";
    this->sendDeviceCommand(cmd);
}

void snapshot::getRectangleSuc(QString message){
    QStringList rectangle = message.split(QLatin1Char(' '));
    rectangle.removeFirst();
    if (rectangle.size()!=1){
        QList<int> rectInt;
        int i = 0;
        while (i != rectangle.size()){
            rectInt << rectangle[i].toInt();
            i++;
        }
        qDebug() << rectInt;
        emit rectangleReady(rectInt);
    }
}

void snapshot::getRectangle(){
    this->sendDeviceCommand("MeasureRectangle get");
}

void snapshot::addRectClick(QString x, QString y, QString width, QString height){
    QString cmd = "MeasureRectangle "+x+" "+y+" "+width+" "+height;
    this->sendDeviceCommand(cmd);
}

void snapshot::getTempCorrectionSuc(QString message){
    QStringList blacknessAndTemp = message.split(QLatin1Char(' '));
    blacknessAndTemp.removeFirst();
    emit correctionReady(blacknessAndTemp);
    qDebug() << message;
}

void snapshot::getTempCorrection(){
    this->sendDeviceCommand("TemperatureCorrection get");
}
void snapshot::applyTempCorrections(QString blackness,QString tAmb){
    QString cmd = "TemperatureCorrection "+blackness+" "+tAmb;
    this->sendDeviceCommand(cmd);
}


