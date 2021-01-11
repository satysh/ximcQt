#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimerEvent>

#include "STANDASettingsWidget.h"

STANDASettingsWidget::STANDASettingsWidget(QWidget *parent/* = nullptr*/)
    : QWidget(parent)
{
    m_pVoltageEdit = new QLineEdit;
    m_pCurrentEdit = new QLineEdit;
    m_pSpeedEdit = new QLineEdit;

    QVBoxLayout* pvbxLayout1 = new QVBoxLayout;
    pvbxLayout1->addWidget(new QLabel("voltage"));
    pvbxLayout1->addWidget(m_pVoltageEdit);

    QVBoxLayout* pvbxLayout2 = new QVBoxLayout;
    pvbxLayout2->addWidget(new QLabel("current"));
    pvbxLayout2->addWidget(m_pCurrentEdit);

    QVBoxLayout* pvbxLayout3 = new QVBoxLayout;
    pvbxLayout3->addWidget(new QLabel("speed"));
    pvbxLayout3->addWidget(m_pSpeedEdit);

    QHBoxLayout* phbxLayout = new QHBoxLayout;
    phbxLayout->addLayout(pvbxLayout1);
    phbxLayout->addLayout(pvbxLayout2);
    phbxLayout->addLayout(pvbxLayout3);

    setLayout(phbxLayout);

    startTimer(10);
}

STANDASettingsWidget::~STANDASettingsWidget()
{
	
}

void STANDASettingsWidget::timerEvent(QTimerEvent* ptmrEv)
{
    if (ptmrEv->timerId() > 0) {
        emit checkVoltage();
        emit checkCurrent();
        emit checkSpeed();
    }
}

void STANDASettingsWidget::getNomVoltage(QString voltage)
{
    m_pVoltageEdit->setText(voltage);
}
void STANDASettingsWidget::getNomCurrent(QString current)
{
    m_pCurrentEdit->setText(current);
}
void STANDASettingsWidget::getNomSpeed(QString speed)
{
    m_pSpeedEdit->setText(speed);
}

