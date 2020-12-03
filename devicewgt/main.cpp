#include <QtWidgets>
#include "STANDADeviceWidget.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    STANDADeviceWidget wgt;

    wgt.show();
    return app.exec();
}
