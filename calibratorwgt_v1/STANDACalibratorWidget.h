#pragma once

#include <vector>

#include <QWidget>

#include "../deviceobj/STANDADevice.h"

class STANDACalibratorWidget : public QWidget
{
    Q_OBJECT
public:
    STANDACalibratorWidget(QWidget *parent = nullptr);
    ~STANDACalibratorWidget();


protected:
    virtual void timerEvent(QTimerEvent*);

private:

};
