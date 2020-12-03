#pragma once

#include <QWidget>

class QString;
class QTimerEvent;

class STANDAVisualization : public QWidget
{
    Q_OBJECT
public:
    STANDAVisualization(QWidget *parent = nullptr);
    ~STANDAVisualization();

    void setXRayId (int id)     { xRayId=id; }
    void setXRayPos(double pos) { xRayPos=pos; }

    double getXRayId  () { return xRayId; }
    double getXRayPos () { return xRayPos; }
    double getXRayStep() { return xRayStep; }

    void makeXRay();

protected:
    virtual void timerEvent(QTimerEvent*);

public slots:
    void startMoveXRay();
    void stopMoveXRay ();
    void setXRayPos(QString);

    void moveAllToBasePositions();

private:
    /* XRay params */
    bool   moveXRayFlag=false;
    int    xRayId=0;
    double xRayPos=0.;
    double xRayStep=1.;
    QWidget *pxraywgt=nullptr;


};
