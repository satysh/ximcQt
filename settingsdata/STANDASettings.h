#pragma once

#include <QObject>
#include <QString>

class STANDASettings : public QObject
{
    Q_OBJECT
public:
    STANDASettings(QObject *parent = nullptr);
    ~STANDASettings();

    void setDeviceName  (QString name) { m_deviceName=name; }
    void setFriendlyName(QString name) { m_friendlyName=name; }
    void setNomVoltage  (int vol)      { m_nomVoltage=vol; }
    void setNomSpeed    (int speed)    { m_nomSpeed=speed; }
    void setAccel       (int accel)    { m_nomAccel=accel; }
    void setDecel       (int decel)    { m_nomDecel=decel; }
    void setZeroPosition(int pos)      { m_zeroPosition=pos; }
    void setMaxPosition (int pos)      { m_maxPosition=pos; }


    QString getDeviceName  () { return m_deviceName; }
    QString getFriendlyName() { return m_friendlyName; }
    int getNomVoltage      () { return m_nomVoltage; }
    int getNomSpeed        () { return m_nomSpeed; }
    int getAccel           () { return m_nomAccel; }
    int getDecel           () { return m_nomDecel; }
    int getZeroPosition    () { return m_zeroPosition; }
    int getMaxPosition     () { return m_maxPosition; }

private:
    QString m_deviceName="";
    QString m_friendlyName="";
    int     m_nomVoltage=0;
    int     m_nomSpeed=0;
    int     m_nomAccel=0;
    int     m_nomDecel=0;
    int     m_zeroPosition=0;
    int     m_maxPosition=0;
};
