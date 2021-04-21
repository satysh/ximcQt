#include "STANDACalibratorWidget.h"

#include <ximc.h>

#include <QDebug>
#include <QHBoxLayout>
#include <QTimerEvent>
#include <QIntValidator>
#include <QFile>
#include <QTextStream>
#include <QListWidgetItem>

STANDACalibratorWidget::STANDACalibratorWidget(QWidget *parent/* = nullptr*/)
    : QWidget(parent)
{
    qDebug() << "STANDACalibratorWidget::STANDACalibratorWidget";
    startTimer(1);
    m_device = new STANDADevice;
    /*int fixedw=700;
    int fixedh=600;
    setFixedSize(fixedw, fixedh);*/

    m_pInfoWindow = new QTextEdit;
    m_pInfoWindow->setReadOnly(true);

    m_pmainLayout = new QVBoxLayout;

    FindAvailableDevices();
    CheckOutputTxt();
    MakeDevSelectButtons();
    MakeControlWindow();

    m_pmainLayout->addWidget(m_pInfoWindow);
    setLayout(m_pmainLayout);

    ConnectDeviceAndCW();
}
STANDACalibratorWidget::~STANDACalibratorWidget()
{
    qDebug() << "STANDACalibratorWidget::~STANDACalibratorWidget()";
    m_device->Close();
    WriteOutputTxt();
}
// ---------- Public slots -----

void STANDACalibratorWidget::trigger(bool flag)
{
    QRadioButton *pcurRbtn = (QRadioButton*)sender();
    QString curDevFriendlyName = pcurRbtn->text();
    QString curDevName;
    for (int i=0; i<m_ndevs; i++) {
        if (curDevFriendlyName == m_mapOfSettings[m_devNamesList.at(i)].getFriendlyName()) {
            curDevName = m_devNamesList.at(i);
            break;
        }
    }
    QString outStr = curDevFriendlyName;
    if (flag) {
        outStr += " was on";
        m_device->setName(curDevName);
        m_device->Init();
        m_pDevNameEdit->setText(m_device->getFriendlyName());
        m_pDevVoltageEdit->setText(QString().setNum(m_device->getNomVoltage()));
        m_pDevSpeedEdit->setText(QString().setNum(m_device->getNomSpeed()));
        m_pDevDecelerationEdit->setText(QString().setNum(m_device->getCurDeceleration()));
        /*
        m_curFriendlyName = m_device->getFriendlyName();
        m_curDevVoltage = m_device->getNomVoltage();
        m_curDevSpeed = m_device->getNomSpeed();
        m_curDevAccel = m_device->getCurAcceleration();
        m_curDevDecel = m_device->getCurDeceleration();*/
    }
    else {
        outStr += " was off";
        m_device->Close();
        //saveCurrentDevParams(curDevName);
    }

    Print(outStr);
}

// -----------------------------
// --- Private Slots -----------
void STANDACalibratorWidget::setFriendlyName()
{
    QString curDevFriendlyName=m_pDevNameEdit->text();
    m_device->setFriendlyName(curDevFriendlyName);
    m_mapOfSettings[m_device->getName()].setFriendlyName(curDevFriendlyName);
}
void STANDACalibratorWidget::setNomVoltage()
{
    int curNomVoltage = (int)m_pDevVoltageEdit->text().toDouble();
    m_device->setNomVoltage(curNomVoltage);
    m_mapOfSettings[m_device->getName()].setNomVoltage(curNomVoltage);
}
void STANDACalibratorWidget::setNomSpeed()
{
    int curNomSpeed = (int)m_pDevSpeedEdit->text().toDouble();
    m_device->setNomSpeed(curNomSpeed);
    m_mapOfSettings[m_device->getName()].setNomSpeed(curNomSpeed);
}
void STANDACalibratorWidget::setDevAccel()
{
    int curDevAccel = (int)m_pDevAccelerationEdit->text().toDouble();
    m_device->setDevAccel(curDevAccel);
    m_mapOfSettings[m_device->getName()].setAccel(curDevAccel);
}
void STANDACalibratorWidget::setDevDecel()
{
    int curDevDecel = (int)m_pDevDecelerationEdit->text().toDouble();
    m_device->setDevDecel(curDevDecel);
    m_mapOfSettings[m_device->getName()].setDecel(curDevDecel);
}
void STANDACalibratorWidget::setPosition()
{
    int curDevPos = (int)m_pDevPosEdit->text().toDouble();
    m_device->moveTo(curDevPos);
}
void STANDACalibratorWidget::setZeroPosition()
{
    int curZeroPos = (int)m_pcurDevPos->text().toDouble();
    m_mapOfSettings[m_device->getName()].setZeroPosition(curZeroPos);
}
void STANDACalibratorWidget::setMaxPosition()
{
    int curDevMaxPos = (int)m_pcurDevPos->text().toDouble();
    m_mapOfSettings[m_device->getName()].setMaxPosition(curDevMaxPos);
}
void STANDACalibratorWidget::setLSw1Border(QString str)
{
    qDebug() << str;
    if (str == "left") {
        m_pLSw2Border->setCurrentIndex(1);
        m_device->setSW1ToLeft();
    }
    else {
        m_pLSw2Border->setCurrentIndex(0);
        m_device->setSW1ToRight();
    }
}

