#pragma once

#include <vector>

#include <QWidget>
#include <QString>
#include <QTextEdit>
#include <QStringList>
#include <QGroupBox>
#include <QVector>
#include <QRadioButton>

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
    void trigger(bool);

protected:
    virtual void timerEvent(QTimerEvent*);

private:
    QTextEdit    *m_pInfoWindow;
    QGroupBox    *m_pgbxOfDevs;
    STANDADevice *m_device;

    QVector<QRadioButton*> m_vectorOfDevs;

    int m_ndevs=0;
    int m_curDevId;
    QStringList m_devNamesList;
    QStringList m_devFriendlyNamesList;
};
