#include "STANDASetup.h"
#include "STANDAHunter.h"
#include <QtWidgets>

int main (int argc, char **argv)
{
    QApplication app(argc, argv);
    QWidget wgt;
    QPushButton* pcmdInit = new QPushButton("init");
    QPushButton* pcmdMove = new QPushButton("move");

    STANDAHunter deviceHunter;
    STANDASetup device;

    QObject::connect(pcmdInit, SIGNAL(clicked()),
                     &deviceHunter, SLOT(printDevicesInfoSlot())
                    );

    QObject::connect(pcmdMove, SIGNAL(clicked()),
                     &device, SLOT(moveSlot())
                    );
    QObject::connect(&deviceHunter, SIGNAL(deviceNotfound()),
                     &app, SLOT(quit())
                    );


    QHBoxLayout* phbxLayout = new QHBoxLayout;
    phbxLayout->addWidget(pcmdInit);
    phbxLayout->addWidget(pcmdMove);

    wgt.setLayout(phbxLayout);
    wgt.show();

    return app.exec();
}
