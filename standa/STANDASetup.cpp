#include <QDebug>
#include <QStringList>

#include "STANDASetup.h"

STANDASetup::STANDASetup(QObject *parent/*=nullptr*/)
    : QObject(parent)
{
    qDebug() << "STANDASetup::STANDASetup";
}

STANDASetup::~STANDASetup()
{
}


void STANDASetup::loadDevice(QString devName/*=""*/)
{
    if (devName != "") {
        qDebug() << "Device " << devName << " load processing...";
        emit deviceLoadStatus(true);
    }
    else {
        qDebug() << "Device is not found!";
        emit deviceLoadStatus(false);
        emit deviceNotfound();
    }
}
void STANDASetup::moveX(QString devName, double xcor)
{
    if (devName != "") {
        qDebug() << "Device " << devName << " move to X=" << xcor << " processing...";
        emit deviceMoveX(true);
    }
    else {
        qDebug() << "Can't move x. Device is not found!";
        emit deviceMoveX(false);
    }

}
void STANDASetup::moveY     (QString devName, double ycor)
{
    if (devName != "") {
        qDebug() << "Device " << devName << " move to Y=" << ycor << " processing...";
        emit deviceMoveY(true);
    }
    else {
        qDebug() << "Can't move y. Device is not found!";
        emit deviceMoveY(false);
    }
}
void STANDASetup::moveZ     (QString devName, double zcor)
{
    if (devName != "") {
        qDebug() << "Device " << devName << " move to Z=" << zcor << " processing...";
        emit deviceMoveZ(true);
    }
    else {
        qDebug() << "Can't move z. Device is not found!";
        emit deviceMoveZ(false);
    }
}
void STANDASetup::move      (QString devName, double xcor, double ycor, double zcor)
{
    moveX(devName, xcor);
    moveY(devName, ycor);
    moveZ(devName, zcor);
}
// ----------------- Slots -----------------------------------------------------------------
void STANDASetup::loadDeviceSlot()
{
    loadDevice(deviceName);
}
void STANDASetup::moveXSlot()
{
    moveX(deviceName, xcor);
}
void STANDASetup::moveYSlot()
{
    moveY(deviceName, ycor);
}
void STANDASetup::moveZSlot()
{
    moveY(deviceName, zcor);
}
void STANDASetup::moveSlot()
{
    move(deviceName, xcor, ycor, zcor);
}
void STANDASetup::testSignlasSlot()
{
    qDebug() << "signal...";
}
// -----------------------------------------------------------------------------------------

void STANDASetup::connectDefault()
{
   connect(this, SIGNAL(deviceNotfound()),  SLOT(testSignlas()));
   connect(this, SIGNAL(deviceMoveX(bool)), SLOT(testSignlas()));
   connect(this, SIGNAL(deviceMoveY(bool)), SLOT(testSignlas()));
   connect(this, SIGNAL(deviceMoveZ(bool)), SLOT(testSignlas()));
}
