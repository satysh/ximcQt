#pragma once

#include <ximc.h>

#include <QObject>
#include <QString>

class STANDADevice : public QObject
{
    Q_OBJECT
public:
    STANDADevice(QObject *parent = nullptr);
    ~STANDADevice();
    
    void setHomePos(int pos) { m_homePos=pos; }


    int getHomePos()  { return m_homePos; }
    QString getName() { return devName; }
    int     getId  () { return m_device; }
    double  getPos () { return devPos; }
    double  getStep() { return devStep; }

public:
    void setNomSpeed(int speed);

    int getNomVoltage();
    int getNomCurrent();
    int getNomSpeed();

public:
    bool check(); // This method provides checking device to be moved safty
    void moveToBasePos(); // TODO Implement me!

protected:
    virtual void timerEvent(QTimerEvent*);

public slots:
    void setName(QString name) { devName=name;          emit nameIsSet(); }
    //void setId  (QString idev) { devId=idev.toInt();    emit idIsSet(); }
    void setPos (QString pos)  { devPos=pos.toDouble(); emit posIsSet(); }
    void setStep(QString sp)   { devStep=sp.toDouble(); emit stepIsSet(); }
    void setNomSpeed(QString speed) { setNomSpeed(speed.toInt()); emit speedChecked(speed); }


    void Init();
    void Delete();

    void stop();
    void move();

    void getNomVoltageSlot() { emit voltageChecked(QString().setNum(getNomVoltage())); }
    void getNomCurrentSlot()     { emit currentChecked(QString().setNum(getNomCurrent())); }
    void getNomSpeedSlot()       { emit speedChecked(QString().setNum(getNomSpeed())); }
signals:
    void nameIsSet   ();
    void idIsSet     ();
    void posIsSet    ();
    void stepIsSet   ();
    void voltageChecked(QString);
    void currentChecked(QString);
    void speedChecked(QString);

    void deviceIsStopped();

    void deviceMoveStart();
    void deviceMoveEnd  ();
/*
    void deviceIsFound   ();
    void deviceIsNotFound();*/

    void Enabled ();
    void Disabled();

private:
    void makeBaseConnections();

private:
    QString devName="";
    //int     devId=0;
    double  m_devMinPos=0.;
    double  m_devMaxPos=1000.;
    double  devPos=0.;
    double  devStep=0.;

    int m_homePos=0;
    int m_timerId;

    bool devIdIsValid=false;
    bool devPosIsValid=false;
    bool devStepIsValid=false;

    device_t          m_device;
    status_t          m_status;
    engine_settings_t m_engine_settings;
    status_calb_t     m_status_calb;
    calibration_t     m_calibration;
    get_position_t    m_get_position;

    edges_settings_t  m_edges_settings;
};
