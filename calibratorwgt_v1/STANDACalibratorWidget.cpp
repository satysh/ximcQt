#include "STANDACalibratorWidget.h"

#include <ximc.h>

#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTimerEvent>
#include <QRadioButton>


STANDACalibratorWidget::STANDACalibratorWidget(QWidget *parent/* = nullptr*/)
    : QWidget(parent)
{
    qDebug() << "STANDACalibratorWidget::STANDACalibratorWidget";
    int fixedw=500;
    int fixedh=500;
    //setFixedSize(fixedw, fixedh);

    m_pInfoWindow = new QTextEdit;
    m_pInfoWindow->setReadOnly(true);

    FindAvailableDevices();
    MakeDevButtons();

    QVBoxLayout *pmainLayout = new QVBoxLayout;
    pmainLayout->addWidget(m_pgbxOfDevs);
    pmainLayout->addWidget(m_pInfoWindow);
    setLayout(pmainLayout);
}
// ---------- Public slots -----

void STANDACalibratorWidget::test()
{
    Print("toggled");
}

// -----------------------------
STANDACalibratorWidget::~STANDACalibratorWidget()
{
}

void STANDACalibratorWidget::Print(QString str)
{
    static int iStr = 1;
    if (iStr > 1) {
        QString outStr  = m_pInfoWindow->toPlainText() + '\n' + QString().setNum(iStr) + ": " + str;
        m_pInfoWindow->setText(outStr);
    }
    else {
        m_pInfoWindow->setText(QString().setNum(iStr)+": "+str);
    }
    iStr++;
}

void STANDACalibratorWidget::FindAvailableDevices()
{
    const int probe_flags = ENUMERATE_PROBE;
    const char* enumerate_hints = "";

    char ximc_version_str[32];
    device_enumeration_t devenum;

//  ximc_version returns library version string.
    ximc_version(ximc_version_str);
    qDebug() << "libximc version " << ximc_version_str;
    Print(QString("libximc version ")+QString(ximc_version_str));

//  Set bindy (network) keyfile. Must be called before any call to "enumerate_devices" or "open_device" if you
//  wish to use network-attached controllers. Accepts both absolute and relative paths, relative paths are resolved
//  relative to the process working directory. If you do not need network devices then "set_bindy_key" is optional.
    set_bindy_key("keyfile.sqlite");

//  Device enumeration function. Returns an opaque pointer to device enumeration data.
    devenum = enumerate_devices(probe_flags, enumerate_hints);

//  Gets device count from device enumeration data
    m_ndevs = get_device_count(devenum);
    m_ndevs = 10;
    //setndevs(2); // TODO It exists just for test
//  Terminate if there are no connected devices
    if (m_ndevs <= 0)
    {
        qDebug() << "No devices found";
        Print(QString("No devices found"));
    //  Free memory used by device enumeration data
        free_enumerate_devices(devenum);
    }
    else {
        for (int i=0; i<m_ndevs; i++) {
            QString curDevName(get_device_name(devenum, i));
            curDevName = "device_"+QString().setNum(i); // TODO It exits just for Tests
            m_devNamesList << curDevName;
            m_devFriendlyNamesList << curDevName;
        }


        qDebug() << m_ndevs << " devices were found:";
        Print(QString().setNum(m_ndevs)+QString( " devices were found:"));
        for (int i=0; i<m_ndevs; i++) {
            qDebug() << " " << i+1 << ": " << m_devNamesList.at(i);
            Print(QString(" ")+QString().setNum(i+1)+QString(": ")+m_devNamesList.at(i));
        }
        //Free memory used by device enumeration data
        free_enumerate_devices(devenum);
    }
}

void STANDACalibratorWidget::MakeDevButtons()
{
    m_pgbxOfDevs = new QGroupBox("&Devices");

    QHBoxLayout *phbxLayout = new QHBoxLayout;

    for (int i=0; i<m_ndevs; i++) {
        QRadioButton *pcurRbtn = new QRadioButton( QString("&")+m_devFriendlyNamesList.at(i) );
        phbxLayout->addWidget(pcurRbtn);
    }

    m_pgbxOfDevs->setLayout(phbxLayout);
}

void STANDACalibratorWidget::timerEvent(QTimerEvent*)
{
}
