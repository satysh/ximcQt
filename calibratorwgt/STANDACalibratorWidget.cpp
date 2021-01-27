#include "STANDACalibratorWidget.h"

#include <ximc.h>

#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>
#include <QTimerEvent>
#include <QIntValidator>
#include <QFile>
#include <QTextStream>

STANDACalibratorWidget::STANDACalibratorWidget(QWidget *parent/* = nullptr*/)
    : QWidget(parent)
{
    qDebug() << "STANDACalibratorWidget::STANDACalibratorWidget";
    int fixedw=500;
    int fixedh=500;

    setFixedSize(fixedw, fixedh);
    m_pInfoWindow = new QTextEdit(this);
    m_pInfoWindow->resize(fixedw, fixedh);
    m_pInfoWindow->setReadOnly(true);

    LoadAvailableDevices();
}

STANDACalibratorWidget::~STANDACalibratorWidget()
{
    qDebug() << "STANDACalibratorWidget::~STANDACalibratorWidget";
    if (m_pcurDevice) delete m_pcurDevice;
    Close();
}

void STANDACalibratorWidget::LoadAvailableDevices()
{
    bool loadStatus=true;
    const int probe_flags = ENUMERATE_PROBE;
    const char* enumerate_hints = "";

    QStringList outStringsList;
    char ximc_version_str[32];
    device_enumeration_t devenum;

//  ximc_version returns library version string.
    ximc_version(ximc_version_str);
    qDebug() << "libximc version " << ximc_version_str;
    outStringsList << "libximc version " << ximc_version_str << "\n";

//  Set bindy (network) keyfile. Must be called before any call to "enumerate_devices" or "open_device" if you
//  wish to use network-attached controllers. Accepts both absolute and relative paths, relative paths are resolved
//  relative to the process working directory. If you do not need network devices then "set_bindy_key" is optional.
    set_bindy_key("keyfile.sqlite");

//  Device enumeration function. Returns an opaque pointer to device enumeration data.
    devenum = enumerate_devices(probe_flags, enumerate_hints);

//  Gets device count from device enumeration data
    setndevs(get_device_count(devenum));

    //setndevs(2); // TODO It exists just for test
//  Terminate if there are no connected devices
    if (getndevs() <= 0)
    {
        qDebug() << "No devices found";
        outStringsList << "No devices found";
    //  Free memory used by device enumeration data
        free_enumerate_devices(devenum);
        loadStatus=false;
        emit failed();
    }
    else {
        for (int i=0; i<getndevs(); i++) {
            QString curDevName(get_device_name(devenum, i));
            //curDevName = "device_"+QString().setNum(i); // TODO It exits just for Tests
            addDevice(curDevName);
        }


        qDebug() << getndevs() << " devices were found:";
        outStringsList << QString().setNum(getndevs()) << " devices were found:\n";
        for (int i=0; i<getndevs(); i++) {
            qDebug() << " " << i+1 << ": " << m_devNamesList.at(i);
            outStringsList << " " << QString().setNum(i+1) << ": " << m_devNamesList.at(i) << "\n";
        }
        //Free memory used by device enumeration data
        free_enumerate_devices(devenum);
        emit successfull();
    }
    QString outString;
    for (int i=0; i<outStringsList.size(); ++i) {
        outString += outStringsList.at(i);
    }
    m_pInfoWindow->setText(outString);
    if (loadStatus) {
        m_pNext = new QPushButton("next", this);
        m_pNext->move(400, 50);
        connect(m_pNext, SIGNAL(clicked()), this, SLOT(InitCalibration()));
    }
}

void STANDACalibratorWidget::CloseCurDevice()
{
    m_pcurDevice->Close();
    m_curIndex++;
    if (m_curIndex < getndevs()) {
        QString devName("curDevName: ");
        devName += m_devNamesList.at(m_curIndex);
        m_pDevNameLabel->setText(devName);
        m_pcurDevice->setName(getcurDevName());
        m_pcurDevice->Init();
    }
    else {
        m_pInfoWindow->clear();
        m_pDevNameLabel->setText("End");
        m_pNext->setText("finish");
        disconnect(m_pNext, SIGNAL(clicked()), this, SLOT(next()));
        connect(m_pNext, SIGNAL(clicked()), this, SLOT(finish()));
        m_pNext->setEnabled(true);
        m_pSetNomVoltage->setEnabled(false);
        m_pSetNomSpeed->setEnabled(false);
        m_pSetZeroPos->setEnabled(false);
        m_pSetMaxPos->setEnabled(false);
        m_pHome->setEnabled(false);
        m_pLeft->setEnabled(false);
        m_pRight->setEnabled(false);
    }
}
QString STANDACalibratorWidget::getcurDevName()
{
    if ( m_devNamesList.size() > 0)
        return m_devNamesList.at(m_curIndex);
    else
        return "";
}

