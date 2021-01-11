#pragma once

#include <QWidget>

class QLineEdit;
class QTimerEvent;

#include <QString>

class STANDASettingsWidget : public QWidget
{
    Q_OBJECT
public:
	STANDASettingsWidget(QWidget *parent = nullptr);
	~STANDASettingsWidget();

protected:
    virtual void timerEvent(QTimerEvent*);

public slots:
    void getNomVoltage(QString);
    void getNomCurrent(QString);
    void getNomSpeed(QString);

signals:
    void checkVoltage();
    void checkCurrent();
    void checkSpeed();

private:
    QLineEdit* m_pVoltageEdit;
    QLineEdit* m_pCurrentEdit;
    QLineEdit* m_pSpeedEdit;
};
