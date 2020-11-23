#include "RowOfWidgets.h"

RowOfWidgets::RowOfWidgets(QWidget *parent/*=nullptr*/)
    : QWidget(parent)
{
    MakeLayout();
    ConnectWidgets();
}

RowOfWidgets::~RowOfWidgets()
{

    if (pLbl)     delete pLbl;
    if (pDSpnBox) delete pDSpnBox;
    if (pSldr)    delete pSldr;
    if (pCmd)     delete pCmd;
}

void RowOfWidgets::MakeLayout()
{
    pLbl = new QLabel("empty");
    pDSpnBox = new QDoubleSpinBox;
    pDSpnBox->setMinimum(0.);
    pDSpnBox->setMaximum(10.);
    pDSpnBox->setValue(0.);
    pDSpnBox->setSingleStep(0.01);
    pLblInfo = new QLabel("0");
    pSldr = new QSlider(Qt::Horizontal);
    pSldr->setRange(0, 10);
    pSldr->setPageStep(2);
    pSldr->setTickInterval(1);
    pSldr->setTickPosition(QSlider::TicksBelow);
    pCmd = new QPushButton("Ok");

    QHBoxLayout *phbxLayout = new QHBoxLayout;
    phbxLayout->addWidget(pLbl);
    phbxLayout->addWidget(pDSpnBox);
    phbxLayout->addWidget(pLblInfo);
    phbxLayout->addWidget(pSldr);
    phbxLayout->addWidget(pCmd);

    phbxLayout->setSpacing(20);

    this->setLayout(phbxLayout);
}

void RowOfWidgets::ConnectWidgets()
{
    connect(pCmd, SIGNAL(clicked()),
            this, SLOT(Print())
           );
    connect(pDSpnBox, SIGNAL(valueChanged(double)),
            pLblInfo, SLOT(setNum(double))
           );
    connect(pDSpnBox, SIGNAL(valueChanged(double)),
            this, SLOT(getDouble(double))
           );
    connect(pSldr, SIGNAL(valueChanged(int)),
            pLblInfo, SLOT(setNum(int))
           );
    connect(pSldr, SIGNAL(valueChanged(int)),
            this, SLOT(getInt(int))
           );
    connect(this, SIGNAL(sendInt(int)),
            pSldr, SLOT(setValue(int))
           );
    connect(this, SIGNAL(sendDouble(double)),
            pDSpnBox, SLOT(setValue(double))
           );
}

QString RowOfWidgets::getRowInfo()
{
    QString curStr(pLbl->text());
    curStr +=  "\t" + pDSpnBox->text();
    curStr +=  "\t" + QString().setNum(pSldr->value()) + "\n";
    return curStr;
}
void RowOfWidgets::Print()
{
    qDebug() << "[info]: " << pLbl->text() << ", " << pDSpnBox->text() << ", "
             << pSldr->value();
    QString curStr(pLbl->text());
    curStr +=  "\t" + pDSpnBox->text() + "\n";
    emit sendString(curStr); // make signal
}

void RowOfWidgets::LocalOkIsClicked()
{
}

void RowOfWidgets::getDouble(double d)
{
    int curInt = (int)d;
    emit sendInt(curInt);
}

void RowOfWidgets::getInt(int in)
{
    double curD = (double)in;
    emit sendDouble(curD);
}
