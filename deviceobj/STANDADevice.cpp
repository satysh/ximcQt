#include <stdio.h>

#include <QDebug>
#include <QTimerEvent>

#include "STANDADevice.h"

STANDADevice::STANDADevice(QObject *parent/*=nullptr*/)
    : QObject(parent)
{
    qDebug() << "STANDADevice::STANDADevice";
    if (getName() == "") emit Disabled();
}

STANDADevice::~STANDADevice()
{
    Delete();
}

void STANDADevice::setNomSpeed(int speed)
{
    m_engine_settings.NomSpeed = speed;
    set_engine_settings( m_device, &m_engine_settings );
}

int STANDADevice::getNomVoltage()
{
    get_engine_settings( m_device, &m_engine_settings );
    int nomVoltage = m_engine_settings.NomVoltage;
    emit voltageChecked(QString().setNum(nomVoltage));
    return nomVoltage;
}

int STANDADevice::getNomCurrent()
{
    get_engine_settings( m_device, &m_engine_settings );
    int nomCurrent = m_engine_settings.NomCurrent;
    emit currentChecked(QString().setNum(nomCurrent));
    return nomCurrent;
}

int STANDADevice::getNomSpeed()
{
    get_engine_settings( m_device, &m_engine_settings );
    int nomSpeed = m_engine_settings.NomSpeed;
    emit speedChecked(QString().setNum(nomSpeed));
    return nomSpeed;
}

void STANDADevice::moveToBasePos()
{
    //get_status( m_device, &m_status );
    //command_move ( m_device, m_status.CurPosition+180, 0);
}

void STANDADevice::timerEvent(QTimerEvent* ptev)
{
    get_status( m_device, &m_status );
    //qDebug() << "QTimerEvent speed = " << m_status.CurSpeed;
    if (m_status.CurSpeed == 0) {
        killTimer(ptev->timerId());
        emit deviceMoveEnd();
    }
}
// ------------------- Init Device -----------------------------------

void STANDADevice::Init()
{
    qDebug() << "Init device: \n"
             << "Name: " << getName() << "\n"
             << "Id:   "   << getId() << "\n"
             << "pos:  "  << getPos() << "\n"
             << "step: "    << getStep();

    char device_name[256];
    QByteArray ba = getName().toLocal8Bit();
    const char *c_str = ba.data();
    strcpy(device_name, c_str);
    m_device = open_device(device_name);
    emit Enabled();
    qDebug() << "[TEST]: char device name is " << device_name;

    printf( "Getting status parameters: " );
//  Read device status from a device
    get_status( m_device, &m_status );
    printf( "position %d, encoder %lld, speed %d\n", m_status.CurPosition, m_status.EncPosition, m_status.CurSpeed );
    setHomePos(m_status.CurPosition);

    printf( "Getting engine parameters: " );
//  Read engine settings from a device
    get_engine_settings( m_device, &m_engine_settings );
    printf( "voltage %d, current %d, speed %d\n", m_engine_settings.NomVoltage, m_engine_settings.NomCurrent, m_engine_settings.NomSpeed );
    m_engine_settings.NomSpeed=1500;
    set_engine_settings( m_device, &m_engine_settings );
    const char* units = "mm";

    printf( "Getting calibrated parameters: " );
//  Setting calibration constant to 0.1 (one controller step equals this many units)
    m_calibration.A = 0.1;
//  We have to set microstep mode to convert microsteps to calibrated units correctly
    m_calibration.MicrostepMode = m_engine_settings.MicrostepMode;
    //Read calibrated device status from a device
    get_status_calb( m_device, &m_status_calb, &m_calibration);
    printf( "calibrated position %.3f %s, calibrated speed %.3f %s/s\n", m_status_calb.CurPosition, units, m_status_calb.CurSpeed, units );

    printf("Getting edges settings: ");
    get_edges_settings(m_device, &m_edges_settings);
    printf( "lB %d, ulb %d, rB %d, urB\n", m_edges_settings.LeftBorder, m_edges_settings.uLeftBorder
                                         , m_edges_settings.RightBorder, m_edges_settings.uRightBorder
          );

    moveToBasePos();
    qDebug() << " ----------------- Init End -----------------";
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
    command_home(m_device);
    close_device( &m_device );
    qDebug() << " ----------------- DeleteDevice End -----------------";
}
// ------------------- Slots -----------------------------------------
void STANDADevice::stop()
{
    qDebug() << "Device "<< getName() << " is stopped! pos: "  << getPos() << "\n";
    command_stop( m_device );
    emit deviceIsStopped();
}
void STANDADevice::move()
{
    startTimer(1000);
    emit deviceMoveStart();
    qDebug() << "Device " << getName() << " is moving to pos: "  << getPos() << "\n";
    double Position = (double)getHomePos() + getPos();
    command_move ( m_device, (int)Position, 0);
}

void STANDADevice::left()
{
    command_left(m_device);
}

void STANDADevice::right()
{
    command_right(m_device);
}
// ------------------------------------------------------------------
// ------------- Private methods
void STANDADevice::makeBaseConnections()
{

}

bool STANDADevice::check()
{
    if (devIdIsValid && devPosIsValid && devStepIsValid) {
        emit Enabled();
        return true;
    }
    else {
        emit Disabled();
        return false;
    }
}

// -------------------------------------------------------------------------------------
