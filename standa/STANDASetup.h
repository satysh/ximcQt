#pragma once

#include <QObject>
class QString;

class STANDASetup : public QObject
{
    Q_OBJECT
public:
    STANDASetup(QObject *parent = nullptr);
    ~STANDASetup();

    void setDevId(int idev)  { devId=idev; }
    void setXcor (double xc) { xcor=xc; }
    void setYcor (double yc) { ycor=yc; }
    void setZcor (double zc) { zcor=zc; }
    void setDevName(QString name) {deviceName=name; }

    void loadDevice(QString devName="");
    void moveX     (QString devName, double xcor);
    void moveY     (QString devName, double ycor);
    void moveZ     (QString devName, double zcor);
    void move      (QString devName, double xcor, double ycor, double zcor);


public slots:
    void loadDeviceSlot       ();
    void moveXSlot            ();
    void moveYSlot            ();
    void moveZSlot            ();
    void moveSlot             ();
    void testSignlasSlot      ();

signals:
    void deviceLoadStatus(bool);
    void deviceMoveX     (bool);
    void deviceMoveY     (bool);
    void deviceMoveZ     (bool);
    void deviceNotfound  (    );

private:
    void connectDefault();

private:
    int    devId=0;
    int    ndevs=0;
    double xcor=0.;
    double ycor=0.;
    double zcor=0.;
    double step=0.;
    QString deviceName="";
};
