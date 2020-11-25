#include <QDebug>

#include "STANDASetup.h"

STANDASetup::STANDASetup(QObject *parent/*=nullptr*/)
    : QObject(parent)
{
    qDebug() << "STANDASetup::STANDASetup";
}

STANDASetup::~STANDASetup()
{
    DeleteDevice();
}

// ------------------- Init Device -----------------------------------

void STANDASetup::InitDevice()
{
    qDebug() << "Init device: "
             << "DevName: " << getDevName() << "\n"
             << "DevId: "   << getDevId() << "\n"
             << "(x,y,z)=(" << getXcor() << ", " << getYcor() << ", " << getZcor() << ")\n"
             << "step: "    << getStep();
    qDebug() << " ----------------- Init End -----------------";
}
// ------------------- Delete Device ---------------------------------
void STANDASetup::DeleteDevice()
{
    stopDevice();
    qDebug() << "Delete device: "
             << "DevName: " << getDevName() << "\n"
             << "DevId: "   << getDevId() << "\n"
             << "(x,y,z)=(" << getXcor() << "," << getYcor() << "," << getZcor() << ")\n"
             << "step: "    << getStep();
    qDebug() << " ----------------- DeleteDevice End -----------------";
}
// ------------------- Slots -----------------------------------------
void STANDASetup::stopDevice()
{
    qDebug() << "Device is "<< getDevName() << " is stoped! x=" << getXcor()
                                            << ", y=" << getYcor()
                                            << ", z=" << getZcor();
    emit deviceIsStopped();
}
void STANDASetup::moveToXcor()
{
    emit deviceMoveStart();
    qDebug() << "Device is moving to x=" << getXcor();
    emit deviceMoveEnd();
}
void STANDASetup::moveToYcor()
{
    emit deviceMoveStart();
    qDebug() << "Device is moving to y=" << getYcor();
    emit deviceMoveEnd();

}
void STANDASetup::moveToZcor()
{
    emit deviceMoveStart();
    qDebug() << "Device is moving to z=" << getZcor();
    emit deviceMoveEnd();
}
void STANDASetup::moveToXYZ ()
{
    emit deviceMoveStart();
    qDebug() << "Device is moving to x=" << getXcor()
                               << ", y=" << getYcor()
                               << ", z=" << getZcor();
    emit deviceMoveEnd();
}
