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
    qDebug() << "Init device: \n"
             << "Name: " << getName() << "\n"
             << "Id:   "   << getId() << "\n"
             << "pos:  "  << getPos() << "\n"
             << "step: "    << getStep();
    qDebug() << " ----------------- Init End -----------------";

    if (devFoundStatus) emit deviceIsFound();
    else                emit deviceIsNotFound();
}
// ------------------- Delete Device ---------------------------------
void STANDADevice::Delete()
{
    stop();
    qDebug() << "Delete device: \n"
             << "Name: " << getName() << "\n"
             << "Id:   "   << getId() << "\n"
             << "pos:  "  << getPos() << "\n"
             << "step: "    << getStep();
    qDebug() << " ----------------- DeleteDevice End -----------------";
}
// ------------------- Slots -----------------------------------------
void STANDADevice::stop()
{
    qDebug() << "Device "<< getName() << " is stopped! pos: "  << getPos() << "\n";
    emit deviceIsStopped();
}
void STANDADevice::move()
{
    emit deviceMoveStart();
    qDebug() << "Device " << getName() << " is moving to pos: "  << getPos() << "\n";
    emit deviceMoveEnd();
}

void STANDADevice::check()
{
    (devFoundStatus&devIdIsValid&devPosIsValid&devStepIsValid) ?
        emit deviceIsAvailable() : emit deviceIsNotAvailable();

}
// ------------- Private methods
void STANDADevice::makeBaseConnections()
{

}


// -------------------------------------------------------------------------------------
