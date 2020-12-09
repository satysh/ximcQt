#include <QtWidgets>

#include "../devicewgt/STANDADeviceWidget.h"
#include "../view/STANDAVisualization.h"
#include "../deviceobj/STANDADevice.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    STANDADevice device;
    STANDADeviceWidget devWgt;
    STANDAVisualization viewWgt;
   
    QObject::connect(&devWgt, SIGNAL(turnOnDevice()),
                     &device, SLOT(Init())
                    );
    QObject::connect(&devWgt, SIGNAL(turnOffDevice()),
                     &device, SLOT(Delete())
                    );
    QObject::connect(&devWgt, SIGNAL(posIsValid(QString)),
                     &device, SLOT(setPos(QString))
                    );
    QObject::connect(&devWgt, SIGNAL(startMoveDevice()),
                     &device, SLOT(move())
                    );
    QObject::connect(&devWgt, SIGNAL(stopMoveDevice()),
                     &device, SLOT(stop())
                    );

    // Connect device widget and view widget
    QObject::connect(&devWgt,  SIGNAL(posIsValid(QString)),
                     &viewWgt, SLOT  (setXRayPos(QString))
                    );
    QObject::connect(&devWgt,  SIGNAL(startMoveDevice()),
                     &viewWgt, SLOT  (startMoveXRay())
                    );
    QObject::connect(&devWgt,  SIGNAL(stopMoveDevice()),
                     &viewWgt, SLOT  (stopMoveXRay())
                    );
    QObject::connect(&viewWgt, SIGNAL(xRayIsStopped()),
                     &devWgt,  SLOT  (setMoveMod())
                    );

    devWgt.show();
    viewWgt.show();

    return app.exec();
}
