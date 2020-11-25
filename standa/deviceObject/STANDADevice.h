#pragma once

#include <QObject>
#include <QString>

class STANDADevice : public QObject
{
    Q_OBJECT
public:
    STANDADevice(QObject *parent = nullptr);
    ~STANDADevice();

    QString getName () { return devName; }
    int     getId   () { return devId; }
    double  getXcor () { return devXcor; }
    double  getYcor () { return devYcor; }
    double  getZcor () { return devZcor; }
    double  getStep () { return devStep; }


public slots:
    void setName(QString name) { devName=name;    emit nameIsChanged(); }
    void setId  (QString idev) { devId=idev.toInt(); emit idIsChanged(); }
    void setXcor(QString xc)   { devXcor=xc.toDouble(); emit xcorIsChanged(); }
    void setYcor(QString yc)   { devYcor=yc.toDouble(); emit ycorIsChanged(); }
    void setZcor(QString zc)   { devZcor=zc.toDouble(); emit zcorIsChanged(); }
    void setStep(QString sp)   { devStep=sp.toDouble(); emit stepIsChanged(); }

    void Init();
    void Delete();

    void stop();
    void moveToXcor();
    void moveToYcor();
    void moveToZcor();
    void moveToXYZ ();

    void check();              // This slot provides checking device to be moved safty
signals:
    void nameIsChanged   ();
    void idIsChanged     ();
    void xcorIsChanged   ();
    void ycorIsChanged   ();
    void zcorIsChanged   ();
    void stepIsChanged   ();

    void deviceIsStopped();

    void deviceMoveStart();
    void deviceMoveEnd  ();

    void deviceIsFound   ();
    void deviceIsNotFound();

    void deviceIsAvailable   ();
    void deviceIsNotAvailable();

private:
    void makeBaseConnections();


private:
    QString devName="";
    int     devId=0;
    double  devXcor=0.;
    double  devYcor=0.;
    double  devZcor=0.;
    double  devStep=0.;

    bool devFoundStatus=false;
    bool devIdIsValid=false;
    bool devXIsValid=false;
    bool devYIsValid=false;
    bool devZIsValid=false;
    bool devStepIsValid=false;
};
