#include <QString>
#include <QPoint>
#include <QPalette>
#include <QTimerEvent>
#include <QDebug>

#include "STANDASim.h"

STANDASim::STANDASim(QWidget *parent/* = nullptr*/)
    : QWidget(parent)
{
    QPalette pal;
    pal.setBrush(backgroundRole(), QBrush(QPixmap(":/standa.jpeg")));
    setPalette(pal);
    resize(200, 75);
    setAutoFillBackground(true);
    startTimer(100);
}

STANDASim::~STANDASim()
{
}

void STANDASim::timerEvent(QTimerEvent* ptev)
{
    //qDebug() << "STANDASim::timerEvent";
    if (moveFlag) {
        QPoint point = pos();
        int cX = point.rx();
        if (cX > getPos())      cX -= step;
        else if (cX < getPos()) cX += step;
        point.setX(cX);
        move(point);

        qDebug() << "moving.. pos=" << cX;
        if (cX == getPos()) stop();
    }
}
void STANDASim::start()
{
    moveFlag=true;
}

void STANDASim::stop()
{
    moveFlag=false;
}

void STANDASim::setPos(QString str)
{
    setPos(str.toInt());

}
