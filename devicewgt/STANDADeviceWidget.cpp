#include <QDebug>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QSlider>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "STANDADeviceWidget.h"

STANDADeviceWidget::STANDADeviceWidget(QWidget *parent/*=0*/)
    : QWidget(parent)
{
    setFixedSize(800, 80);
    makeLabels();
    makeEditors();
    makeButtons();
    makeLayout();
    makeConnections();
}

STANDADeviceWidget::~STANDADeviceWidget()
{
}

int STANDADeviceWidget::getmaxStepNumber()
{
    double lenAxisMoveDevice= getmaxPos() - getminPos();
    return (int)lenAxisMoveDevice*100; // to provide accuracy 0.01
}
// -------- make base widgets that the class includes
void STANDADeviceWidget::makeLabels()
{
    plblName    = new QLabel("&name");
    plblId      = new QLabel("&id");
    plblPos     = new QLabel("&pos [mm]");
    plblupPos   = new QLabel("&up pos");
    plbldownPos = new QLabel("&down pos");
}
void STANDADeviceWidget::makeEditors()
{
    pnameEdit = new QLineEdit("empty");
    //pnameEdit->setAlignment(Qt::AlignRight);
    plblName->setBuddy(pnameEdit);

    pidEdit = new QLineEdit("0");
    pidEdit->setAlignment(Qt::AlignRight);
    pidEdit->setValidator(new QIntValidator);
    plblId->setBuddy(pidEdit);

    pposEdit = new QLineEdit("0");
    pposEdit->setAlignment(Qt::AlignRight);
    QDoubleValidator *pposValidator = new QDoubleValidator(getminPos(), getmaxPos(), 2);
    pposValidator->setNotation(QDoubleValidator::StandardNotation);
    pposValidator->setLocale(QLocale::C);
    pposEdit->setValidator(pposValidator);
    //pposValidator->setRange(getminPos(), getmaxPos());
    plblPos->setBuddy(pposEdit);

    pdPosEdit = new QLineEdit(QString("").setNum(getdPos()));
    pdPosEdit->setAlignment(Qt::AlignRight);
    const QSize editorSize = QSize(50, 25);
    pdPosEdit->setFixedSize(editorSize);
    QDoubleValidator *pdPosValidator = new QDoubleValidator(pdPosEdit);
    pdPosValidator->setLocale(QLocale::C);
    pdPosEdit->setValidator(pdPosValidator);
}
void STANDADeviceWidget::makeButtons()
{
    const QSize cmdSize = QSize(25, 25);
    pcmdOk      = new QPushButton("Ok");
    pcmdMove    = new QPushButton("move");
    pcmddownPos = new QPushButton("-");
    pcmddownPos->setFixedSize(cmdSize);
    plbldownPos->setBuddy(pcmddownPos);
    pcmdupPos   = new QPushButton("+");
    pcmdupPos->setFixedSize(cmdSize);
    plblupPos->setBuddy(pcmdupPos);

    const QSize sldrSize = QSize(300, 25);
    psldrdeviceStep = new QSlider(Qt::Horizontal);
    psldrdeviceStep->setFixedSize(sldrSize);
    psldrdeviceStep->setSingleStep(1);
    psldrdeviceStep->setRange(0, getmaxStepNumber());
    psldrdeviceStep->setValue(QString(pposEdit->text()).toInt());
    int tickInterval = getmaxStepNumber()/20;
    psldrdeviceStep->setTickInterval(tickInterval);
    psldrdeviceStep->setTickPosition(QSlider::TicksBelow);
}
void STANDADeviceWidget::makeLayout()
{
    // 1 column
    QVBoxLayout *pvbxLayout1 = new QVBoxLayout;
    pvbxLayout1->addWidget(plblName,  0, Qt::AlignCenter);
    pvbxLayout1->addWidget(pnameEdit, 0, Qt::AlignCenter);

    // 2 column
    QVBoxLayout *pvbxLayout2 = new QVBoxLayout;
    pvbxLayout2->addWidget(plblId,  0, Qt::AlignCenter);
    pvbxLayout2->addWidget(pidEdit, 0, Qt::AlignCenter);

    // 3 column
    QVBoxLayout *pvbxLayout3 = new QVBoxLayout;
    pvbxLayout3->addWidget(plblPos,  0, Qt::AlignCenter);
    pvbxLayout3->addWidget(pposEdit, 0, Qt::AlignCenter);

    // 4 column
    QVBoxLayout *pvbxLayout4 = new QVBoxLayout;
    pvbxLayout4->addWidget(new QLabel(""), 0, Qt::AlignCenter);
    pvbxLayout4->addWidget(pcmddownPos, 0, Qt::AlignCenter);

    // 5 column
    QVBoxLayout *pvbxLayout5 = new QVBoxLayout;
    pvbxLayout5->addWidget(new QLabel(""), 0, Qt::AlignCenter);
    pvbxLayout5->addWidget(pdPosEdit, 0, Qt::AlignCenter);

    // 6 column
    QVBoxLayout *pvbxLayout6 = new QVBoxLayout;
    pvbxLayout6->addWidget(new QLabel(""), 0, Qt::AlignCenter);
    pvbxLayout6->addWidget(pcmdupPos, 0, Qt::AlignCenter);

    // 7column
    QVBoxLayout *pvbxLayout7 = new QVBoxLayout;
    pvbxLayout7->addWidget(new QLabel(""), 0, Qt::AlignCenter);
    pvbxLayout7->addWidget(psldrdeviceStep, 0, Qt::AlignCenter);

    // 8 column
    QVBoxLayout *pvbxLayout8 = new QVBoxLayout;
    pvbxLayout8->addWidget(pcmdOk);
    pvbxLayout8->addWidget(pcmdMove);

    // 1 row
    QHBoxLayout *phbxLayout = new QHBoxLayout;
    phbxLayout->addLayout(pvbxLayout1);
    phbxLayout->addLayout(pvbxLayout2);
    phbxLayout->addLayout(pvbxLayout3);
    phbxLayout->addLayout(pvbxLayout4);
    phbxLayout->addLayout(pvbxLayout5);
    phbxLayout->addLayout(pvbxLayout6);
    phbxLayout->addLayout(pvbxLayout7);
    phbxLayout->addLayout(pvbxLayout8);

    this->setLayout(phbxLayout);
}
void STANDADeviceWidget::makeConnections()
{
    // Connect step of pos changing edit
    connect(pdPosEdit, SIGNAL(textChanged(QString)), SLOT(setdPos(QString)));

    // Connect position edit
    connect(pposEdit, SIGNAL(textChanged(QString)), SLOT(checkUserTypedPosIsValid(QString)));

    // Connect down and up buttons
    connect(pcmddownPos, SIGNAL(clicked()), SLOT(downPos()));
    connect(pcmdupPos,   SIGNAL(clicked()), SLOT(upPos()));

    // Connect Slider
    connect(psldrdeviceStep, SIGNAL(valueChanged(int)), SLOT(setPosBySlider(int)));

    //connect(this, SIGNAL(posIsValid(QString)), SLOT(posIsValidDebug(QString)));
}
// --------------------------------------------------

