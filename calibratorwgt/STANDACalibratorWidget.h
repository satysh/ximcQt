#pragma once

#include <vector>

//#include <ximc.h>

#include <QWidget>
#include <QString>
#include <QStringList>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>

class STANDACalibratorWidget : public QWidget
{
    Q_OBJECT
public:
    STANDACalibratorWidget(QWidget *parent = nullptr);
    ~STANDACalibratorWidget();

    void LoadAvailableDevices();
    void addDevice(QString devName) { m_devNamesList << devName; }

    void InitDev(int index);
    void DeleteDev(int index);

    void setndevs(int ndevs) { m_ndevs=ndevs; }

    int getndevs() { return m_ndevs; }

private:
    int m_ndevs=0;
    int m_curIndex=0;
    QStringList m_devNamesList;
    std::vector<int> m_vDevVoltages;
    std::vector<int> m_vDevSpeeds;
    std::vector<int> m_vDevZeroPoses;
    std::vector<int> m_vDevMaxPoses;

    /*Labels*/
    QLabel *m_pNomVoltageLabel;
    QLabel *m_pNomSpeedLabel;
    QLabel *m_pCurPosLabel;

    /*Editors*/
    QLineEdit *m_pNomVoltageEdit;
    QLineEdit *m_pNomSpeedEdit;

    QTextEdit *m_pInfoWindow;

    /*Buttons*/
    QPushButton *m_pSetNomVoltage;
    QPushButton *m_pSetNomSpeed;
    QPushButton *m_pSetZeroPos;
    QPushButton *m_pSetMaxPos;
    QPushButton *m_pLeft;
    QPushButton *m_pRight;
    QPushButton *m_pNext;

};
