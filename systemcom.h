#ifndef SYSTEMCOM_H
#define SYSTEMCOM_H

#include <QObject>

class systemCom:public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString appPath READ appPath)
    Q_PROPERTY(QString pathToSaveDir READ pathToSaveDir WRITE setPathToSaveDir)
public:
    explicit systemCom(QString appPath, QObject *parent = nullptr);
    ~systemCom();
    QString appPath() const;
    QString pathToSaveDir() const;
public slots:
    void setPathToSaveDir(QString pathTSD);
    void makeDir();
    void checkAddress();
    void saveAddress(QString address);

private:
    void createDeviceFile();
    void createConfigFile();
    void checkConfig();
    QString m_appPath;
    QString m_pathToSaveDir;

signals:
    void hasAddress(QString address);
};

#endif // SYSTEMCOM_H
