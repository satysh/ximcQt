#include "RowOfWidgets.h"
#include "MainWidget.h"

MainWidget::MainWidget(QWidget *parent/*=nullptr*/)
    : QWidget(parent)
{
    MakeLayout();
    ConnectWidgets();
}

MainWidget::~MainWidget()
{
    for (int i=0; i<MainWidgetConsts::numOfWidgsInTheRow; i++)
        if (pRowOfWgs[i]) delete pRowOfWgs[i];
    if (pCmdMain)  delete pCmdMain;
    if (ptxtEdit)  delete ptxtEdit;
}

void MainWidget::MakeLayout()
{
    for (int i=0; i<MainWidgetConsts::numOfWidgsInTheRow; i++) {
        pRowOfWgs[i] = new RowOfWidgets;
        pRowOfWgs[i]->setLabelName( "name_"+QString().setNum(i));
    }
    ptxtEdit = new QTextEdit;
    pCmdMain = new QPushButton("OK");

    QVBoxLayout *pvbxLayout = new QVBoxLayout;
    for (int i=0; i<MainWidgetConsts::numOfWidgsInTheRow; i++) {
        pvbxLayout->addWidget(pRowOfWgs[i]);
    }
    pvbxLayout->addWidget(ptxtEdit);
    pvbxLayout->addWidget(pCmdMain);
    this->setLayout(pvbxLayout);
}

void MainWidget::ConnectWidgets()
{
    connect(pCmdMain, SIGNAL(clicked()),
            this, SLOT(Print())
           );
    for (int i=0; i<MainWidgetConsts::numOfWidgsInTheRow; i++) {
        connect(pRowOfWgs[i], SIGNAL(sendString(QString)),
                ptxtEdit, SLOT(setText(QString))
               );
    }
}

void MainWidget::Print()
{
    QString curStr;
    for (int i=0; i<MainWidgetConsts::numOfWidgsInTheRow; i++) {
        curStr += pRowOfWgs[i]->getRowInfo();
    }
    ptxtEdit->setText(curStr);
}

void MainWidget::LocalOkIsClicked()
{
  qDebug() << "?";
}
