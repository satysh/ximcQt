#include <QDebug>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
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
    makeButtons();
    makeLabels();
    makeEditors();
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
    if (pstepEdit) delete pstepEdit;
}

// -------- make base widgets that the class includes
void STANDADeviceWidget::makeButtons()
{
    pcmdOk   = new QPushButton("Ok");
    pcmdMove = new QPushButton("move");
}
void STANDADeviceWidget::makeLabels()
{
    plblName = new QLabel("&name");
    plblId   = new QLabel("&id");
    plblPos  = new QLabel("&pos [mm]");
    plblStep = new QLabel("&step");
}
void STANDADeviceWidget::makeEditors()
{
    pnameEdit = new QLineEdit;
    plblName->setBuddy(pnameEdit);

    pidEdit   = new QLineEdit;
    QIntValidator *pidValidator = new QIntValidator;
    pidEdit->setValidator(pidValidator);
    plblId->setBuddy(pidEdit);

    pposEdit    = new QLineEdit;
    QDoubleValidator *pposValidator = new QDoubleValidator;
    pposValidator->setLocale(QLocale::C);
    plblPos->setBuddy(pposEdit);

    pstepEdit = new QLineEdit;
    QDoubleValidator *pstepValidator = new QDoubleValidator;
    pstepValidator->setLocale(QLocale::C);
    plblStep->setBuddy(pstepEdit);
}
void STANDADeviceWidget::makeConnections()
{
    connect(pnameEdit, SIGNAL(textChanged(QString)),
            pdevice,   SLOT(setName(QString))
           );
    connect(pnameEdit, SIGNAL(textChanged(QString)),
            pdevice,   SLOT(setId(QString))
           );
    connect(pposEdit, SIGNAL(textChanged(QString)),
            pdevice,  SLOT(setPos(QString))
           );
    connect(pstepEdit, SIGNAL(textChanged(QString)),
            pdevice,   SLOT(setStep(QString))
           );
    connect(pcmdOk,  SIGNAL(clicked()),
            pdevice, SLOT(Init())
           );
    connect(pcmdMove, SIGNAL(clicked()),
            pdevice,  SLOT(move())
           );
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
    pvbxLayout4->addWidget(plblStep, 0, Qt::AlignCenter);
    pvbxLayout4->addWidget(pstepEdit, 0, Qt::AlignCenter);

    // 5 column
    QVBoxLayout *pvbxLayout5 = new QVBoxLayout;
    pvbxLayout5->addWidget(pcmdOk);
    pvbxLayout5->addWidget(pcmdMove);

    // 1 row
    QHBoxLayout *phbxLayout = new QHBoxLayout;
    phbxLayout->addLayout(pvbxLayout1);
    phbxLayout->addLayout(pvbxLayout2);
    phbxLayout->addLayout(pvbxLayout3);
    phbxLayout->addLayout(pvbxLayout4);
    phbxLayout->addLayout(pvbxLayout5);

    this->setLayout(phbxLayout);
}
// --------------------------------------------------
