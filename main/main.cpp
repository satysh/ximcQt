#include <QtWidgets>

#include "../devicewgt/STANDADeviceWidget.h"
#include "../view/STANDAVisualization.h"
#include "../deviceobj/STANDADevice.h"
#include "../devloader/STANDALoader.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QWidget      mainWgt;
    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    STANDALoader devsloader;
    STANDADevice* device;
    STANDADeviceWidget* devWgt;

    //STANDAVisualization viewWgt;

    devsloader.findDevices(); 
    const int nDevs = devsloader.getnDevs();

    if (nDevs > 0) {
        device = new STANDADevice[nDevs];
        devWgt = new STANDADeviceWidget[nDevs];
    }
    else {
    	return -1;
    }

    for (int i=0; i<nDevs; i++) {
      QObject::connect(&devWgt[i], SIGNAL(turnOnDevice()),
                       &device[i], SLOT(Init())
                      );
      QObject::connect(&devWgt[i], SIGNAL(turnOffDevice()),
                       &device[i], SLOT(Delete())
                      );
      QObject::connect(&devWgt[i], SIGNAL(posIsValid(QString)),
                       &device[i], SLOT(setPos(QString))
                      );
      QObject::connect(&devWgt[i], SIGNAL(startMoveDevice()),
                       &device[i], SLOT(move())
                      );
      QObject::connect(&devWgt[i], SIGNAL(stopMoveDevice()),
                       &device[i], SLOT(stop())
                      );
      QObject::connect(&device[i], SIGNAL(deviceMoveEnd()),
                       &devWgt[i], SLOT(setMoveMod())
                      );
      devWgt[i].setmaxPos(73000);
      device[i].setName(devsloader.getDevName(i));
      device[i].Init();
      devWgt[i].Init();
      pvbxLayout->addWidget(&devWgt[i]);
    }
    
    mainWgt.setLayout(pvbxLayout);

    // Connect device widget and view widget
    /*
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
    viewWgt.show();
    */
    mainWgt.show();


    return app.exec();
}