void STANDACalibratorWidget::setLSw2Border(QString str)
{
    qDebug() << str;
    if (str == "left") {
        m_pLSw1Border->setCurrentIndex(1);
        m_device->setSW1ToRight();
    }
    else {
        m_pLSw1Border->setCurrentIndex(0);
        m_device->setSW1ToLeft();
    }
}

void STANDACalibratorWidget::setLSw1LOW(QString str)
{
    qDebug() << str;
    if (str == "open") {
        m_device->setSW1LOW(false);
    }
    else {
        m_device->setSW1LOW(true);
    }

}

void STANDACalibratorWidget::setLSw2LOW(QString str)
{
    qDebug() << str;
    if (str == "open") {
        m_device->setSW2LOW(false);
    }
    else {
        m_device->setSW2LOW(true);
    }
}

// -----------------------------

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
    //m_ndevs = 3; // TODO It exits just for Tests
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
            QString curDevFriendlyName = m_device->getFriendlyName();
            //curDevFriendlyName = "fr_device_"+QString().setNum(i); // TODO It exits just for Tests
            m_devFriendlyNamesList << curDevFriendlyName;
            m_mapOfSettings[m_devNamesList.at(i)].setFriendlyName(curDevFriendlyName);
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

    QHBoxLayout *phbxLayout = new QHBoxLayout;

    for (int i=0; i<m_ndevs; i++) {
        QRadioButton *pcurRbtn = new QRadioButton(m_mapOfSettings[m_devNamesList.at(i)].getFriendlyName());
        phbxLayout->addWidget(pcurRbtn);
        connect(pcurRbtn, SIGNAL(toggled(bool)), this, SLOT(trigger(bool)));
    }

    m_pgbxOfDevs->setLayout(phbxLayout);

    m_pmainLayout->addWidget(m_pgbxOfDevs);
}

