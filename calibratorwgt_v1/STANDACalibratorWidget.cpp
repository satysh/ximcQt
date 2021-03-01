#include "STANDACalibratorWidget.h"

#include <ximc.h>

#include <QDebug>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimerEvent>


STANDACalibratorWidget::STANDACalibratorWidget(QWidget *parent/* = nullptr*/)
    : QWidget(parent)
{
}

STANDACalibratorWidget::~STANDACalibratorWidget()
{
}

void STANDACalibratorWidget::timerEvent(QTimerEvent*)
{
}
