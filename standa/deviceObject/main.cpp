#include <QtWidgets>
#include "STANDADevice.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QWidget wgt;
    STANDADevice device;

    QPushButton* pcmdInit = new QPushButton("init");
    QPushButton* pcmdExit = new QPushButton("exit");

    // Lables
    QLabel lblName("&devName");
    QLabel lblId  ("&devId");
    QLabel lblPos ("&Pos");
    QLabel lblStep ("&step");
    // --------------------------------------------------------------------------------------------

    // --- Editors
    QLineEdit* pdevNameEdit = new QLineEdit;
    lblName.setBuddy(pdevNameEdit);

    QLineEdit* pdevIdEdit = new QLineEdit;
    QIntValidator* pdevIdValidator = new QIntValidator(pdevIdEdit);
    pdevIdEdit->setValidator(pdevIdValidator);
    lblId.setBuddy(pdevIdEdit);

    QLineEdit* pposEdit = new QLineEdit;
    QDoubleValidator* pxValidator = new QDoubleValidator(pposEdit);
    pxValidator->setLocale(QLocale::C);
    pposEdit->setValidator(pxValidator);
    lblPos.setBuddy(pposEdit);

    QLineEdit* pstepEdit = new QLineEdit;
    QDoubleValidator* pstepValidator = new QDoubleValidator(pstepEdit);
    pstepValidator->setLocale(QLocale::C);
    pstepEdit->setValidator(pstepValidator);
    lblStep.setBuddy(pstepEdit);
    // --------------------------------------------------------------------------------------------

    // ---- Connection
    QObject::connect(pdevNameEdit, SIGNAL(textChanged(QString)),
                     &device, SLOT(setName(QString))
                    );
    QObject::connect(pdevIdEdit, SIGNAL(textChanged(QString)),
                     &device, SLOT(setId(QString))
                    );
    QObject::connect(pposEdit, SIGNAL(textChanged(QString)),
                     &device, SLOT(setPos(QString))
                    );
    QObject::connect(pstepEdit, SIGNAL(textChanged(QString)),
                     &device, SLOT(setStep(QString))
                    );
    QObject::connect(pcmdInit, SIGNAL(clicked()),
                     &device, SLOT(Init())
                    );
    QObject::connect(pcmdExit, SIGNAL(clicked()),
                     &app, SLOT(quit())
                    );
    // --------------------------------------------------------------------------------------------

    QHBoxLayout* phbxLayout1 = new QHBoxLayout;
    phbxLayout1->setSpacing(5);
    phbxLayout1->addWidget(&lblName);
    phbxLayout1->addWidget(&lblId);
    phbxLayout1->addWidget(&lblPos);
    phbxLayout1->addWidget(&lblStep);

    QHBoxLayout* phbxLayout2 = new QHBoxLayout;
    phbxLayout2->addWidget(pdevNameEdit);
    phbxLayout2->addWidget(pdevIdEdit);
    phbxLayout2->addWidget(pposEdit);
    phbxLayout2->addWidget(pstepEdit);

    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    pvbxLayout->addLayout(phbxLayout1);
    pvbxLayout->addLayout(phbxLayout2);
    pvbxLayout->addWidget(pcmdInit);
    pvbxLayout->addWidget(pcmdExit);

    wgt.setLayout(pvbxLayout);
    wgt.show();

    return app.exec();
}
