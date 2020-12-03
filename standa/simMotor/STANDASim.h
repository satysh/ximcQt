#pragma once

#include <QWidget>

class QString;
class QTimerEvent;

class STANDASim : public QWidget
{
    Q_OBJECT
public:
    STANDASim(QWidget *parent = nullptr);
    ~STANDASim();

    void setId (int i) { id=i; }
    void setPos(int i) { curPos=i; }

    int getId () { return id; }
    int getPos() { return curPos; }

protected:
    virtual void timerEvent(QTimerEvent*);

public slots:
    void start();
    void stop();
    void setPos(QString);

private:
    int id=0;
    int curPos=0;
    int step=1;

    bool moveFlag=false;
};
