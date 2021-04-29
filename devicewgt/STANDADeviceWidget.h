#pragma once

#include <QWidget>
#include <QLabel>

class QPushButton;
class QLineEdit;
class QSlider;
class QString;

class STANDADeviceWidget : public QWidget
{
    Q_OBJECT
public:
    STANDADeviceWidget(QWidget *parent = nullptr);
    ~STANDADeviceWidget();

    void Init();

    void setdPos  (double dpos) { dPos=dpos; }
    void setminPos(double pos)  { m_minPos=pos; }
    void setmaxPos(double pos)  { m_maxPos=pos; }
    void setminPosInDeviceCodes(int pos) { m_minPosInDeviceCodes=pos; }
    void setmaxPosInDeviceCodes(int pos) { m_maxPosInDeviceCodes=pos; }
    void setDeviceName(QString name) { plblName->setText(name); }


    double getdPos  () { return dPos; }
    double getminPos() { return m_minPos; }
    double getmaxPos() { return m_maxPos; }

    int getmaxStepNumber();

private:
    void makeLabels();
    void makeEditors();
    void makeButtons();
    void makeLayout();
    void makeBaseConnections();

public slots:
    void setdPos(QString);
/*
    void setminPos(QString);
    void setmaxPos(QString);
    void setDeviceId(QString);
    void setDeviceToBasePosition();
*/
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
    void setSliderToFixedPos();

    void posIsValidDebug(QString);


signals:
    void turnOnDevice();
    void turnOffDevice();
    void posIsValid(QString);
    void posIsNotValid();
    void startMoveDevice(QString);
    void stopMoveDevice ();
    void moveDeviceTo(int);

private:
    void setPreviousPos       (QString str) { strPreviousPos=str; }

    QString getPreviousPos       () { return strPreviousPos; }

private:
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

    QSlider *psldr;

    bool sliderActiveFlag=false;

    double dPos=0.01; // step of position value changing
    double m_minPos=0.;
    double m_maxPos=1000.;
    double m_currentPosition=0.;
    int    m_currentPositionInDevCodes=0;
    int m_minPosInDeviceCodes=0;
    int m_maxPosInDeviceCodes=0;

    QString strPreviousPos="";
    QString m_strCurrentPosition="";
};
