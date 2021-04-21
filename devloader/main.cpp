#include <QtWidgets>
#include "STANDALoader.h"

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
    QWidget      wgt;
    STANDALoader loader(&wgt);

    loader.findAvailableDevices();

    wgt.show();
	return app.exec();
}