void STANDACalibratorWidget::MakeControlWindow()
{
    QHBoxLayout *phbxLayoutCW = new QHBoxLayout;

    QVBoxLayout *pvbxLayout0Column = new QVBoxLayout;

    m_pcurDevNameLable = new QLabel("empty");
    m_pcurDevVoltage = new QLabel("empty");
    m_pcurDevSpeed = new QLabel("empty");
    m_pcurDevAcceleration = new QLabel("empty");
    m_pcurDevDeceleration = new QLabel("empty");
    m_pcurDevPos = new QLabel("empty");

    m_pDevNameEdit = new QLineEdit;
    m_pDevVoltageEdit = new QLineEdit;
    m_pDevSpeedEdit = new QLineEdit;
    m_pDevAccelerationEdit = new QLineEdit;
    m_pDevDecelerationEdit = new QLineEdit;
    m_pDevPosEdit = new QLineEdit;

    m_pDevVoltageEdit->setValidator(new QIntValidator);
    m_pDevSpeedEdit->setValidator(new QIntValidator);
    m_pDevAccelerationEdit->setValidator(new QIntValidator);
    m_pDevDecelerationEdit->setValidator(new QIntValidator);
    m_pDevPosEdit->setValidator(new QIntValidator);

    m_pcmdDevSetZeroPos = new QPushButton("setZeroPos");
    m_pcmdDevSetMaxPos = new QPushButton("setMaxPos");


    m_pDevNameOk = new QPushButton("ok");
    m_pDevVoltageOk = new QPushButton("ok");
    m_pDevSpeedOk = new QPushButton("ok");
    m_pDevAccelerationOk = new QPushButton("ok");
    m_pDevDecelerationOk = new QPushButton("ok");
    m_pDevPosOk = new QPushButton("ok");


    QHBoxLayout *phbxLayout0ColumnRow0 = new QHBoxLayout;
    phbxLayout0ColumnRow0->addWidget(new QLabel(""), 0, Qt::AlignTop | Qt::AlignLeft);
    phbxLayout0ColumnRow0->addWidget(new QLabel("current"), 0, Qt::AlignTop | Qt::AlignLeft);
    phbxLayout0ColumnRow0->addWidget(new QLabel("set"), 0, Qt::AlignTop | Qt::AlignCenter);
    phbxLayout0ColumnRow0->addWidget(new QLabel("ok"), 0, Qt::AlignTop | Qt::AlignCenter);
    pvbxLayout0Column->addLayout(phbxLayout0ColumnRow0);

    QHBoxLayout *phbxLayout0ColumnRow1 = new QHBoxLayout;
    phbxLayout0ColumnRow1->addWidget(new QLabel("devName:"), 0, Qt::AlignTop | Qt::AlignLeft);
    phbxLayout0ColumnRow1->addWidget(m_pcurDevNameLable, 0, Qt::AlignTop | Qt::AlignCenter);
    phbxLayout0ColumnRow1->addWidget(m_pDevNameEdit, 0, Qt::AlignTop | Qt::AlignCenter);
    phbxLayout0ColumnRow1->addWidget(m_pDevNameOk, 0, Qt::AlignTop | Qt::AlignCenter);
    pvbxLayout0Column->addLayout(phbxLayout0ColumnRow1);

    QHBoxLayout *phbxLayout0ColumnRow2 = new QHBoxLayout;
    phbxLayout0ColumnRow2->addWidget(new QLabel("devVoltage:"), 0, Qt::AlignTop | Qt::AlignLeft);
    phbxLayout0ColumnRow2->addWidget(m_pcurDevVoltage, 0, Qt::AlignTop | Qt::AlignCenter);
    phbxLayout0ColumnRow2->addWidget(m_pDevVoltageEdit, 0, Qt::AlignTop | Qt::AlignCenter);
    phbxLayout0ColumnRow2->addWidget(m_pDevVoltageOk, 0, Qt::AlignTop | Qt::AlignCenter);
    pvbxLayout0Column->addLayout(phbxLayout0ColumnRow2);

    QHBoxLayout *phbxLayout0ColumnRow3 = new QHBoxLayout;
    phbxLayout0ColumnRow3->addWidget(new QLabel("devSpeed:"), 0, Qt::AlignTop | Qt::AlignLeft);
    phbxLayout0ColumnRow3->addWidget(m_pcurDevSpeed, 0, Qt::AlignTop | Qt::AlignCenter);
    phbxLayout0ColumnRow3->addWidget(m_pDevSpeedEdit, 0, Qt::AlignTop | Qt::AlignCenter);
    phbxLayout0ColumnRow3->addWidget(m_pDevSpeedOk, 0, Qt::AlignTop | Qt::AlignCenter);
    pvbxLayout0Column->addLayout(phbxLayout0ColumnRow3);

    QHBoxLayout *phbxLayout0ColumnRow4 = new QHBoxLayout;
    phbxLayout0ColumnRow4->addWidget(new QLabel("devAccel:"), 0, Qt::AlignTop | Qt::AlignLeft);
    phbxLayout0ColumnRow4->addWidget(m_pcurDevAcceleration, 0, Qt::AlignTop | Qt::AlignCenter);
    phbxLayout0ColumnRow4->addWidget(m_pDevAccelerationEdit, 0, Qt::AlignTop | Qt::AlignCenter);
    phbxLayout0ColumnRow4->addWidget(m_pDevAccelerationOk, 0, Qt::AlignTop | Qt::AlignCenter);
    pvbxLayout0Column->addLayout(phbxLayout0ColumnRow4);

    QHBoxLayout *phbxLayout0ColumnRow5 = new QHBoxLayout;
    phbxLayout0ColumnRow5->addWidget(new QLabel("devDecel:"), 0, Qt::AlignTop | Qt::AlignLeft);
    phbxLayout0ColumnRow5->addWidget(m_pcurDevDeceleration, 0, Qt::AlignTop | Qt::AlignCenter);
    phbxLayout0ColumnRow5->addWidget(m_pDevDecelerationEdit, 0, Qt::AlignTop | Qt::AlignCenter);
    phbxLayout0ColumnRow5->addWidget(m_pDevDecelerationOk, 0, Qt::AlignTop | Qt::AlignCenter);
    pvbxLayout0Column->addLayout(phbxLayout0ColumnRow5);

    QHBoxLayout *phbxLayout0ColumnRow6 = new QHBoxLayout;
    phbxLayout0ColumnRow6->addWidget(new QLabel("devPos:"), 0, Qt::AlignTop | Qt::AlignLeft);
    phbxLayout0ColumnRow6->addWidget(m_pcurDevPos, 0, Qt::AlignTop | Qt::AlignCenter);
    phbxLayout0ColumnRow6->addWidget(m_pDevPosEdit, 0, Qt::AlignTop | Qt::AlignCenter);
    phbxLayout0ColumnRow6->addWidget(m_pDevPosOk, 0, Qt::AlignTop | Qt::AlignCenter);
    pvbxLayout0Column->addLayout(phbxLayout0ColumnRow6);

    QHBoxLayout *phbxLayout0ColumnRow7 = new QHBoxLayout;
    phbxLayout0ColumnRow7->addWidget(new QLabel(""), 0, Qt::AlignTop | Qt::AlignCenter);
    phbxLayout0ColumnRow7->addWidget(new QLabel(""), 0, Qt::AlignTop | Qt::AlignCenter);
    phbxLayout0ColumnRow7->addWidget(new QLabel(""), 0, Qt::AlignTop | Qt::AlignCenter);
    phbxLayout0ColumnRow7->addWidget(new QLabel(""), 0, Qt::AlignTop | Qt::AlignCenter);
    phbxLayout0ColumnRow7->addWidget(m_pcmdDevSetZeroPos, 0, Qt::AlignTop | Qt::AlignCenter);
    phbxLayout0ColumnRow7->addWidget(m_pcmdDevSetMaxPos, 0, Qt::AlignTop | Qt::AlignCenter);
    phbxLayout0ColumnRow7->addWidget(new QLabel(""), 0, Qt::AlignTop | Qt::AlignCenter);
    phbxLayout0ColumnRow7->addWidget(new QLabel(""), 0, Qt::AlignTop | Qt::AlignCenter);
    pvbxLayout0Column->addLayout(phbxLayout0ColumnRow7);

    phbxLayoutCW->addLayout(pvbxLayout0Column);

    m_pDevLeft = new QPushButton("left");
    m_pDevRight = new QPushButton("right");

    QHBoxLayout *phbxLayoutLR = new QHBoxLayout;
    phbxLayoutLR->addWidget(m_pDevLeft);
    phbxLayoutLR->addWidget(m_pDevRight);

    m_pDevHome = new QPushButton("home");
    m_pDevStop = new QPushButton("stop");

    QVBoxLayout *pvbxLayout1Column = new QVBoxLayout;
    pvbxLayout1Column->addWidget(new QLabel("move"), 0, Qt::AlignCenter);
    pvbxLayout1Column->addLayout(phbxLayoutLR);
    pvbxLayout1Column->addWidget(m_pDevHome);
    pvbxLayout1Column->addWidget(m_pDevStop);

    m_pLSw1PushedPos = new QComboBox;
    m_pLSw1PushedPos->addItem("open");
    m_pLSw1PushedPos->addItem("close");
    m_pLSw1Border = new QComboBox;
    m_pLSw1Border->addItem("left");
    m_pLSw1Border->addItem("right");
    m_pLSw1Border->setCurrentIndex(0);
    m_pLSw2PushedPos = new QComboBox;
    m_pLSw2PushedPos->addItem("open");
    m_pLSw2PushedPos->addItem("close");
    m_pLSw2Border = new QComboBox;
    m_pLSw2Border->addItem("left");
    m_pLSw2Border->addItem("right");
    m_pLSw2Border->setCurrentIndex(1);

    connect(m_pLSw1Border, SIGNAL(textActivated(QString)), this, SLOT(setLSw1Border(QString)));
    connect(m_pLSw2Border, SIGNAL(textActivated(QString)), this, SLOT(setLSw2Border(QString)));
    connect(m_pLSw1PushedPos, SIGNAL(textActivated(QString)), this, SLOT(setLSw1LOW(QString)));
    connect(m_pLSw2PushedPos, SIGNAL(textActivated(QString)), this, SLOT(setLSw2LOW(QString)));

    pvbxLayout1Column->addWidget(new QLabel("Limit Switch 1"));
    QHBoxLayout *phbxLSw1PushedPosLayout = new QHBoxLayout;
    phbxLSw1PushedPosLayout->addWidget(new QLabel("Pushed Position"));
    phbxLSw1PushedPosLayout->addWidget(m_pLSw1PushedPos);
    pvbxLayout1Column->addLayout(phbxLSw1PushedPosLayout);
    QHBoxLayout *phbxLSw1BorderLayout = new QHBoxLayout;
    phbxLSw1BorderLayout->addWidget(new QLabel("Border"));
    phbxLSw1BorderLayout->addWidget(m_pLSw1Border);
    pvbxLayout1Column->addLayout(phbxLSw1BorderLayout);

    pvbxLayout1Column->addWidget(new QLabel("Limit Switch 2"));
    QHBoxLayout *phbxLSw2PushedPosLayout = new QHBoxLayout;
    phbxLSw2PushedPosLayout->addWidget(new QLabel("Pushed Position"));
    phbxLSw2PushedPosLayout->addWidget(m_pLSw2PushedPos);
    pvbxLayout1Column->addLayout(phbxLSw2PushedPosLayout);
    QHBoxLayout *phbxLSw2BorderLayout = new QHBoxLayout;
    phbxLSw2BorderLayout->addWidget(new QLabel("Border"));
    phbxLSw2BorderLayout->addWidget(m_pLSw2Border);
    pvbxLayout1Column->addLayout(phbxLSw2BorderLayout);

    phbxLayoutCW->addLayout(pvbxLayout1Column);

    m_pmainLayout->addLayout(phbxLayoutCW);
}

