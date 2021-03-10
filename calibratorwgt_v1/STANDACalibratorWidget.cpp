#include "STANDACalibratorWidget.h"

#include <ximc.h>

#include <QDebug>
#include <QHBoxLayout>
#include <QTimerEvent>
#include <QIntValidator>
#include <QFile>
#include <QTextStream>

STANDACalibratorWidget::STANDACalibratorWidget(QWidget *parent/* = nullptr*/)
    : QWidget(parent)
{
    qDebug() << "STANDACalibratorWidget::STANDACalibratorWidget";
    startTimer(1);
    m_device = new STANDADevice;
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

    ConnectDeviceAndCW();
}
// ---------- Public slots -----

void STANDACalibratorWidget::trigger(bool flag)
{
    QRadioButton *pcurRbtn = (QRadioButton*)sender();
    QString curDevFriendlyName = pcurRbtn->text();
    QString curDevName="empty";
    for (int i=0; i<m_ndevs; i++) {
        if (m_devFriendlyNamesList.at(i) == curDevFriendlyName) {
            curDevName = m_devNamesList.at(i);
            break;
        }
    }
    QString outStr = curDevFriendlyName;
    if (flag) {
        outStr += " was on";
        m_device->setName(curDevName);
        m_device->Init();
        m_curDevVoltage=0;
        m_curDevSpeeds=0;
        m_curDevZeroPos=0;
        m_curDevMaxPos=0;
        m_curDevDecel=0;
    }
    else {
        outStr += " was off";
        m_device->Close();

        m_mapOfDevVoltages[curDevName] = m_curDevVoltage;
        m_mapOfDevSpeeds[curDevName] = m_curDevSpeeds;
        m_mapOfDevZeroPoses[curDevName] = m_curDevZeroPos;
        m_mapOfDevMaxPoses[curDevName] = m_curDevMaxPos;
        m_mapOfDevDecel[curDevName] = m_curDevDecel;
    }

    Print(outStr);
}

