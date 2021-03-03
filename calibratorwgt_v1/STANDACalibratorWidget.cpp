#include "STANDACalibratorWidget.h"

#include <ximc.h>

#include <QDebug>
#include <QHBoxLayout>
#include <QTimerEvent>
#include <QSize>


STANDACalibratorWidget::STANDACalibratorWidget(QWidget *parent/* = nullptr*/)
    : QWidget(parent)
{
    qDebug() << "STANDACalibratorWidget::STANDACalibratorWidget";
    startTimer(1);
    int fixedw=900;
    int fixedh=600;
    //setFixedSize(fixedw, fixedh);

    m_pInfoWindow = new QTextEdit;
    m_pInfoWindow->setReadOnly(true);

    m_pmainLayout = new QVBoxLayout;

    FindAvailableDevices();
    MakeDevSelectButtons();
    MakeControlWindow();

    m_pmainLayout->addWidget(m_pInfoWindow);
    setLayout(m_pmainLayout);
}
// ---------- Public slots -----

void STANDACalibratorWidget::trigger(bool flag)
{
    QRadioButton *pcurRbtn = (QRadioButton*)sender();
    QString outStr = pcurRbtn->text();
    if (flag) {
        outStr += " was on";
    }
    else {
        outStr += " was off";
    }

    Print(outStr);
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

void STANDACalibratorWidget::MakeDevSelectButtons()
{
    m_pgbxOfDevs = new QGroupBox("&Devices");
    //connect(m_pgbxOfDevs, SIGNAL(toggled(bool)), this, SLOT(test(bool)));

    QHBoxLayout *phbxLayout = new QHBoxLayout;

    for (int i=0; i<m_ndevs; i++) {
        QRadioButton *pcurRbtn = new QRadioButton(m_devFriendlyNamesList.at(i));
        phbxLayout->addWidget(pcurRbtn);
        connect(pcurRbtn, SIGNAL(toggled(bool)), this, SLOT(trigger(bool)));
    }

    m_pgbxOfDevs->setLayout(phbxLayout);

    m_pmainLayout->addWidget(m_pgbxOfDevs);
}

void STANDACalibratorWidget::MakeControlWindow()
{
    QHBoxLayout *phbxLayoutCW = new QHBoxLayout;

    m_pcurDevNameLable = new QLabel("empty");
    m_pcurDevVoltage = new QLabel("empty");
    m_pcurDevSpeed = new QLabel("empty");
    m_pcurDevAcceleration = new QLabel("empty");
    m_pcurDevPos = new QLabel("empty");

    QVBoxLayout *pvbxLayout1Row = new QVBoxLayout;
    pvbxLayout1Row->addWidget(new QLabel("current"), 0, Qt::AlignCenter);
    pvbxLayout1Row->addWidget(m_pcurDevNameLable);
    pvbxLayout1Row->addWidget(m_pcurDevVoltage);
    pvbxLayout1Row->addWidget(m_pcurDevSpeed);
    pvbxLayout1Row->addWidget(m_pcurDevAcceleration);
    pvbxLayout1Row->addWidget(m_pcurDevPos);

    phbxLayoutCW->addLayout(pvbxLayout1Row);

    m_pDevNameEdit = new QLineEdit;
    m_pDevVoltageEdit = new QLineEdit;
    m_pDevSpeedEdit = new QLineEdit;
    m_pDevAccelerationEdit = new QLineEdit;

    QVBoxLayout *pvbxLayout2Row = new QVBoxLayout;
    pvbxLayout2Row->addWidget(new QLabel("set"), 0, Qt::AlignCenter);
    pvbxLayout2Row->addWidget(m_pDevNameEdit);
    pvbxLayout2Row->addWidget(m_pDevVoltageEdit);
    pvbxLayout2Row->addWidget(m_pDevSpeedEdit);
    pvbxLayout2Row->addWidget(m_pDevAccelerationEdit);



    m_pcmdDevSetZeroPos = new QPushButton("setZeroPos");
    m_pcmdDevSetMaxPos = new QPushButton("setMaxPos");

    QHBoxLayout *phbxLayout = new QHBoxLayout;
    phbxLayout->addWidget(m_pcmdDevSetZeroPos);
    phbxLayout->addWidget(m_pcmdDevSetMaxPos);
    pvbxLayout2Row->addLayout(phbxLayout);

    phbxLayoutCW->addLayout(pvbxLayout2Row);

    m_pDevNameOk = new QPushButton("ok");
    m_pDevVoltageOk = new QPushButton("ok");
    m_pDevSpeedOk = new QPushButton("ok");
    m_pDevAccelerationOk = new QPushButton("ok");

    QVBoxLayout *pvbxLayout3Row = new QVBoxLayout;
    pvbxLayout3Row->addWidget(new QLabel("ok"), 0, Qt::AlignCenter);
    pvbxLayout3Row->addWidget(m_pDevNameOk);
    pvbxLayout3Row->addWidget(m_pDevVoltageOk);
    pvbxLayout3Row->addWidget(m_pDevSpeedOk);
    pvbxLayout3Row->addWidget(m_pDevAccelerationOk);
    pvbxLayout3Row->addWidget(new QLabel(""));

    phbxLayoutCW->addLayout(pvbxLayout3Row);

    m_pDevLeft = new QPushButton("left");
    m_pDevRight = new QPushButton("right");

    QHBoxLayout *phbxLayoutLR = new QHBoxLayout;
    phbxLayoutLR->addWidget(m_pDevLeft);
    phbxLayoutLR->addWidget(m_pDevRight);

    m_pDevHome = new QPushButton("home");
    m_pDevStop = new QPushButton("stop");

    m_pDevRenameMe1 = new QRadioButton("on/off"); // TODO
    m_pDevRenameMe2 = new QRadioButton("on/off"); // TODO

    QVBoxLayout *pvbxLayout4Row = new QVBoxLayout;
    pvbxLayout4Row->addWidget(new QLabel("move"), 0, Qt::AlignCenter);
    pvbxLayout4Row->addLayout(phbxLayoutLR);
    pvbxLayout4Row->addWidget(m_pDevHome);
    pvbxLayout4Row->addWidget(m_pDevStop);
    pvbxLayout4Row->addWidget(m_pDevRenameMe1);
    pvbxLayout4Row->addWidget(m_pDevRenameMe2);

    phbxLayoutCW->addLayout(pvbxLayout4Row);

    m_pmainLayout->addLayout(phbxLayoutCW);
}

void STANDACalibratorWidget::timerEvent(QTimerEvent*)
{
/*
    m_pDevNameEdit->resize(m_pDevHome->size().width(), m_pDevHome->size().height());
    m_pDevVoltageEdit->resize(m_pDevHome->size().width(), m_pDevHome->size().height());
    m_pDevSpeedEdit->resize(m_pDevHome->size().width(), m_pDevHome->size().height());
    m_pDevAccelerationEdit->resize(m_pDevHome->size().width(), m_pDevHome->size().height());
    m_pcmdDevSetZeroPos->resize(m_pDevHome->size().width(), m_pDevHome->size().height());
    m_pcmdDevSetMaxPos->resize(m_pDevHome->size().width(), m_pDevHome->size().height());*/
}
