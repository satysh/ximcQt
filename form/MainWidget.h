#pragma once

#include <QtWidgets>
class RowOfWidgets;

QT_BEGIN_NAMESPACE
namespace MainWidgetConsts { const int numOfWidgsInTheRow=5;}
QT_END_NAMESPACE

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

    void MakeLayout();
    void ConnectWidgets();

public slots:
    void Print();
    void LocalOkIsClicked();
signals:
    void sendString(QString);

private:
    RowOfWidgets *pRowOfWgs[MainWidgetConsts::numOfWidgsInTheRow]={nullptr};
    QPushButton  *pCmdMain=nullptr;
    QTextEdit    *ptxtEdit=nullptr;
};
