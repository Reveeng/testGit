#ifndef SYSTEMCOM_H
#define SYSTEMCOM_H

#include <QObject>

class BlackBody{
    Q_OBJECT

    Q_PROPERTY(int x READ x WRITE setX)
    Q_PROPERTY(int y READ y WRITE setY)
    Q_PROPERTY(float t READ t WRITE setT)
    Q_PROPERTY(bool listen READ listen WRITE setListen)
    Q_PROPERTY(QString mac READ mac WRITE setMac)

public:
    int x() const{
        return m_x;};
    int y() const{
        return m_y;};
    float t() const{
        return m_t;};
    bool listen() const{
        return m_listen;};
    QString mac() const{
        return m_mac;};

    void setX(int x){
        if (m_x == x) return;
        else m_x = x;}

    void setY(int y){
        if (m_y == y) return;
        else m_y = y;}

    void setT(float t){
        if (m_t == t) return;
        else m_t = t;}

    void setListen(bool listen){
        if (m_listen == listen) return;
        else m_listen = listen;}

    void setMac(QString mac){
        if (m_mac == mac) return;
        else m_mac = mac;}

private:
    int m_x;
    int m_y;
    float m_t;
    bool m_listen;
    QString m_mac;
};

class systemCom:public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString appPath READ appPath)
    Q_PROPERTY(QString pathToSaveDir READ pathToSaveDir WRITE setPathToSaveDir)
    Q_PROPERTY(BlackBody *firstBB READ firstBB )
    Q_PROPERTY(BlackBody *secondBB READ secondBB )
public:
    explicit systemCom(QString appPath, QObject *parent = nullptr);
    ~systemCom();
    QString appPath() const;
    QString pathToSaveDir() const;
    BlackBody* firstBB() const;
    BlackBody* secondBB() const;
public slots:
    void setPathToSaveDir(QString pathTSD);
    void makeDir();
    void checkAddress();
    void saveAddress(QString address);
    void writeToConfig();

private:
    void createDeviceFile();
    void createConfigFile();
    void checkConfig();
    QString m_appPath;
    QString m_pathToSaveDir;
    BlackBody *m_firstBB;
    BlackBody *m_secondBB;

signals:
    void hasAddress(QString address);
};

#endif // SYSTEMCOM_H
