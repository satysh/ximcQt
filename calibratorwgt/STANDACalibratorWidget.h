#pragma once

#include <vector>

#include <QWidget>
#include <QString>
#include <QStringList>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>

#include "../deviceobj/STANDADevice.h"

class STANDACalibratorWidget : public QWidget
{
    Q_OBJECT
public:
    STANDACalibratorWidget(QWidget *parent = nullptr);
    ~STANDACalibratorWidget();

    void LoadAvailableDevices();
    void addDevice(QString devName) { m_devNamesList << devName; }

    void CloseCurDevice();

    void setndevs(int ndevs) { m_ndevs=ndevs; }
    void setcurIndex(int index) { m_curIndex=index; }

    int getndevs() { return m_ndevs; }
    //int getcurIndex() { return m_curIndex; }

    QString getcurDevName();

private:
	void writeOutputTxt();
    void connectButtons();
    bool check();

protected:
    virtual void timerEvent(QTimerEvent*);

private slots:
    void InitCalibration();
    void setNomVoltage();
    void setNomSpeed();
    void setZeroPos();
    void setMaxPos();
    void next();
    void finish();
    void Close();

signals:
    void successfull();
    void failed();
    void calibrationFinished();

private:
    int m_ndevs=0;
    int m_curIndex=0;
    QStringList m_devNamesList;
    std::vector<int> m_vDevVoltages;
    std::vector<int> m_vDevSpeeds;
    std::vector<int> m_vDevZeroPoses;
    std::vector<int> m_vDevMaxPoses;

    STANDADevice *m_pcurDevice=nullptr;

    /*Labels*/
    QLabel *m_pCurVoltageLabel;
    QLabel *m_pCurSpeedLabel;
    QLabel *m_pCurPosLabel;
    QLabel *m_pDevNameLabel;

    /*Editors*/
    QLineEdit *m_pNomVoltageEdit;
    QLineEdit *m_pNomSpeedEdit;

    QTextEdit *m_pInfoWindow;

    /*Buttons*/
    QPushButton *m_pSetNomVoltage;
    QPushButton *m_pSetNomSpeed;
    QPushButton *m_pSetZeroPos;
    QPushButton *m_pSetMaxPos;
    QPushButton *m_pStop;
    QPushButton *m_pHome;
    QPushButton *m_pLeft;
    QPushButton *m_pRight;
    QPushButton *m_pNext;

    bool m_nomVoltageIsSet=false;
    bool m_nomSpeedIsSet=false;
    bool m_zeroPosIsSet=false;
    bool m_maxPosIsSet=false;
};
