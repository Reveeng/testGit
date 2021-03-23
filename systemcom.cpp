#include "systemcom.h"
#include <QStandardPaths>
#include <QDir>
#include <QTextStream>
#include <QDebug>
#include <QTimer>

systemCom::systemCom(QString appPath, QObject *parent):QObject(parent),
    m_appPath(appPath)
{
    makeDir();
    createDeviceFile();
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
        QTimer::singleShot(1,this,SLOT(checkAddress()));
    }
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

void systemCom::saveAddress(QString address){
    QFile deviceFile(m_appPath+"/device.txt");
    deviceFile.open(QIODevice::WriteOnly);
    QTextStream stream(&deviceFile);
    stream << address;
    deviceFile.close();
}
