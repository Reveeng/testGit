#ifndef ONVIF_H
#define ONVIF_H
#include <QtCore>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <functional>
#include <QObject>
#include "systemcom.h"

class onvif : public systemCom
{
    Q_OBJECT
    Q_PROPERTY(QString adress READ adress WRITE setAdress)
    Q_PROPERTY(bool timeoutCounter READ timeoutCounter WRITE setTimeoutCounter NOTIFY timeoutCounterChanged)
public:
    explicit onvif(QString appPath,QObject *parent = nullptr);
    ~onvif();

    void sendRequest(QString servicePath,QString soapReqBody, QString cmd);

    QString adress() const;

    bool timeoutCounter() const;

private:
    void makeEnvelopeBegin(QString securityHeader);
    QNetworkAccessManager *manager;
    QString m_adress;
    QString soap;
    bool m_timeoutCounter;

public slots:
    void sendDeviceCommand(QString cmd);
    void synchronizeTime(QString comand, QDate date, int H,int M,int S);
    void setAdress(QString adress);
    void setTimeoutCounter(bool timeoutCounter);

signals:
    void successRequest(QString message);
    void unsuccessRequest();
    void timeout();
    void sendToLog(QString cmd, QString reply);
    void timeoutCounterChanged(bool timeoutCounter);
};


#endif // ONVIF_H
