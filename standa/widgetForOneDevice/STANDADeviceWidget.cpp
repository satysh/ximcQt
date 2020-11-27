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
    if (pstepEdit) delete pstepEdit;
    if (psldrPos) delete psldrPos;
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
    QIntValidator *pidValidator = new QIntValidator(pidEdit);
    pidEdit->setValidator(pidValidator);
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

    pstepEdit = new QLineEdit("0");
    pstepEdit->setAlignment(Qt::AlignRight);
    QDoubleValidator *pstepValidator = new QDoubleValidator(pstepEdit);
    pstepValidator->setLocale(QLocale::C);
    pstepEdit->setValidator(pstepValidator);
    plblStep->setBuddy(pstepEdit);
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
    psldrPos = new QSlider(Qt::Horizontal);
    psldrPos->setFixedSize(sldrSize);
    psldrPos-> setSingleStep(getdPos());
    int maxSliderValue = (int)(getmaxPos()-getminPos())*0.1;
    psldrPos->setRange(0, maxSliderValue);
    psldrPos->setValue(0);
    psldrPos->setTickInterval(1);
    psldrPos->setTickPosition(QSlider::TicksBelow);
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
    pvbxLayout7->addWidget(psldrPos, 0, Qt::AlignCenter);

    // 8 column
    QVBoxLayout *pvbxLayout8 = new QVBoxLayout;
    pvbxLayout8->addWidget(plblStep, 0, Qt::AlignCenter);
    pvbxLayout8->addWidget(pstepEdit, 0, Qt::AlignCenter);

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
    // Connect step edit
    connect(pdPosEdit, SIGNAL(textChanged(QString)), SLOT(setdPos(QString)));

    // Connect position edit
    connect(pposEdit, SIGNAL(textChanged(QString)), SLOT(checkPosIsValid(QString)));
    /*connect(this,     SIGNAL(posIsValid(QString)),
            pposEdit, SLOT(setText(QString))
           );*/
    //connect(this, SIGNAL(posIsValid(QString)), SLOT(setSliderValue(QString)));
    connect(pcmddownPos, SIGNAL(clicked()), SLOT(downPos()));
    connect(pcmdupPos,   SIGNAL(clicked()), SLOT(upPos()));
    connect(psldrPos, SIGNAL(valueChanged(int)), SLOT(setPosBySlider(int)));

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

    // Connect step edit
    connect(pstepEdit, SIGNAL(textChanged(QString)),
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
void STANDADeviceWidget::setSliderValue(QString str)
{
    int sldrValue = int(10.*str.toDouble() / (getmaxPos()-getminPos()));
    psldrPos->setValue(sldrValue);
}
void STANDADeviceWidget::setPosBySlider(int num)
{
    if (num == 10)     checkPosIsValid(QString().setNum(getmaxPos()));
    else if (num == 0) checkPosIsValid(QString().setNum(getminPos()));
    double doubleValue = QString(pposEdit->text()).toDouble();
    int intPart = int(doubleValue);
    doubleValue -= (double)intPart;
    doubleValue += (double)num*0.1*(getmaxPos()-getminPos());

    checkPosIsValid(QString().setNum(doubleValue));
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
            //setSliderValue(str);
        }
        else {
            emit posIsNotValid();
            pposEdit->setText(getstrPreviousPos());
        }
    }
}
// --------------------------------------------------
