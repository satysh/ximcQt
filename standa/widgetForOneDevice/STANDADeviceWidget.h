#pragma once

#include <QWidget>

class STANDADevice;
class QPushButton;
class QLabel;
class QLineEdit;
class QSlider;
class QString;

class STANDADeviceWidget : public QWidget
{
    Q_OBJECT
public:
    STANDADeviceWidget(QWidget *parent = nullptr);
    ~STANDADeviceWidget();

    void setminPos(double pos)  { minPos=pos; }
    void setmaxPos(double pos)  { maxPos=pos; }
    void setdPos  (double dpos) { dPos=dpos; }


    double getdPos  () { return dPos; }
    double getminPos() { return minPos; }
    double getmaxPos() { return maxPos; }

    int getmindeviceStep() { return mindeviceStep; }
    int getmaxdeviceStep() { return maxdeviceStep; }

    void makeLabels();
    void makeEditors();
    void makeButtons();
    void makeLayout();
    void makeConnections();


public slots:
    void checkDevice();
    void upPos();
    void downPos();

    void setdPos(QString);

    void checkPosIsValid(QString);
    void checkdeviceStepIsValid(QString);
    void checkdeviceStepIsValid(int);

signals:
    void posIsValid(QString);
    void posIsNotValid();
    void deviceStepIsValid(QString);

private:
    void setstrPreviousPos       (QString str) { strPreviousPos=str; }
    void setstrPreviousdeviceStep(QString str) { strPreviousdeviceStep=str; }

    QString getstrPreviousPos       () { return strPreviousPos; }
    QString getstrPreviousdeviceStep() { return strPreviousdeviceStep; }

private:
    STANDADevice *pdevice;

    QPushButton *pcmdOk;
    QPushButton *pcmdMove;
    QPushButton *pcmdupPos;
    QPushButton *pcmddownPos;

    QLabel *plblName;
    QLabel *plblId;
    QLabel *plblPos;
    QLabel *plblupPos;
    QLabel *plbldownPos;
    QLabel *plblStep;

    QLineEdit *pnameEdit;
    QLineEdit *pidEdit;
    QLineEdit *pposEdit;
    QLineEdit *pdPosEdit;
    QLineEdit *pdevicestepEdit;

    QSlider *psldrdeviceStep;

    double dPos=0.01; // step of position value changing
    double minPos=0.;
    double maxPos=100.;

    int mindeviceStep=1;
    int maxdeviceStep=100;

    QString strPreviousPos="";
    QString strPreviousdeviceStep="";
};
