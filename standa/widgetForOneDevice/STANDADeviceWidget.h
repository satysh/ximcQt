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
    void setSliderValue(QString);
    void setPosBySlider(int);

    void checkPosIsValid(QString);

signals:
    void posIsValid(QString);
    void posIsNotValid();

private:
    void setstrPreviousPos(QString str) { strPreviousPos=str; }
    QString getstrPreviousPos() { return strPreviousPos; }

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
    QLineEdit *pstepEdit;

    QSlider *psldrPos;

    double dPos=0.01; // step of position value changing
    double minPos=0.;
    double maxPos=100.;

    QString strPreviousPos="";
};