void STANDACalibratorWidget::writeOutputTxt()
{
    QFile fileOut("../settingsdata/fileout.txt"); 
    if(fileOut.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream writeStream(&fileOut);
        for (int i=0; i<getndevs(); i++) {
            writeStream << m_devNamesList.at(i) << "\t";
            writeStream << m_vDevVoltages.at(i) << "\t";
            writeStream << m_vDevSpeeds.at(i) << "\t";
            writeStream << m_vDevZeroPoses.at(i) << "\t";
            writeStream << m_vDevMaxPoses.at(i) << "\n";
        }
    }
    fileOut.close(); 
}

void STANDACalibratorWidget::connectButtons()
{
    connect(m_pStop, SIGNAL(clicked()), m_pcurDevice, SLOT(stop()));
    connect(m_pHome, SIGNAL(clicked()), m_pcurDevice, SLOT(home()));
    connect(m_pLeft, SIGNAL(pressed()),  m_pcurDevice, SLOT(left()));
    connect(m_pLeft, SIGNAL(released()), m_pcurDevice, SLOT(stop()));

    connect(m_pRight, SIGNAL(pressed()),  m_pcurDevice, SLOT(right()));
    connect(m_pRight, SIGNAL(released()), m_pcurDevice, SLOT(stop()));

    connect(m_pSetNomVoltage , SIGNAL(clicked()), this, SLOT(setNomVoltage()));
    connect(m_pSetNomSpeed, SIGNAL(clicked()), this, SLOT(setNomSpeed()));
    connect(m_pSetZeroPos, SIGNAL(clicked()), this, SLOT(setZeroPos()));
    connect(m_pSetMaxPos, SIGNAL(clicked()), this, SLOT(setMaxPos()));

}

bool STANDACalibratorWidget::check()
{
    return m_nomVoltageIsSet&m_nomSpeedIsSet&m_zeroPosIsSet&m_maxPosIsSet;
}

void STANDACalibratorWidget::timerEvent(QTimerEvent *ptimerEv)
{
    //qDebug() << "timer " << ptimerEv->timerId();
    m_pCurVoltageLabel->setText(QString().setNum(m_pcurDevice->getCurVoltage()));
    m_pCurSpeedLabel->setText(QString().setNum(m_pcurDevice->getCurSpeed()));
    m_pCurPosLabel->setText(QString().setNum(m_pcurDevice->getCurOwnPosition()));

    if (check()) m_pNext->setEnabled(true);
}

