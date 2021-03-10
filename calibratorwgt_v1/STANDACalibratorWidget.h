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

#include "../deviceobj/STANDADevice.h"

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
    void WriteOutputTxt();

    void setStageName(QString name) { m_device->setStageName(name); }
    void setDevVoltage(int voltage) { m_device->setNomVoltage(voltage); }
    void setDevSpeed(int speed)     { m_device->setNomSpeed(speed); }
    void setDevAccel(int accel)     { m_device->setDevAccel(accel); }
    void setDevDecel(int decel)     { m_device->setDevDecel(decel); }

public slots:
    void trigger(bool);

    void setStageName () { m_curStageName=m_pDevNameEdit->text(); setStageName(m_curStageName); }
    void setDevVoltage() { m_curDevVoltage=m_pDevVoltageEdit->text().toDouble(); setDevVoltage(m_curDevVoltage); }
    void setDevSpeed  () { m_curDevSpeeds=m_pDevSpeedEdit->text().toDouble(); setDevSpeed(m_curDevSpeeds); }
    void setDevZeroPos() { m_curDevZeroPos=m_pcurDevPos->text().toDouble(); }
    void setDevMaxPos () { m_curDevMaxPos=m_pcurDevPos->text().toDouble(); }
    void setDevDecel  () { m_curDevDecel=m_pDevDecelerationEdit->text().toDouble(); setDevDecel(m_curDevDecel); }

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
    QLabel *m_pcurDevDeceleration;
    QLabel *m_pcurDevPos;

    QLineEdit *m_pDevNameEdit;
    QLineEdit *m_pDevVoltageEdit;
    QLineEdit *m_pDevSpeedEdit;
    QLineEdit *m_pDevDecelerationEdit;

    QPushButton *m_pcmdDevSetZeroPos;
    QPushButton *m_pcmdDevSetMaxPos;
    QPushButton *m_pDevNameOk;
    QPushButton *m_pDevVoltageOk;
    QPushButton *m_pDevSpeedOk;
    QPushButton *m_pDevDecelerationOk;

    QPushButton *m_pDevLeft;
    QPushButton *m_pDevRight;
    QPushButton *m_pDevHome;
    QPushButton *m_pDevStop;

    QRadioButton *m_pDevRenameMe1; // TODO
    QRadioButton *m_pDevRenameMe2; // TODO

    int m_ndevs=0;
    int m_curDevId;
    QStringList m_devNamesList;
    QStringList m_devFriendlyNamesList;

    //std::map<QString, QString> m_mapDevNameVsFriendlyName; // TODO remove It
    std::map<QString, int> m_mapOfDevVoltages;
    std::map<QString, int> m_mapOfDevSpeeds;
    std::map<QString, int> m_mapOfDevZeroPoses;
    std::map<QString, int> m_mapOfDevMaxPoses;
    std::map<QString, int> m_mapOfDevDecel;

    QString m_curStageName="";
    int m_curDevVoltage=0;
    int m_curDevSpeeds=0;
    int m_curDevZeroPos=0;
    int m_curDevMaxPos=0;
    int m_curDevDecel=0;
};
