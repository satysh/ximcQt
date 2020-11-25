#include <QDebug>

#include "STANDADevice.h"

STANDADevice::STANDADevice(QObject *parent/*=nullptr*/)
    : QObject(parent)
{
    qDebug() << "STANDADevice::STANDADevice";
    emit deviceIsNotAvailable();
}

STANDADevice::~STANDADevice()
{
    Delete();
}

// ------------------- Init Device -----------------------------------

void STANDADevice::Init()
{
    qDebug() << "Init device: "
             << "Name: " << getName() << "\n"
             << "Id: "   << getId() << "\n"
             << "(x,y,z)=(" << getXcor() << ", " << getYcor() << ", " << getZcor() << ")\n"
             << "step: "    << getStep();
    qDebug() << " ----------------- Init End -----------------";

    if (devFoundStatus) emit deviceIsFound();
    else                emit deviceIsNotFound();
}
// ------------------- Delete Device ---------------------------------
void STANDADevice::Delete()
{
    stop();
    qDebug() << "Delete device: "
             << "Name: " << getName() << "\n"
             << "Id: "   << getId() << "\n"
             << "(x,y,z)=(" << getXcor() << "," << getYcor() << "," << getZcor() << ")\n"
             << "step: "    << getStep();
    qDebug() << " ----------------- DeleteDevice End -----------------";
}
// ------------------- Slots -----------------------------------------
void STANDADevice::stop()
{
    qDebug() << "Device "<< getName() << " is stopped! x=" << getXcor()
                                                    << ", y=" << getYcor()
                                                    << ", z=" << getZcor();
    emit deviceIsStopped();
}
void STANDADevice::moveToXcor()
{
    emit deviceMoveStart();
    qDebug() << "Device " << getName() << " is moving to x=" << getXcor();
    emit deviceMoveEnd();
}
void STANDADevice::moveToYcor()
{
    emit deviceMoveStart();
    qDebug() << "Device " << getName() << " is moving to y=" << getYcor();
    emit deviceMoveEnd();

}
void STANDADevice::moveToZcor()
{
    emit deviceMoveStart();
    qDebug() << "Device " << getName() << " is moving to z=" << getZcor();
    emit deviceMoveEnd();
}
void STANDADevice::moveToXYZ ()
{
    emit deviceMoveStart();
    qDebug() << "Device " << getName() << " is moving to x=" << getXcor()
                                                   << ", y=" << getYcor()
                                                   << ", z=" << getZcor();
    emit deviceMoveEnd();
}

void STANDADevice::check()
{
    (devFoundStatus&devIdIsValid&devXIsValid&devYIsValid&devZIsValid&devStepIsValid) ?
        emit deviceIsAvailable() : emit deviceIsNotAvailable();

}
// ------------- Private methods
void STANDADevice::makeBaseConnections()
{

}


// -------------------------------------------------------------------------------------
