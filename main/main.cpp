#include <QtWidgets>

#include "../devicewgt/STANDADeviceWidget.h"
#include "../view/STANDAVisualization.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    STANDADeviceWidget devWgt;
    STANDAVisualization viewWgt;


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