#pragma once

#include <ximc.h>

#include <QObject>
#include <QString>

class STANDADevice : public QObject
{
    Q_OBJECT
public:
    STANDADevice(QObject *parent = nullptr);
    STANDADevice(QString name, QObject *parent = nullptr);
    ~STANDADevice();

    void setHomePos(int pos) { m_homePos=pos; }


    int getHomePos()  { return m_homePos; }
    QString getName() { return devName; }
    int     getId  () { return m_device; }
    double  getPos () { return devPos; }
    double  getStep() { return devStep; }

public:
    void setStageName(QString name);
    void setFriendlyName(QString name);
    void setNomVoltage(int voltage);
    void setNomSpeed(int speed);
    void setDevAccel(int accel);
    void setDevDecel(int decel);
    void setSW1ToRight();
    void setSW1ToLeft();
    void setSW1LOW(bool flag);
    void setSW2LOW(bool flag);


    QString getStageName();
    QString getFriendlyName();
    int getNomVoltage();
    int getNomCurrent();
    int getNomSpeed();

    int getCurVoltage();
    int getCurSpeed();
    int getCurAcceleration();
    int getCurDeceleration();
    int getCurOwnPosition();

    void moveTo(int pos);

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
    void Close();

    void stop ();
    void sstp (); // Soft stop engine
    void move ();
    void moveTo(QString);
    void left ();
    void right();
    void home();

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
    double  devPos=0.; // TODO Is it necessary?
    double  devStep=0.; // TODO Is it necessary?

    int m_homePos=0;
    int m_timerId;

    bool devIdIsValid=false;
    bool devPosIsValid=false;
    bool devStepIsValid=false;

    device_t          m_device;
    stage_name_t      m_stage_name;
    controller_name_t m_controller_name;
    status_t          m_status;
    engine_settings_t m_engine_settings;
    move_settings_t   m_move_settings;
    status_calb_t     m_status_calb;
    calibration_t     m_calibration;
    get_position_t    m_get_position;

    edges_settings_t  m_edges_settings;
};
