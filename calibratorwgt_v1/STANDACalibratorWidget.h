#pragma once

#include <map>


#include <QWidget>
#include <QString>
#include <QTextEdit>
#include <QStringList>
#include <QGroupBox>
#include <QRadioButton>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QComboBox>

#include "../deviceobj/STANDADevice.h"
#include "../settingsdata/STANDASettings.h"

class STANDACalibratorWidget : public QWidget
{
    Q_OBJECT
public:
    STANDACalibratorWidget(QWidget *parent = nullptr);
    ~STANDACalibratorWidget();

    void Print(QString str);
    void FindAvailableDevices();
    void MakeDevSelectButtons();
    void MakeControlWindow(); // CW
    void ConnectDeviceAndCW();
    void CheckOutputTxt();
    void WriteOutputTxt();

    QStringList getDevNamesList()         { return m_devNamesList; }
    QStringList getDevFriendlyNamesList() { return m_devFriendlyNamesList; }

public slots:
    void trigger(bool);


private slots:
    void setFriendlyName ();
    void setNomVoltage   ();
    void setNomSpeed     ();
    void setDevAccel     ();
    void setDevDecel     ();
    void setPosition     ();
    void setZeroPosition ();
    void setMaxPosition  ();

    void setLSw1Border(QString);
    void setLSw2Border(QString);
    void setLSw1LOW(QString);
    void setLSw2LOW(QString);
/*
private:
    void saveCurrentDevParams(QString curDevName);
*/
protected:
    virtual void timerEvent(QTimerEvent*);

private:
    QTextEdit    *m_pInfoWindow;
    QGroupBox    *m_pgbxOfDevs;
    STANDADevice *m_device;
    QVBoxLayout  *m_pmainLayout;

    /* Control Window Widgets */
    QLabel *m_pcurDevNameLable;
    QLabel *m_pcurDevVoltage;
    QLabel *m_pcurDevSpeed;
    QLabel *m_pcurDevAcceleration;
    QLabel *m_pcurDevDeceleration;
    QLabel *m_pcurDevPos;

    QLineEdit *m_pDevNameEdit;
    QLineEdit *m_pDevVoltageEdit;
    QLineEdit *m_pDevSpeedEdit;
    QLineEdit *m_pDevAccelerationEdit;
    QLineEdit *m_pDevDecelerationEdit;
    QLineEdit *m_pDevPosEdit;

    QPushButton *m_pcmdDevSetZeroPos;
    QPushButton *m_pcmdDevSetMaxPos;
    QPushButton *m_pDevNameOk;
    QPushButton *m_pDevVoltageOk;
    QPushButton *m_pDevSpeedOk;
    QPushButton *m_pDevAccelerationOk;
    QPushButton *m_pDevDecelerationOk;
    QPushButton *m_pDevPosOk;

    QPushButton *m_pDevLeft;
    QPushButton *m_pDevRight;
    QPushButton *m_pDevHome;
    QPushButton *m_pDevStop;

    QComboBox *m_pLSw1PushedPos;
    QComboBox *m_pLSw1Border;
    QComboBox *m_pLSw2PushedPos;
    QComboBox *m_pLSw2Border;

    int m_ndevs=0;
    int m_curDevId;
    QStringList m_devNamesList;
    QStringList m_devFriendlyNamesList;

    std::map<QString, STANDASettings> m_mapOfSettings; // <DeviceName, Settings>
    /*
    std::map<QString, QString> m_mapOfFriendlyNames;
    std::map<QString, int> m_mapOfDevVoltages;
    std::map<QString, int> m_mapOfDevSpeeds;
    std::map<QString, int> m_mapOfDevZeroPoses;
    std::map<QString, int> m_mapOfDevMaxPoses;
    std::map<QString, int> m_mapOfDevAccel;
    std::map<QString, int> m_mapOfDevDecel;*/
/*
    QString m_curFriendlyName="";
    int m_curDevVoltage=0;
    int m_curDevSpeed=0;
    int m_curDevZeroPos=0;
    int m_curDevMaxPos=0;
    int m_curDevAccel=0;
    int m_curDevDecel=0;
    int m_curDevPos;*/
};