// -----------------------------
STANDACalibratorWidget::~STANDACalibratorWidget()
{
    // Write Last device calibration Info
    QString curDevName = m_pcurDevNameLable->text();
    m_mapOfDevVoltages[curDevName] = m_curDevVoltage;
    m_mapOfDevSpeeds[curDevName] = m_curDevSpeeds;
    m_mapOfDevZeroPoses[curDevName] = m_curDevZeroPos;
    m_mapOfDevMaxPoses[curDevName] = m_curDevMaxPos;
    m_mapOfDevDecel[curDevName] = m_curDevDecel;
    WriteOutputTxt();
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
    //m_ndevs = 10; // TODO It exits just for Tests
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
            //curDevName = "device_"+QString().setNum(i); // TODO It exits just for Tests
            m_devNamesList << curDevName;
        }

        for (int i=0; i<m_ndevs; i++) {
            m_device->setName(m_devNamesList.at(i));
            m_device->Init();
            QString curDevFriendlyName = m_device->getStageName();
            //curDevFriendlyName = "fr_device_"+QString().setNum(i); // TODO It exits just for Tests
            m_devFriendlyNamesList << curDevFriendlyName;
            m_device->Close();
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

    QVBoxLayout *pvbxLayout0Row = new QVBoxLayout;
    pvbxLayout0Row->addWidget(new QLabel(""), 0, Qt::AlignCenter);
    pvbxLayout0Row->addWidget(new QLabel("devName:"), 0, Qt::AlignLeft);
    pvbxLayout0Row->addWidget(new QLabel("devVoltage:"), 0, Qt::AlignLeft);
    pvbxLayout0Row->addWidget(new QLabel("devSpeed:"), 0, Qt::AlignLeft);
    pvbxLayout0Row->addWidget(new QLabel("devDecel:"), 0, Qt::AlignLeft);
    pvbxLayout0Row->addWidget(new QLabel("devPos:"), 0, Qt::AlignLeft);

    phbxLayoutCW->addLayout(pvbxLayout0Row);

    m_pcurDevNameLable = new QLabel("empty");
    m_pcurDevVoltage = new QLabel("empty");
    m_pcurDevSpeed = new QLabel("empty");
    m_pcurDevDeceleration = new QLabel("empty");
    m_pcurDevPos = new QLabel("empty");

    QVBoxLayout *pvbxLayout1Row = new QVBoxLayout;
    pvbxLayout1Row->addWidget(new QLabel("current"), 0, Qt::AlignCenter);
    pvbxLayout1Row->addWidget(m_pcurDevNameLable);
    pvbxLayout1Row->addWidget(m_pcurDevVoltage);
    pvbxLayout1Row->addWidget(m_pcurDevSpeed);
    pvbxLayout1Row->addWidget(m_pcurDevDeceleration);
    pvbxLayout1Row->addWidget(m_pcurDevPos);

    phbxLayoutCW->addLayout(pvbxLayout1Row);

    m_pDevNameEdit = new QLineEdit;
    m_pDevVoltageEdit = new QLineEdit;
    m_pDevSpeedEdit = new QLineEdit;
    m_pDevDecelerationEdit = new QLineEdit;

    m_pDevVoltageEdit->setValidator(new QIntValidator(0, 1500));
    m_pDevSpeedEdit->setValidator(new QIntValidator(0, 1500));
    m_pDevDecelerationEdit->setValidator(new QIntValidator(0, 1500));

    QVBoxLayout *pvbxLayout2Row = new QVBoxLayout;
    pvbxLayout2Row->addWidget(new QLabel("set"), 0, Qt::AlignCenter);
    pvbxLayout2Row->addWidget(m_pDevNameEdit);
    pvbxLayout2Row->addWidget(m_pDevVoltageEdit);
    pvbxLayout2Row->addWidget(m_pDevSpeedEdit);
    pvbxLayout2Row->addWidget(m_pDevDecelerationEdit);



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
    m_pDevDecelerationOk = new QPushButton("ok");

    QVBoxLayout *pvbxLayout3Row = new QVBoxLayout;
    pvbxLayout3Row->addWidget(new QLabel("ok"), 0, Qt::AlignCenter);
    pvbxLayout3Row->addWidget(m_pDevNameOk);
    pvbxLayout3Row->addWidget(m_pDevVoltageOk);
    pvbxLayout3Row->addWidget(m_pDevSpeedOk);
    pvbxLayout3Row->addWidget(m_pDevDecelerationOk);
    pvbxLayout3Row->addWidget(new QLabel(""));

    phbxLayoutCW->addLayout(pvbxLayout3Row);

    m_pDevLeft = new QPushButton("left");
    m_pDevRight = new QPushButton("right");

    QHBoxLayout *phbxLayoutLR = new QHBoxLayout;
    phbxLayoutLR->addWidget(m_pDevLeft);
    phbxLayoutLR->addWidget(m_pDevRight);

    m_pDevHome = new QPushButton("home");
    m_pDevStop = new QPushButton("stop");

    m_pDevRenameMe1 = new QRadioButton("on/off"); // TODO rename title
    m_pDevRenameMe2 = new QRadioButton("on/off"); // TODO rename title

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

void STANDACalibratorWidget::ConnectDeviceAndCW()
{
    connect(m_pDevLeft, SIGNAL(pressed()), m_device, SLOT(left()));
    connect(m_pDevLeft, SIGNAL(released()), m_device, SLOT(stop()));
    connect(m_pDevRight, SIGNAL(pressed()), m_device, SLOT(right()));
    connect(m_pDevRight, SIGNAL(released()), m_device, SLOT(stop()));
    connect(m_pDevHome, SIGNAL(clicked()), m_device, SLOT(home()));
    connect(m_pDevStop, SIGNAL(clicked()), m_device, SLOT(stop()));

    connect(m_pDevNameOk, SIGNAL(clicked()), this, SLOT(setStageName()));
    connect(m_pDevVoltageOk, SIGNAL(clicked()),this, SLOT(setDevVoltage()));
    connect(m_pDevSpeedOk, SIGNAL(clicked()),this, SLOT(setDevSpeed()));
    connect(m_pcmdDevSetZeroPos, SIGNAL(clicked()),this, SLOT(setDevZeroPos()));
    connect(m_pcmdDevSetMaxPos, SIGNAL(clicked()),this, SLOT(setDevMaxPos()));
    connect(m_pDevDecelerationOk, SIGNAL(clicked()),this, SLOT(setDevDecel()));
}

void STANDACalibratorWidget::WriteOutputTxt()
{
    QFile fileOut("../settingsdata/fileout.txt");
    if(fileOut.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream writeStream(&fileOut);
        for (int i=0; i<m_ndevs; i++) {
            writeStream << m_devNamesList.at(i) << "\t";
            writeStream << m_mapOfDevVoltages[m_devNamesList.at(i)] << "\t";
            writeStream << m_mapOfDevSpeeds[m_devNamesList.at(i)] << "\t";
            writeStream << m_mapOfDevZeroPoses[m_devNamesList.at(i)] << "\t";
            writeStream << m_mapOfDevMaxPoses[m_devNamesList.at(i)] << "\t";
            writeStream << m_mapOfDevDecel[m_devNamesList.at(i)] << "\n";
        }
    }
    fileOut.close();
}

void STANDACalibratorWidget::timerEvent(QTimerEvent*)
{
/*
    m_pDevNameEdit->resize(m_pDevHome->size().width(), m_pDevHome->size().height());
    m_pDevVoltageEdit->resize(m_pDevHome->size().width(), m_pDevHome->size().height());
    m_pDevSpeedEdit->resize(m_pDevHome->size().width(), m_pDevHome->size().height());
    m_pDevDecelerationEdit->resize(m_pDevHome->size().width(), m_pDevHome->size().height());
    m_pcmdDevSetZeroPos->resize(m_pDevHome->size().width(), m_pDevHome->size().height());
    m_pcmdDevSetMaxPos->resize(m_pDevHome->size().width(), m_pDevHome->size().height());*/

    m_pcurDevNameLable->setText(m_device->getName());
    m_pcurDevVoltage->setText(QString().setNum(m_device->getCurVoltage()));
    m_pcurDevSpeed->setText(QString().setNum(m_device->getCurSpeed()));
    m_pcurDevDeceleration->setText(QString().setNum(m_device->getCurDeceleration()));
    m_pcurDevPos->setText(QString().setNum(m_device->getCurOwnPosition()));
}
