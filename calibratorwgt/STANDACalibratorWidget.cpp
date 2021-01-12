#include "STANDACalibratorWidget.h"

#include <ximc.h>

#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>
#include <QTimerEvent>

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

    setndevs(3);
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
            QString curDevName/*(get_device_name(devenum, i))*/;
            curDevName = "device_"+QString().setNum(i); // Test
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

void STANDACalibratorWidget::InitDevice()
{
    m_pcurDevice = new STANDADevice(getcurDevName());
    m_pcurDevice->Init();
}

QString STANDACalibratorWidget::getcurDevName()
{
    if ( m_devNamesList.size() > 0)
        return m_devNamesList.at(getcurIndex());
    else
        return "";
}
void STANDACalibratorWidget::Close()
{
    m_devNamesList.clear();
    m_vDevVoltages.clear();
    m_vDevSpeeds.clear();
    m_vDevZeroPoses.clear();
    m_vDevMaxPoses.clear();

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

void STANDACalibratorWidget::connectButtons()
{
    connect(m_pLeft, SIGNAL(pressed()),  m_pcurDevice, SLOT(left()));
    connect(m_pLeft, SIGNAL(released()), m_pcurDevice, SLOT(stop()));

    connect(m_pRight, SIGNAL(pressed()),  m_pcurDevice, SLOT(right()));
    connect(m_pRight, SIGNAL(released()), m_pcurDevice, SLOT(stop()));
}

void STANDACalibratorWidget::timerEvent(QTimerEvent *ptimerEv)
{
    //qDebug() << "timer " << ptimerEv->timerId();
    m_pCurVoltageLabel->setText(QString().setNum(m_pcurDevice->getCurVoltage()));
    m_pCurSpeedLabel->setText(QString().setNum(m_pcurDevice->getCurSpeed()));
    m_pCurPosLabel->setText(QString().setNum(m_pcurDevice->getCurOwnPosition()));
}

// ---- Private slots -----------------------------------------------------------------
void STANDACalibratorWidget::InitCalibration()
{
//    m_pInfoWindow->resize(size().width(), size().height()-400);
    disconnect(m_pNext, SIGNAL(clicked()), this, SLOT(InitCalibration()));
    m_pNext->setEnabled(false);
    //m_pInfoWindow->clear();
    startTimer(10);
    InitDevice();

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
    m_pNomVoltageEdit = new QLineEdit("nomvol");
    pvbx2ColumnLayout->addWidget(m_pNomVoltageEdit);
    QLabel *nomspeedLabel = new QLabel("nomSpeed");
    nomspeedLabel->setFont(font);
    pvbx2ColumnLayout->addWidget(nomspeedLabel, 0, Qt::AlignCenter);
    m_pNomSpeedEdit = new QLineEdit("nomspeed");
    pvbx2ColumnLayout->addWidget(m_pNomSpeedEdit);
    pvbx2ColumnLayout->addWidget(new QLabel(""));
    m_pLeft = new QPushButton("left");
    pvbx2ColumnLayout->addWidget(m_pLeft,  0, Qt::AlignRight);


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
}
// ------------------------------------------------------------------------------------

// ---- Public slots ------------------------------------------------------------------

// ------------------------------------------------------------------------------------
