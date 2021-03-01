#include "STANDACalibratorWidget.h"

#include <QtWidgets>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    STANDACalibratorWidget wgt;
    wgt.show();

    //QObject::connect(&wgt, SIGNAL(calibrationFinished()), &app, SLOT(quit()));

    return app.exec();
}
