#include "systemcom.h"
#include <QStandardPaths>
#include <QDir>
#include <QTextStream>
#include <QDebug>
#include <QTimer>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

systemCom::systemCom(QString appPath, QObject *parent):QObject(parent),
    m_appPath(appPath),
    m_firstBB(new BlackBody(this)),
    m_secondBB(new BlackBody(this))
{
    makeDir();
    createDeviceFile();
    createConfigFile();
}

systemCom::~systemCom(){

}

QString systemCom::appPath() const
{
    return m_appPath;
}

QString systemCom::pathToSaveDir() const
{
    return m_pathToSaveDir;
}

void systemCom::setPathToSaveDir(QString pathTSD){
    if (m_pathToSaveDir == pathTSD)
        return;
    else{
        m_pathToSaveDir = pathTSD;
    }
}

BlackBody * systemCom::firstBB() const{
    return m_firstBB;
}

BlackBody * systemCom::secondBB() const{
    return m_secondBB;
}

void systemCom::makeDir(){
    const QStringList homepath =  QStandardPaths::standardLocations(QStandardPaths::DesktopLocation);
    QString savePath = QString("%1").arg(homepath.first());
    QDir deskDir = QDir(savePath);
    savePath += "/CoronovisorSavedFrames";
    QDir dirname = QDir(savePath);
    if (!dirname.exists()){
        deskDir.mkdir(savePath);
        setPathToSaveDir(savePath);
    }
    else{
        setPathToSaveDir(savePath);
    }
}

void systemCom::createDeviceFile(){
    if (!QFile::exists(m_appPath+"/device.txt")){
        QFile deviceFile(m_appPath+"/device.txt");
        deviceFile.open(QIODevice::WriteOnly);
        deviceFile.close();
    }
    else{
        QTimer::singleShot(100,this,SLOT(checkAddress()));
    }
}

void systemCom::createConfigFile(){
    if (!QFile::exists(m_appPath+"/config.json")){
        QFile configFile(m_appPath+"/config.json");
        configFile.open(QIODevice::WriteOnly);
        configFile.close();
    }
    QFile  configFile(m_appPath+"/config.json");
    if (configFile.size() != 0)
        connect(this, &systemCom::hasAddress, this , &systemCom::checkConfig);
}

void systemCom::checkAddress(){
    QFile deviceFile(m_appPath+"/device.txt");
    deviceFile.open(QIODevice::ReadOnly);
    QByteArray line;
    while (!deviceFile.atEnd()){
          line = deviceFile.readLine();
    }
    if (line.size() != 0){
        emit hasAddress(QString::fromStdString(line.toStdString()));
    }
}

void systemCom::writeToConfig(){
    qDebug() << m_firstBB->x() << m_firstBB->y() << m_firstBB->t()<< m_firstBB->listen() << m_firstBB->mac();
    qDebug() << m_secondBB->x() << m_secondBB->y() << m_secondBB->t()<< m_secondBB->listen() << m_secondBB->mac();
    QJsonObject twoBB;
    QJsonObject blackBody1{ {"x",m_firstBB->x()}, {"y",m_firstBB->y()},
                            {"t",m_firstBB->t()}, {"listen",m_firstBB->listen()},
                            {"mac",m_firstBB->mac()} };

    QJsonObject blackBody2{ {"x",m_secondBB->x()}, {"y",m_secondBB->y()},
                            {"t",m_secondBB->t()}, {"listen",m_secondBB->listen()},
                            {"mac",m_secondBB->mac()}};
    twoBB["FirstBlackBody"] = blackBody1;
    twoBB["SecondBlackBody"] = blackBody2;
    if (!QFile::exists(m_appPath+"/config.json")){
        createConfigFile();
    }
    QFile configFile(m_appPath+"/config.json");
    configFile.open(QIODevice::WriteOnly);
    configFile.write(QJsonDocument(twoBB).toJson(QJsonDocument::Indented));
    configFile.close();

}

void systemCom::checkConfig(QString address){
    Q_UNUSED(address);
    QFile configFile(m_appPath+"/config.json");
    configFile.open(QIODevice::ReadOnly);
    QByteArray config;
    config = configFile.readAll();
    configFile.close();
    emit hasConfig(QString::fromStdString(config.toStdString()));
}

void systemCom::saveAddress(QString address){
    QFile deviceFile(m_appPath+"/device.txt");
    deviceFile.open(QIODevice::WriteOnly);
    QTextStream stream(&deviceFile);
    stream << address;
    deviceFile.close();
}
