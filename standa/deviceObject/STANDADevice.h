#pragma once

#include <QObject>
#include <QString>

class STANDADevice : public QObject
{
    Q_OBJECT
public:
    STANDADevice(QObject *parent = nullptr);
    ~STANDADevice();

    QString getName() { return devName; }
    int     getId  () { return devId; }
    double  getPos () { return devPos; }
    double  getStep() { return devStep; }

    bool check(); // This method provides checking device to be moved safty

public slots:
    void setName(QString name) { devName=name;          emit nameIsSet(); }
    void setId  (QString idev) { devId=idev.toInt();    emit idIsSet(); }
    void setPos (QString pos)  { devPos=pos.toDouble(); emit posIsSet(); }
    void setStep(QString sp)   { devStep=sp.toDouble(); emit stepIsSet(); }

    void Init();
    void Delete();

    void stop();
    void move();

signals:
    void nameIsSet   ();
    void idIsSet     ();
    void posIsSet   ();
    void stepIsSet   ();

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
    double  devPos=0.;
    double  devStep=0.;

    bool devFoundStatus=false;
    bool devIdIsValid=false;
    bool devPosIsValid=false;
    bool devStepIsValid=false;
};