// ---- Private slots -----------------------------------------------------------------
void STANDACalibratorWidget::InitCalibration()
{
//    m_pInfoWindow->resize(size().width(), size().height()-400);
    disconnect(m_pNext, SIGNAL(clicked()), this, SLOT(InitCalibration()));
    connect(m_pNext, SIGNAL(clicked()), this, SLOT(next()));
    m_pNext->setEnabled(false);
    //m_pInfoWindow->clear();
    startTimer(10);
    
    // Init first device 
    m_pcurDevice = new STANDADevice(getcurDevName());
    m_pcurDevice->Init();

    QVBoxLayout *pvbxLayout = new QVBoxLayout;

    QString devName("curDevName: ");
    devName += m_devNamesList.at(0);
    m_pDevNameLabel = new QLabel(devName);

    QHBoxLayout *phbx1RowLayout = new QHBoxLayout; // 1 row
    phbx1RowLayout->addWidget(m_pDevNameLabel);
    phbx1RowLayout->addWidget(m_pNext);
    pvbxLayout->addLayout(phbx1RowLayout);

    QVBoxLayout *pvbx1ColumnLayout = new QVBoxLayout;
    QLabel *voltageLabel = new QLabel("curVoltage");
    QFont font;
    font.setPixelSize(10);
    voltageLabel->setFont(font);
    pvbx1ColumnLayout->addWidget(voltageLabel, 0, Qt::AlignBottom);
    m_pCurVoltageLabel = new QLabel("0");
    pvbx1ColumnLayout->addWidget(m_pCurVoltageLabel);
    QLabel *speedLabel = new QLabel("curSpeed");
    speedLabel->setFont(font);
    pvbx1ColumnLayout->addWidget(speedLabel, 0, Qt::AlignBottom);
    m_pCurSpeedLabel = new QLabel("0");
    pvbx1ColumnLayout->addWidget(m_pCurSpeedLabel);
    QLabel *posLabel = new QLabel("curPos");
    posLabel->setFont(font);
    pvbx1ColumnLayout->addWidget(posLabel);
    m_pCurPosLabel = new QLabel("0");
    pvbx1ColumnLayout->addWidget(m_pCurPosLabel);

    QVBoxLayout *pvbx2ColumnLayout = new QVBoxLayout;
    QLabel *nomvolLabel = new QLabel("nomVoltage");
    nomvolLabel->setFont(font);
    pvbx2ColumnLayout->addWidget(nomvolLabel, 0, Qt::AlignCenter);
    m_pNomVoltageEdit = new QLineEdit(QString().setNum(m_pcurDevice->getCurVoltage()));
    m_pNomVoltageEdit->setValidator(new QIntValidator);
    pvbx2ColumnLayout->addWidget(m_pNomVoltageEdit);
    QLabel *nomspeedLabel = new QLabel("0");
    nomspeedLabel->setFont(font);
    pvbx2ColumnLayout->addWidget(nomspeedLabel, 0, Qt::AlignCenter);
    m_pNomSpeedEdit = new QLineEdit(QString().setNum(m_pcurDevice->getCurSpeed()));
    m_pNomSpeedEdit->setValidator(new QIntValidator);
    pvbx2ColumnLayout->addWidget(m_pNomSpeedEdit);
    pvbx2ColumnLayout->addWidget(new QLabel(""));

    QHBoxLayout *phbxButtonsRowLayout = new QHBoxLayout;
    m_pStop = new QPushButton("stop");
    m_pHome = new QPushButton("home");
    m_pLeft = new QPushButton("left");
    phbxButtonsRowLayout->addWidget(m_pStop);
    phbxButtonsRowLayout->addWidget(m_pHome);
    phbxButtonsRowLayout->addWidget(m_pLeft, 0, Qt::AlignRight);

    pvbx2ColumnLayout->addLayout(phbxButtonsRowLayout);


    QVBoxLayout *pvbx3ColumnLayout = new QVBoxLayout;
    pvbx3ColumnLayout->addWidget(new QLabel(""));
    m_pSetNomVoltage = new QPushButton("ok");
    pvbx3ColumnLayout->addWidget(m_pSetNomVoltage);
    pvbx3ColumnLayout->addWidget(new QLabel(""));
    m_pSetNomSpeed = new QPushButton("ok");
    pvbx3ColumnLayout->addWidget(m_pSetNomSpeed);
    pvbx3ColumnLayout->addWidget(new QLabel(""));
    m_pRight = new QPushButton("right");
    pvbx3ColumnLayout->addWidget(m_pRight);

    QHBoxLayout *phbx2RowLayout = new QHBoxLayout; // 2 row
    phbx2RowLayout->addLayout(pvbx1ColumnLayout);
    phbx2RowLayout->addLayout(pvbx2ColumnLayout);
    phbx2RowLayout->addLayout(pvbx3ColumnLayout);
    pvbxLayout->addLayout(phbx2RowLayout);

    QHBoxLayout *phbx3RowLayout = new QHBoxLayout; // 3 row
    m_pSetZeroPos = new QPushButton("setZeroPosition");
    m_pSetMaxPos  = new QPushButton("setMaxPosition");
    phbx3RowLayout->addWidget(m_pSetZeroPos);
    phbx3RowLayout->addWidget(m_pSetMaxPos);
    pvbxLayout->addLayout(phbx3RowLayout);


    pvbxLayout->addWidget(m_pInfoWindow); // last row

    setLayout(pvbxLayout);

    connectButtons();
}
void STANDACalibratorWidget::setNomVoltage()
{
    int curNomVoltage = QString(m_pNomVoltageEdit->text()).toInt();
    qDebug() << "For " << getcurDevName() << " setNomVoltage to " << curNomVoltage;
    if (!m_nomVoltageIsSet) {
        m_vDevVoltages.push_back(curNomVoltage);
        m_nomVoltageIsSet=true;
    }
    else {
        m_vDevVoltages[m_curIndex] = curNomVoltage;
    }
}
void STANDACalibratorWidget::setNomSpeed()
{
    int curNomSpeed = QString(m_pNomSpeedEdit->text()).toInt();
    qDebug() << "For " << getcurDevName() << " setNomSpeed to " << curNomSpeed;
    if (!m_nomSpeedIsSet) {
        m_vDevSpeeds.push_back(curNomSpeed);
        m_nomSpeedIsSet=true;
    }
    else {
        m_vDevSpeeds[m_curIndex] = curNomSpeed;
    }

}
void STANDACalibratorWidget::setZeroPos()
{
    int curZeroPos = QString(m_pCurPosLabel->text()).toInt();
    qDebug() << "For " << getcurDevName() << " setZeroPos to " << curZeroPos;
    if (!m_zeroPosIsSet) {
        m_vDevZeroPoses.push_back(curZeroPos);
        m_zeroPosIsSet=true;
    }
    else {
        m_vDevZeroPoses[m_curIndex] = curZeroPos;
    }
}

