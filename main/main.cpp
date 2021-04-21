#include <QtWidgets>

#include "../devicewgt/STANDADeviceWidget.h"
#include "../view/STANDAVisualization.h"
#include "../deviceobj/STANDADevice.h"
#include "../devloader/STANDALoader.h"
#include "../settingsdata/STANDASettings.h"
//#include "../devsettings/STANDASettingsWidget.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QWidget      mainWgt;
    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    STANDALoader devsloader;
    std::vector<STANDADevice*> vOfdevs;
    std::vector<STANDADeviceWidget*> vOfDevWgts;
    //STANDASettingsWidget* devSettWgt;

    //STANDAVisualization viewWgt;

    devsloader.findDevices();
    devsloader.findInputFile();
    qDebug() << "loader result is " << devsloader.getResult();

    int nDevs = devsloader.getnDevs();
    qDebug() << "loader found devs=" << nDevs;

    if (nDevs < 1) {
      qDebug() << "no devs were found!";
      return -1;
    }

    for (int i=0; i<nDevs; i++) {
        STANDADevice* p_curdevice = new STANDADevice(&mainWgt);
        STANDADeviceWidget* p_curdevWgt = new STANDADeviceWidget(&mainWgt);
        //devSettWgt = new STANDASettingsWidget[nDevs];
        QObject::connect(p_curdevWgt, SIGNAL(turnOnDevice()),
                         p_curdevice, SLOT(Init())
                        );
        QObject::connect(p_curdevWgt, SIGNAL(turnOffDevice()),
                         p_curdevice, SLOT(Delete())
                        );
        QObject::connect(p_curdevWgt, SIGNAL(posIsValid(QString)),
                         p_curdevice, SLOT(setPos(QString))
                        );
        QObject::connect(p_curdevWgt, SIGNAL(startMoveDevice()),
                         p_curdevice, SLOT(move())
                        );
        QObject::connect(p_curdevWgt, SIGNAL(stopMoveDevice()),
                         p_curdevice, SLOT(stop())
                        );
        QObject::connect(p_curdevice, SIGNAL(deviceMoveEnd()),
                         p_curdevWgt, SLOT(setMoveMod())
                        );
        //p_curdevWgt->setmaxPos(73000);
        p_curdevice->setName(devsloader.getDevName(i));
        p_curdevice->Init();
        p_curdevWgt->Init();
        pvbxLayout->addWidget(p_curdevWgt);
    }
    mainWgt.setLayout(pvbxLayout);

/*
    if (nDevs > 0) {
        STANDADevice* p_curdevice = new STANDADevice(&mainWgt);
        vOfdevs.push_back(p_curdevice);
        STANDADeviceWidget* p_curdevWgt = new STANDADeviceWidget(&mainWgt);
        vOfDevWgts.push_back(p_curdevWgt);
        //devSettWgt = new STANDASettingsWidget[nDevs];
    }
    else {
    	return -1;
    }

    for (int i=0; i<nDevs; i++) {
      STANDADevice* p_curdevice = (STANDADevice*)vOfdevs.at(i);
      STANDADeviceWidget* p_curdevWgt = (STANDADeviceWidget*)vOfDevWgts.at(i);
      QObject::connect(p_curdevWgt, SIGNAL(turnOnDevice()),
                       p_curdevice, SLOT(Init())
                      );
      QObject::connect(p_curdevWgt, SIGNAL(turnOffDevice()),
                       p_curdevice, SLOT(Delete())
                      );
      QObject::connect(p_curdevWgt, SIGNAL(posIsValid(QString)),
                       p_curdevice, SLOT(setPos(QString))
                      );
      QObject::connect(p_curdevWgt, SIGNAL(startMoveDevice()),
                       p_curdevice, SLOT(move())
                      );
      QObject::connect(p_curdevWgt, SIGNAL(stopMoveDevice()),
                       p_curdevice, SLOT(stop())
                      );
      QObject::connect(p_curdevice, SIGNAL(deviceMoveEnd()),
                       p_curdevWgt, SLOT(setMoveMod())
                      );
      //p_curdevWgt->setmaxPos(73000);
      p_curdevice->setName(devsloader.getDevName(i));
      p_curdevice->Init();
      p_curdevWgt->Init();
      pvbxLayout->addWidget(p_curdevWgt);
    }

    mainWgt.setLayout(pvbxLayout);
*/
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
