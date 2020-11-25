#pragma once

#include <QObject>
#include <QString>

class STANDASetup : public QObject
{
    Q_OBJECT
public:
    STANDASetup(QObject *parent = nullptr);
    ~STANDASetup();

    QString getDevName () { return deviceName; }
    int     getDevId   () { return devId; }
    double  getXcor    () { return xcor; }
    double  getYcor    () { return ycor; }
    double  getZcor    () { return zcor; }
    double  getStep    () { return step; }

public slots:
    void setDevName(QString name) { deviceName=name;    emit devNameIsChanged(); }
    void setDevId  (QString idev) { devId=idev.toInt(); emit devIdIsChanged(); }
    void setXcor   (QString xc)   { xcor=xc.toDouble(); emit xcorIsChanged(); }
    void setYcor   (QString yc)   { ycor=yc.toDouble(); emit ycorIsChanged(); }
    void setZcor   (QString zc)   { zcor=zc.toDouble(); emit zcorIsChanged(); }
    void setStep   (QString sp)   { step=sp.toDouble(); emit stepIsChanged(); }

    void InitDevice();
    void DeleteDevice();

    void stopDevice();
    void moveToXcor();
    void moveToYcor();
    void moveToZcor();
    void moveToXYZ ();

signals:
    void devNameIsChanged();
    void devIdIsChanged  ();
    void xcorIsChanged   ();
    void ycorIsChanged   ();
    void zcorIsChanged   ();
    void stepIsChanged   ();

    void deviceIsStopped();

    void deviceMoveStart();
    void deviceMoveEnd  ();

    void deviceIsFound   ();
    void deviceIsNotFound();

private:
    void makeBaseConnections();

private:
    QString deviceName="";
    int    devId=0;
    double xcor=0.;
    double ycor=0.;
    double zcor=0.;
    double step=0.;
};
