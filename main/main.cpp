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

    devsloader.findAvailableDevices();
    devsloader.findUserSettingsFile();
    devsloader.compareAvailableAndUserSettings();

    int nDevs = devsloader.getnAvailableDevs();
    qDebug() << "Available devices number is " << nDevs;

    //nDevs=2; // TODO debug
    if (nDevs < 1) {
      qDebug() << "no devs were found!";
      return -1;
    }
    for (int i=0; i<nDevs; i++) {
        STANDADevice* p_curdevice = new STANDADevice(&mainWgt);
        STANDADeviceWidget* p_curdevWgt = new STANDADeviceWidget(&mainWgt);

        STANDASettings *curDevUserSettings = devsloader.getUserSettings(i);
        if ((bool)curDevUserSettings) {
            // Load device obj
            p_curdevice->setName(curDevUserSettings->getDeviceName());
            p_curdevice->Init();
            if (curDevUserSettings->getFriendlyName() != "")
                p_curdevice->setFriendlyName(curDevUserSettings->getFriendlyName());
            if (curDevUserSettings->getNomVoltage() > 0)
                p_curdevice->setNomVoltage(curDevUserSettings->getNomVoltage());
            if (curDevUserSettings->getNomSpeed() > 0)
                p_curdevice->setNomSpeed(curDevUserSettings->getNomSpeed());
            if (curDevUserSettings->getAccel() > 0)
                p_curdevice->setDevAccel(curDevUserSettings->getAccel());
            if (curDevUserSettings->getDecel() > 0)
                p_curdevice->setDevDecel(curDevUserSettings->getDecel());

            // Load device wgt
            p_curdevWgt->setminPosInDeviceCodes(curDevUserSettings->getZeroPosition());
            p_curdevWgt->setmaxPosInDeviceCodes(curDevUserSettings->getMaxPosition());
            double maxPosition=(double)std::max(curDevUserSettings->getZeroPosition(), curDevUserSettings->getMaxPosition())
                                      -std::min(curDevUserSettings->getZeroPosition(), curDevUserSettings->getMaxPosition());
            p_curdevWgt->setmaxPos(maxPosition);
            p_curdevWgt->Init(); // TODO It has to be that there are no effects for a place for the widget init
            // TODO remove maxPosition! User should set It by It self!
            p_curdevWgt->setDeviceName(p_curdevice->getFriendlyName());
        }
        else {
            p_curdevice->Init();
            p_curdevWgt->Init();
        }

        QObject::connect(p_curdevWgt, SIGNAL(startMoveDevice(QString)),
                         p_curdevice, SLOT(moveTo(QString))
                        );
        QObject::connect(p_curdevWgt, SIGNAL(stopMoveDevice()),
                         p_curdevice, SLOT(sstp())
                        );
        QObject::connect(p_curdevice, SIGNAL(deviceMoveEnd()),
                         p_curdevWgt, SLOT(setMoveMod())
                        );
        pvbxLayout->addWidget(p_curdevWgt);
    }
    mainWgt.setLayout(pvbxLayout);

    // Connect device widget and view widget
    /*
    QObject::connect(&devWgt,  SIGNAL(posIsValid(QString)),
                     &viewWgt, SLOT  (setXRayPos(QString))
                    );
    QObject::connect(&devWgt,  SIGNAL(startMoveDevice(QString)),
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
