#ifndef SNAPSHOT_H
#define SNAPSHOT_H
#include <QObject>
#include <QtCore>
#include "onvif.h"
#include "systemcom.h"
#include "QDebug"
#include "QDir"
#include <QQmlListProperty>

class snapshot : public onvif{
    Q_OBJECT
    Q_PROPERTY(QStringList macs READ macs WRITE setMacs NOTIFY macsChanged)
    Q_PROPERTY(QStringList temps READ temps WRITE setTemps NOTIFY tempsChanged)
    Q_PROPERTY(bool modeOneBB READ modeOneBB WRITE setModeOneBB NOTIFY modeOneBBChanged)
    Q_PROPERTY(bool listen1 READ listen1 WRITE setListen1)
    Q_PROPERTY(bool listen2 READ listen2 WRITE setListen2)
    Q_PROPERTY(QString replyError READ replyError WRITE setReplyError NOTIFY replyErrorChanged)

public:
    QMap<QString,void(snapshot::*)(QString)> functionMap;

    explicit snapshot(QString appPath);
    ~snapshot();

    void initFunctionMap();

    bool modeOneBB() const
    {
        return m_modeOneBB;
    }

    void setModeOneBB(bool modeOneBB){
        if(m_modeOneBB == modeOneBB)
            return;
        m_modeOneBB = modeOneBB;
    }

    bool listen1() const
    {
        return m_listen1;
    }
    void setListen1(bool listen1)
    {
        m_listen1 = listen1;
    }

    bool listen2() const
    {
        return m_listen2;
    }
    void setListen2(bool listen2)
    {
        m_listen2 = listen2;
    }

    QString replyError() const
    {
        return m_replyError;
    }

    void setReplyError(QString replyError)
    {
        if (m_replyError == replyError)
            return;

        m_replyError = replyError;
        emit replyErrorChanged(m_replyError);
    }

    QList<QString> macs() const
    {
        return m_macs;
    }

    QList<QString> temps() const
    {
        return m_temps;
    }

    void setMacs(QStringList macs)
    {
        if (m_macs == macs)
            return;

        m_macs = macs;
        emit macsChanged(m_macs);
    }

    void setTemps(QStringList temps){
        if (m_temps == temps)
            return;
        m_temps = temps;
        emit tempsChanged(m_temps);
    }

private:
    void getRefPointsSuc(QString message);
    void getRefPointsUnsuc(QString message);
    void allBlackBodyAdressSuc(QString message);
    void allBlackBodyAdressUnsuc(QString message);
    void meterSaveScaleSuc(QString message);
    void meterSaveScaleUnsuc(QString message);
    void calibrRefPointsSuc(QString message);
    void calibrRefPointsUnsuc(QString message);
    bool catchError(QString reply);
    void getPaletteSuc(QString message);
    void getTempCorrectionSuc(QString message);
    void getRectangleSuc(QString message);
    //properties
    QString m_temp1;

    QString m_temp2;

    QString m_mac1;

    QString m_mac2;

    bool m_modeOneBB;

    bool m_listen1;

    bool m_listen2;

    QString m_replyError;

    QString m_pathToSaveDir;

    QStringList m_macs;

    QStringList m_temps;


public slots:
    void meterSaveScale();
    void getRefPoints();
    void aBBAdressWithDelay();
//    void getTempInPoint(int x, int y);
    void allBlackBodyAdress();
    void calibrRefPoints(QStringList bb1,QStringList bb2);
    void shutterCalibr();
    void updateIrRefpoint();
    void setPalleteClick(QString low,QString alarm, QString high,QString type);
    void getPalette();
    void addRectClick(QString x, QString y, QString width, QString height);
    void getTempCorrection();
    void applyTempCorrections(QString blackness,QString tAmb);
    void getRectangle();


private slots:
    void processAuxCommandResponse(QString reply);
//    void proccessErrResponce(QString replyerror);

signals:
    void modeOneBBChanged(bool modeOneBB);
    void replyErrorChanged(QString replyError);
    void macsChanged(QStringList macs);
    void tempsChanged(QStringList temps);
    void bbsReady(QStringList hotBB,QStringList coldBB);
    void palleteReady(QStringList pallete);
    void correctionReady(QStringList blacknessAndTemp);
    void rectangleReady(QList<int> rectangle);
    void getABBadress();
};



#endif // SNAPSHOT_H