// ------- Public Slots
void STANDADeviceWidget::setminPos(QString str)
{
    qDebug() << "STANDADeviceWidget::setminPos(" << str << ")";
}
void STANDADeviceWidget::setmaxPos(QString str)
{
    qDebug() << "STANDADeviceWidget::setmaxPos(" << str << ")";
}
void STANDADeviceWidget::setDeviceName(QString str)
{
    qDebug() << "STANDADeviceWidget::setDeviceName(" << str << ")";
}
void STANDADeviceWidget::setDeviceId(QString str)
{
    qDebug() << "STANDADeviceWidget::setDeviceId(" << str << ")";
}
void STANDADeviceWidget::setDeviceBasePosition(QString str)
{
    qDebug() << "STANDADeviceWidget::setDeviceBasePosition(" << str << ")";
}
// --------------------------------------------------


// ------- Private Slots
void STANDADeviceWidget::setPosBySlider(int num)
{
    qDebug() << "STANDADeviceWidget::setPosBySlider(" << num << ")";
    int mod = num/100;
    QString strMod;
    strMod.setNum(mod);
    qDebug() << " mod=" << mod;
    int div = num%100;
    QString strDiv;
    strDiv.setNum(div);
    qDebug() << " div=" << div;
    double axisSliderPos = double(mod) + double(div)/100.;
    qDebug() << " axisSliderPos=" << axisSliderPos;
    disconnect(pposEdit, SIGNAL(textChanged(QString)), this, SLOT(checkUserTypedPosIsValid(QString)));
    pposEdit->setText(QString().setNum(axisSliderPos));
    emit posIsValid(QString().setNum(axisSliderPos));
    connect(pposEdit, SIGNAL(textChanged(QString)), this, SLOT(checkUserTypedPosIsValid(QString)));
}

void STANDADeviceWidget::setSliderToPos(QString str)
{
    qDebug() << "STANDADeviceWidget::setSliderToPos(" << str << ")";
        int curSliderValue = int(100.*str.toDouble());
        qDebug() << "curSliderValue=" << curSliderValue;
        disconnect(psldrdeviceStep, SIGNAL(valueChanged(int)), this, SLOT(setPosBySlider(int)));
        psldrdeviceStep->setValue(curSliderValue);
        connect(psldrdeviceStep, SIGNAL(valueChanged(int)), this, SLOT(setPosBySlider(int)));
}
void STANDADeviceWidget::checkDevice()
{

}
void STANDADeviceWidget::downPos()
{
    //qDebug() << "STANDADeviceWidget::downPos()";
    QString curPosString = pposEdit->text();
    double curPos = curPosString.toDouble();
    curPos -= getdPos();
    curPosString.setNum(curPos);
    pposEdit->setText(curPosString);
}
void STANDADeviceWidget::upPos()
{
    //qDebug() << "STANDADeviceWidget::upPos()";
    QString curPosString = pposEdit->text();
    double curPos = curPosString.toDouble();
    curPos += getdPos();
    curPosString.setNum(curPos);
    pposEdit->setText(curPosString);
}
void STANDADeviceWidget::setdPos(QString str)
{
    setdPos(str.toDouble());
}
void STANDADeviceWidget::checkUserTypedPosIsValid(QString str)
{
    qDebug() << "STANDADeviceWidget::checkUserTypedPosIsValid(" << str << ")";
    if (str != getPreviousPos() && str != "") {
        double curPos = str.toDouble();
        if (curPos <= getmaxPos() && curPos >= getminPos()) {
            setPreviousPos(str);
            setSliderToPos(str);
            posIsValid(str);
        }
        else {
            emit posIsNotValid();
            pposEdit->setText(getPreviousPos());
        }
    }
}

void STANDADeviceWidget::moveStart()
{
    emit startMoveDevice();
}
void STANDADeviceWidget::moveStop()
{
    emit stopMoveDevice();
}
void STANDADeviceWidget::posIsValidDebug(QString str)
{
    qDebug() << "posIsValid = " << str;
}
// --------------------------------------------------
