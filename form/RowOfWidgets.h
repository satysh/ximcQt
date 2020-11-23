#pragma once

#include <QtWidgets>

class RowOfWidgets : public QWidget
{
    Q_OBJECT
public:
    RowOfWidgets(QWidget *parent = nullptr);
    ~RowOfWidgets();

    void MakeLayout();
    void ConnectWidgets();
    void setLabelName(QString name) { pLbl->setText(name); }
    QString getRowInfo();

public slots:
    void Print();
    void LocalOkIsClicked();
    void getDouble(double);
    void getInt(int);
signals:
    void sendString(QString);
    void sendInt(int in);
    void sendDouble(double d);

private:
    QLabel         *pLbl=nullptr;
    QLabel         *pLblInfo=nullptr;
    QDoubleSpinBox *pDSpnBox=nullptr;
    QSlider        *pSldr=nullptr;
    QPushButton    *pCmd=nullptr;
};
