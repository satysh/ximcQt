#include "STANDACalibratorWidget.h"

#include <QtWidgets>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    STANDACalibratorWidget wgt;

    wgt.show();

    return app.exec();
}
