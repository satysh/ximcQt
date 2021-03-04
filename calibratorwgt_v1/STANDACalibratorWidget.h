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
    void MakeControlWindow();

public slots:
    void trigger(bool);

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

    std::map<QString, QString> m_mapDevNameVsFriendlyName;
};
