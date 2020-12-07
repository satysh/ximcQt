#include <QtWidgets>
#include "STANDAVisualization.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    STANDAVisualization wgt;

    wgt.show();
    return app.exec();
}
