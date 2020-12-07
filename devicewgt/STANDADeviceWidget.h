#pragma once

#include <QWidget>

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

    void setdPos  (double dpos) { dPos=dpos; }
    void setminPos(double pos)  { minPos=pos; }
    void setmaxPos(double pos)  { maxPos=pos; }


    double getdPos  () { return dPos; }
    double getminPos() { return minPos; }
    double getmaxPos() { return maxPos; }

    int getmaxStepNumber();

    void makeLabels();
    void makeEditors();
    void makeButtons();
    void makeLayout();
    void makeConnections();

public slots:
    void setdPos(QString);
    void setminPos(QString);
    void setmaxPos(QString);
    void setDeviceName(QString);
    void setDeviceId(QString);
    void setDeviceBasePosition(QString);

    void setMoveMod();
    void setStopMod();


private slots:
    void setPosBySlider(int);
    void setSliderToPos(QString);
    //void setSliderPos(int);

    void checkDevice();
    void upPos();
    void downPos();
    void checkUserTypedPosIsValid(QString);
    void moveStart();
    void moveStop();

    void posIsValidDebug(QString);

signals:
    void posIsValid(QString);
    void posIsNotValid();
    void startMoveDevice();
    void stopMoveDevice ();

private:
    void setPreviousPos       (QString str) { strPreviousPos=str; }

    QString getPreviousPos       () { return strPreviousPos; }

private:
    QPushButton *pcmdOk;
    QPushButton *pcmdMove;
    QPushButton *pcmdupPos;
    QPushButton *pcmddownPos;

    QLabel *plblName;
    QLabel *plblId;
    QLabel *plblPos;
    QLabel *plblupPos;
    QLabel *plbldownPos;

    QLineEdit *pnameEdit;
    QLineEdit *pidEdit;
    QLineEdit *pposEdit;
    QLineEdit *pdPosEdit;

    QSlider *psldrdeviceStep;

    bool sliderActiveFlag=false;

    double dPos=0.01; // step of position value changing
    double minPos=0.;
    double maxPos=1000.;

    QString strPreviousPos="";
};