void STANDACalibratorWidget::setMaxPos()
{
    int curMaxPos = QString(m_pCurPosLabel->text()).toInt();
    qDebug() << "For " << getcurDevName() << " setMaxPos to " << curMaxPos;
    if (!m_maxPosIsSet) {
        m_vDevMaxPoses.push_back(curMaxPos);
        m_maxPosIsSet=true;
    }
    else {
        m_vDevMaxPoses[m_curIndex] = curMaxPos;
    }
}

void STANDACalibratorWidget::next()
{
    qDebug() << "next: ";
    m_pNext->setEnabled(false);
    m_nomVoltageIsSet=false;
    m_nomSpeedIsSet=false;
    m_zeroPosIsSet=false;
    m_maxPosIsSet=false;
    qDebug() << getcurDevName() << "\n"
             << "zero pos is " << m_vDevMaxPoses.at(m_curIndex) << "\n"
             << "max pos is "   << m_vDevMaxPoses.at(m_curIndex) << "\n"
             << "nom voltage is " << m_vDevVoltages.at(m_curIndex) << "\n"
             << "nom speed is" << m_vDevSpeeds.at(m_curIndex);

    QString outString("For "+getcurDevName()+"\n");
    outString += "zero pos is " + QString().setNum(m_vDevMaxPoses.at(m_curIndex))+"\n";
    outString += "max pos is " + QString().setNum(m_vDevMaxPoses.at(m_curIndex)) + "\n";
    outString += "nom voltage is " + QString().setNum(m_vDevVoltages.at(m_curIndex)) + "\n";
    outString += "nom speed is " + QString().setNum(m_vDevSpeeds.at(m_curIndex));
    m_pInfoWindow->setText(outString);
    CloseCurDevice();
}

void STANDACalibratorWidget::finish()
{
    QString outString("The end!\n\n");
    for (int i=0; i<getndevs(); i++) {
        outString += "For device: " + m_devNamesList.at(i) + QString("\n");
        outString += " nomVoltage = " + QString().setNum(m_vDevVoltages.at(i)) + QString("\n");
        outString += " nomSpeed = "   + QString().setNum(m_vDevSpeeds.at(i)) + QString("\n");
        outString += " zeroPos = "    + QString().setNum(m_vDevZeroPoses.at(i)) + QString("\n");
        outString += " maxPos = "     + QString().setNum(m_vDevMaxPoses.at(i)) + QString("\n\n");
    }

    m_pInfoWindow->setText(outString);
    disconnect(m_pNext, SIGNAL(clicked()), this, SLOT(finish()));
    connect(m_pNext, SIGNAL(clicked()), this, SLOT(Close()));
    writeOutputTxt();
    m_pNext->setText("Close");
}
void STANDACalibratorWidget::Close()
{
    m_devNamesList.clear();
    m_vDevVoltages.clear();
    m_vDevSpeeds.clear();
    m_vDevZeroPoses.clear();
    m_vDevMaxPoses.clear();
    emit calibrationFinished();

/*
    if(m_pCurVoltageLabel) delete m_pCurVoltageLabel;
    if(m_pNomSpeedLabel) delete m_pNomSpeedLabel;
    if(m_pCurPosLabel) delete m_pCurPosLabel;

    if(m_pNomVoltageEdit) delete ;
    if(m_pNomSpeedEdit) delete ;

    if(m_pInfoWindow) delete ;

    if(m_pSetNomVoltage) delete ;
    if(m_pSetNomSpeed) delete ;
    if(m_pSetZeroPos) delete ;
    if(m_pSetMaxPos) delete ;
    if(m_pLeft) delete ;
    if(m_pRight) delete ;
    if(m_pNext) delete ;
*/
}
// ------------------------------------------------------------------------------------

// ---- Public slots ------------------------------------------------------------------

// ------------------------------------------------------------------------------------
