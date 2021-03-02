#pragma once

#include <vector>

#include <QWidget>
#include <QString>
#include <QTextEdit>
#include <QStringList>
#include <QGroupBox>

#include "../deviceobj/STANDADevice.h"

class STANDACalibratorWidget : public QWidget
{
    Q_OBJECT
public:
    STANDACalibratorWidget(QWidget *parent = nullptr);
    ~STANDACalibratorWidget();

    void Print(QString str);
    void FindAvailableDevices();
    void MakeDevButtons();

public slots:
    void test();

protected:
    virtual void timerEvent(QTimerEvent*);

private:
    QTextEdit    *m_pInfoWindow;
    QGroupBox    *m_pgbxOfDevs;
    STANDADevice *m_device;

    int m_ndevs=0;
    int m_curDevId;
    QStringList m_devNamesList;
    QStringList m_devFriendlyNamesList;
};
