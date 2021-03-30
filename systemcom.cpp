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
    m_secondBB(new BlackBody(this)),
    m_windHeight(480),
    m_windWidth(640),
    m_address("")
{
    makeDir();
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

int systemCom::windHeight() const{
    return m_windHeight;
}

int systemCom::windWidth() const{
    return m_windWidth;
}

bool systemCom::fullscreen() const{
    return m_fullscreen;
}

QString systemCom::address() const{
    return m_address;
}

void systemCom::setWindHeight(int height){
    if (m_windHeight == height)
        return;
    m_windHeight = height;
}

void systemCom::setWindWidth(int width){
    if (m_windWidth == width)
        return;
    m_windWidth = width;
}

void systemCom::setFullscreen(bool fullscreen){
    if (m_fullscreen == fullscreen)
        return;
    m_fullscreen = fullscreen;
}

void systemCom::setAddress(QString address){
    if (m_address == address)
        return;
    m_address = address;
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

void systemCom::createConfigFile(){
    if (!QFile::exists(m_appPath+"/config.json")){
        QFile configFile(m_appPath+"/config.json");
        configFile.open(QIODevice::WriteOnly);
        configFile.close();
    }
    QFile  configFile(m_appPath+"/config.json");
    if (configFile.size() != 0)
        QTimer::singleShot(100,this,SLOT(checkConfig()));
}

void systemCom::writeToConfig(){
    QJsonObject config;
    QJsonObject blackBody1{ {"x",m_firstBB->x()}, {"y",m_firstBB->y()},
                            {"t",m_firstBB->t()}, {"listen",m_firstBB->listen()},
                            {"mac",m_firstBB->mac()} };

    QJsonObject blackBody2{ {"x",m_secondBB->x()}, {"y",m_secondBB->y()},
                            {"t",m_secondBB->t()}, {"listen",m_secondBB->listen()},
                            {"mac",m_secondBB->mac()}};
    QJsonObject wind {{"height", m_windHeight}, {"width", m_windWidth},
                     {"fullscreen", m_fullscreen}};
    QJsonObject addr {{"address",m_address}};
    config["FirstBlackBody"] = blackBody1;
    config["SecondBlackBody"] = blackBody2;
    config["window"] = wind;
    config["device"] = addr;

    if (!QFile::exists(m_appPath+"/config.json")){
        createConfigFile();
    }
    QFile configFile(m_appPath+"/config.json");
    configFile.open(QIODevice::WriteOnly);
    configFile.write(QJsonDocument(config).toJson(QJsonDocument::Indented));
    configFile.close();

}

void systemCom::checkConfig(){
    QFile configFile(m_appPath+"/config.json");
    configFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray config;
    config = configFile.readAll();
    configFile.close();
    emit hasConfig(QString::fromStdString(config.toStdString()));
}
