#ifndef ONVIF_H
#define ONVIF_H
#include <QtCore>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <functional>
#include <QObject>

class onvif : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString adress READ adress WRITE setAdress)
    Q_PROPERTY(bool timeoutCounter READ timeoutCounter WRITE setTimeoutCounter NOTIFY timeoutCounterChanged)
public:
    int offset;
    explicit onvif(QObject *parent = 0);
    ~onvif();
    void sendRequest(QString servicePath,QString soapReqBody, QString cmd);
//    void sendIRMCommand(QString cmd);

    void sendGetReq();
    QString adress() const
    {
        return m_adress;
    }

    bool timeoutCounter() const
    {
        return m_timeoutCounter;
    }

private:
    QNetworkAccessManager *manager;
    void makeEnvelopeBegin(QString securityHeader);
    QString m_adress;
    QString soap;
    QThread *_thread;

    bool m_timeoutCounter;

public slots:
    void sendDeviceCommand(QString cmd);
    void synchronizeTime(QString comand, QDate date, int H,int M,int S);
    void setAdress(QString adress)
    {
        m_adress = adress;
    }

    void setTimeoutCounter(bool timeoutCounter)
    {
        if (m_timeoutCounter == timeoutCounter)
            return;

        m_timeoutCounter = timeoutCounter;
        emit timeoutCounterChanged(m_timeoutCounter);
    }

signals:
    void successRequest(QString message);
    void unsuccessRequest();
    void timeout();
    void sendToLog(QString cmd, QString reply);
    void timeoutCounterChanged(bool timeoutCounter);
};


#endif // ONVIF_H