void STANDACalibratorWidget::ConnectDeviceAndCW()
{
    connect(m_pDevLeft, SIGNAL(pressed()), m_device, SLOT(left()));
    connect(m_pDevLeft, SIGNAL(released()), m_device, SLOT(sstp()));
    connect(m_pDevRight, SIGNAL(pressed()), m_device, SLOT(right()));
    connect(m_pDevRight, SIGNAL(released()), m_device, SLOT(sstp()));
    connect(m_pDevHome, SIGNAL(clicked()), m_device, SLOT(home()));
    connect(m_pDevStop, SIGNAL(clicked()), m_device, SLOT(stop()));

    connect(m_pDevNameOk, SIGNAL(clicked()), this, SLOT(setFriendlyName()));
    connect(m_pDevVoltageOk, SIGNAL(clicked()),this, SLOT(setNomVoltage()));
    connect(m_pDevSpeedOk, SIGNAL(clicked()),this, SLOT(setNomSpeed()));
    connect(m_pcmdDevSetZeroPos, SIGNAL(clicked()),this, SLOT(setZeroPosition()));
    connect(m_pcmdDevSetMaxPos, SIGNAL(clicked()),this, SLOT(setMaxPosition()));
    connect(m_pDevAccelerationOk, SIGNAL(clicked()), this, SLOT(setDevAccel()));
    connect(m_pDevDecelerationOk, SIGNAL(clicked()),this, SLOT(setDevDecel()));
    connect(m_pDevPosOk, SIGNAL(clicked()), this, SLOT(setPosition()));
}

