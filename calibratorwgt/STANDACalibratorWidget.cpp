#include "STANDACalibratorWidget.h"

#include <QDebug>

STANDACalibratorWidget::STANDACalibratorWidget(QWidget *parent/* = nullptr*/)
    : QWidget(parent)
{
    qDebug() << "STANDACalibratorWidget::STANDACalibratorWidget";
}

STANDACalibratorWidget::~STANDACalibratorWidget()
{
    qDebug() << "STANDACalibratorWidget::~STANDACalibratorWidget";
}
