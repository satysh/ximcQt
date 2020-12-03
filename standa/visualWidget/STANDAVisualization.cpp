#include <QString>
#include <QPoint>
#include <QPalette>
#include <QPixmap>
#include <QTimerEvent>
#include <QDebug>

#include <QtWidgets>

#include "STANDAVisualization.h"

STANDAVisualization::STANDAVisualization(QWidget *parent/* = nullptr*/)
    : QWidget(parent)
{

    setFixedSize(1000, 1000);
    makeXRay();
    moveAllToBasePositions();

    startTimer(10);
}

STANDAVisualization::~STANDAVisualization()
{
}

void STANDAVisualization::makeXRay()
{
    pxraywgt = new QWidget(this);
    QPalette pal;
    QPixmap pix(":/xray.png");
    pal.setBrush(backgroundRole(), QBrush(pix));
    pxraywgt->setPalette(pal);
    pxraywgt->setFixedSize(pix.width(), pix.height());
    pxraywgt->setAutoFillBackground(true);
}
void STANDAVisualization::timerEvent(QTimerEvent* ptev)
{
    int timerId = ptev->timerId();
    timerId++; timerId--;
    //qDebug() << "STANDAVisualization::timerEvent(id=" << timerId << ")";
    if (moveXRayFlag) {
        QPoint point = pos();
        int curPos = point.rx();
        if (curPos > getXRayPos())      curPos -= (int)getXRayStep();
        else if (curPos < getXRayPos()) curPos += (int)getXRayStep();
        point.setX(curPos);
        move(point);

        qDebug() << "moving.. pos=" << curPos;
        if (curPos == getXRayPos()) stopMoveXRay();
    }
}
void STANDAVisualization::startMoveXRay()
{
    moveXRayFlag=true;
}

void STANDAVisualization::stopMoveXRay()
{
    moveXRayFlag=false;
}

void STANDAVisualization::setXRayPos(QString str)
{
    setXRayPos(str.toDouble());
}

void STANDAVisualization::moveAllToBasePositions()
{
    pxraywgt->move(500, 500);
}
