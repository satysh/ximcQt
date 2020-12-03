#include <QtWidgets>
#include "STANDADeviceWidget.h"
#include "../visualWidget/STANDAVisualization.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    STANDADeviceWidget devWgt;
    STANDAVisualization visWgt;


    QObject::connect(&devWgt, SIGNAL(posIsValid(QString)),
                     &visWgt, SLOT(setXRayPos(QString))
                    );
    QObject::connect(&devWgt, SIGNAL(startMoveDevice()),
                     &visWgt, SLOT(startMoveXRay())
                    );

    devWgt.show();
    visWgt.show();
    return app.exec();
}