void STANDACalibratorWidget::CheckOutputTxt()
{
    QFile file("../settingsdata/fileout.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "../settingsdata/fileout.txt does not exist!";
    }
    else {
        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line;
            in >> line;
            for (int i=0; i<m_ndevs; i++) {
                QString curDevName=m_devNamesList.at(i);
                if (line == curDevName) {
                    in >> line;
                    m_mapOfSettings[curDevName].setFriendlyName(line);
                    in >> line;
                    m_mapOfSettings[curDevName].setNomVoltage((int)line.toDouble());
                    in >> line;
                    m_mapOfSettings[curDevName].setNomSpeed((int)line.toDouble());
                    in >> line;
                    m_mapOfSettings[curDevName].setAccel((int)line.toDouble());
                    in >> line;
                    m_mapOfSettings[curDevName].setDecel((int)line.toDouble());
                    in >> line;
                    m_mapOfSettings[curDevName].setZeroPosition((int)line.toDouble());
                    in >> line;
                    m_mapOfSettings[curDevName].setMaxPosition((int)line.toDouble());
                    break;
                }
            }
        }
    }

}
void STANDACalibratorWidget::WriteOutputTxt()
{
    QFile fileOut("../settingsdata/fileout.txt");
    if(fileOut.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream writeStream(&fileOut);
        for (int i=0; i<m_ndevs; i++) {
            QString curDevName=m_devNamesList.at(i);
            writeStream << curDevName << "\t";
            writeStream << m_mapOfSettings[curDevName].getFriendlyName() << "\t";
            writeStream << m_mapOfSettings[curDevName].getNomVoltage() << "\t";
            writeStream << m_mapOfSettings[curDevName].getNomSpeed() << "\t";
            writeStream << m_mapOfSettings[curDevName].getAccel() << "\t";
            writeStream << m_mapOfSettings[curDevName].getDecel() << "\t";
            writeStream << m_mapOfSettings[curDevName].getZeroPosition() << "\t";
            writeStream << m_mapOfSettings[curDevName].getMaxPosition() << "\n";
        }
    }
    fileOut.close();
}
/*
void STANDACalibratorWidget::saveCurrentDevParams(QString curDevName)
{
    m_mapOfFriendlyNames[curDevName]=m_curFriendlyName;
    m_mapOfDevVoltages[curDevName]=m_curDevVoltage;
    m_mapOfDevSpeeds[curDevName]=m_curDevSpeed;
    m_mapOfDevAccel[curDevName]=m_curDevAccel;
    m_mapOfDevDecel[curDevName]=m_curDevDecel;
    m_mapOfDevZeroPoses[curDevName]=m_curDevZeroPos;
    m_mapOfDevMaxPoses[curDevName]=m_curDevMaxPos;
}
*/
void STANDACalibratorWidget::timerEvent(QTimerEvent*)
{
/*
    m_pDevNameEdit->resize(m_pDevHome->size().width(), m_pDevHome->size().height());
    m_pDevVoltageEdit->resize(m_pDevHome->size().width(), m_pDevHome->size().height());
    m_pDevSpeedEdit->resize(m_pDevHome->size().width(), m_pDevHome->size().height());
    m_pDevDecelerationEdit->resize(m_pDevHome->size().width(), m_pDevHome->size().height());
    m_pcmdDevSetZeroPos->resize(m_pDevHome->size().width(), m_pDevHome->size().height());
    m_pcmdDevSetMaxPos->resize(m_pDevHome->size().width(), m_pDevHome->size().height());*/

    m_pcurDevNameLable->setText(m_device->getFriendlyName());
    m_pcurDevVoltage->setText(QString().setNum(m_device->getCurVoltage()));
    m_pcurDevSpeed->setText(QString().setNum(m_device->getCurSpeed()));
    m_pcurDevAcceleration->setText(QString().setNum(m_device->getCurAcceleration()));
    m_pcurDevDeceleration->setText(QString().setNum(m_device->getCurDeceleration()));
    m_pcurDevPos->setText(QString().setNum(m_device->getCurOwnPosition()));
}
