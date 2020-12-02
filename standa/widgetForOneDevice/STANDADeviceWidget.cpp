#include <QDebug>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QSlider>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "../deviceObject/STANDADevice.h"
#include "STANDADeviceWidget.h"

STANDADeviceWidget::STANDADeviceWidget(QWidget *parent/*=0*/)
    : QWidget(parent)
{
    setFixedSize(800, 80);
    pdevice = new STANDADevice;
    makeLabels();
    makeEditors();
    makeButtons();
    makeLayout();
    makeConnections();
}

STANDADeviceWidget::~STANDADeviceWidget()
{
    if (pdevice)   delete pdevice;
    if (pcmdOk)    delete pcmdOk;
    if (pcmdMove)  delete pcmdMove;
    if (plblName)  delete plblName;
    if (plblId)    delete plblId;
    if (plblPos)   delete plblPos;
    if (plblStep)  delete plblStep;
    if (pnameEdit) delete pnameEdit;
    if (pidEdit)   delete pidEdit;
    if (pposEdit)  delete pposEdit;
    if (pcmddownPos) delete pcmddownPos;
    if (pcmdupPos) delete pcmdupPos;
    if (pdevicestepEdit) delete pdevicestepEdit;
    if (psldrdeviceStep) delete psldrdeviceStep;
}

// -------- make base widgets that the class includes
void STANDADeviceWidget::makeLabels()
{
    plblName    = new QLabel("&name");
    plblId      = new QLabel("&id");
    plblPos     = new QLabel("&pos [mm]");
    plblupPos   = new QLabel("&up pos");
    plbldownPos = new QLabel("&down pos");
    plblStep    = new QLabel("&step");
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

    pdevicestepEdit = new QLineEdit("");
    pdevicestepEdit->setAlignment(Qt::AlignRight);
    pdevicestepEdit->setValidator(new QIntValidator(0, getmaxdeviceStep()));
    plblStep->setBuddy(pdevicestepEdit);
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
    //psldrdeviceStep->setSingleStep(1);
    psldrdeviceStep->setRange(0, getmaxdeviceStep());
    psldrdeviceStep->setValue(0);
    psldrdeviceStep->setTickInterval(10);
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
    pvbxLayout8->addWidget(plblStep, 0, Qt::AlignCenter);
    pvbxLayout8->addWidget(pdevicestepEdit, 0, Qt::AlignCenter);

    // 9 column
    QVBoxLayout *pvbxLayout9 = new QVBoxLayout;
    pvbxLayout9->addWidget(pcmdOk);
    pvbxLayout9->addWidget(pcmdMove);

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
    phbxLayout->addLayout(pvbxLayout9);

    this->setLayout(phbxLayout);
}
void STANDADeviceWidget::makeConnections()
{
    // Connect step of pos changing edit
    connect(pdPosEdit, SIGNAL(textChanged(QString)), SLOT(setdPos(QString)));

    // Connect position edit
    connect(pposEdit, SIGNAL(textChanged(QString)), SLOT(checkPosIsValid(QString)));
    
    // Connect down and up buttons 
    connect(pcmddownPos, SIGNAL(clicked()), SLOT(downPos()));
    connect(pcmdupPos,   SIGNAL(clicked()), SLOT(upPos()));

    // Connect device step edit
    connect(pdevicestepEdit, SIGNAL(textChanged(QString)), SLOT(checkdeviceStepIsValid(QString)));

    // Connect Slider 
    connect(psldrdeviceStep, SIGNAL(valueChanged(int)), SLOT(checkdeviceStepIsValid(int)));

    // ------- Connect device
    // Connect name Edit
    connect(pnameEdit, SIGNAL(textChanged(QString)),
            pdevice,   SLOT(setName(QString))
           );
    connect(pidEdit, SIGNAL(textChanged(QString)),
            pdevice,   SLOT(setId(QString))
           );

    // Connect position edit
    connect(this, SIGNAL(posIsValid(QString)),
            pdevice, SLOT(setPos(QString))
           );

    // Connect dvice step edit
    connect(this, SIGNAL(deviceStepIsValid(QString)),
            pdevice,   SLOT(setStep(QString))
           );

    // Connect cmd move
    connect(pcmdMove, SIGNAL(clicked()),
            pdevice,  SLOT(move())
           );

    // Connect cmd Ok
    connect(pcmdOk,  SIGNAL(clicked()),
            pdevice, SLOT(Init())
           );
    // --------------------------------------------------------------------

}
// --------------------------------------------------

// ------- Slots
void STANDADeviceWidget::checkDevice()
{

}
void STANDADeviceWidget::downPos()
{
    QString curPosString = pposEdit->text();
    double curPos = curPosString.toDouble();
    curPos -= getdPos();
    curPosString.setNum(curPos);
    checkPosIsValid(curPosString);
}
void STANDADeviceWidget::upPos()
{
    QString curPosString = pposEdit->text();
    double curPos = curPosString.toDouble();
    curPos += getdPos();
    curPosString.setNum(curPos);
    checkPosIsValid(curPosString);
}
void STANDADeviceWidget::setdPos(QString str)
{
    setdPos(str.toDouble());
}
void STANDADeviceWidget::checkPosIsValid(QString str)
{
    //qDebug() << "STANDADeviceWidget::checkPosIsValid(" << str << ")";
    if (str != getstrPreviousPos()) {
        double curPos = str.toDouble();
        if (curPos <= getmaxPos() && curPos >= getminPos()) {
            emit posIsValid(str);
            pposEdit->setText(str);
            setstrPreviousPos(str);
        }
        else {
            emit posIsNotValid();
            pposEdit->setText(getstrPreviousPos());
        }
    }
}
void STANDADeviceWidget::checkdeviceStepIsValid(QString str)
{
    //qDebug() << "STANDADeviceWidget::checkdeviceStepIsValid(QString=" << str << ")";
    if (str != getstrPreviousdeviceStep() && str != "" /*&& str.toInt() >= getmindeviceStep()*/) {
        int curdeviceStep = str.toInt();
        
        if (curdeviceStep >= getmindeviceStep() && curdeviceStep <= getmaxdeviceStep()) {
            emit deviceStepIsValid(str);    
            setstrPreviousdeviceStep(str);
            psldrdeviceStep->setValue(str.toInt()); 
        }
        else if (curdeviceStep < getmindeviceStep()) {
            pdevicestepEdit->setText(QString().setNum(getmindeviceStep()));
        }
        else {
            pdevicestepEdit->setText(getstrPreviousdeviceStep());
        }
    }
}
void STANDADeviceWidget::checkdeviceStepIsValid(int num)
{
    //qDebug() << "STANDADeviceWidget::checkdeviceStepIsValid(int=" << num << ")";
    if (num < getmindeviceStep())       pdevicestepEdit->setText("");
    else if (num >= getmaxdeviceStep()) pdevicestepEdit->setText(QString().setNum(getmaxdeviceStep()));
    else                                pdevicestepEdit->setText(QString().setNum(num));
}
// --------------------------------------------------
