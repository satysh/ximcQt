#include <QtWidgets>
#include "STANDASetup.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QWidget wgt;
    STANDASetup device;

    QPushButton* pcmdInit = new QPushButton("init");
    QPushButton* pcmdExit = new QPushButton("exit");

    // Lables
    QLabel lblName("&devName");
    QLabel lblId  ("&devId");
    QLabel lblXcor ("&X");
    QLabel lblYcor ("&Y");
    QLabel lblZcor ("&Z");
    QLabel lblStep ("&step");
    // --------------------------------------------------------------------------------------------

    // --- Editors
    QLineEdit* pdevNameEdit = new QLineEdit;
    lblName.setBuddy(pdevNameEdit);

    QLineEdit* pdevIdEdit = new QLineEdit;
    QIntValidator* pdevIdValidator = new QIntValidator(pdevIdEdit);
    pdevIdEdit->setValidator(pdevIdValidator);
    lblId.setBuddy(pdevIdEdit);

    QLineEdit* pxEdit = new QLineEdit;
    QDoubleValidator* pxValidator = new QDoubleValidator(pxEdit);
    pxValidator->setLocale(QLocale::C);
    pxEdit->setValidator(pxValidator);
    lblXcor.setBuddy(pxEdit);

    QLineEdit* pyEdit = new QLineEdit;
    QDoubleValidator* pyValidator = new QDoubleValidator(pyEdit);
    pyValidator->setLocale(QLocale::C);
    pyEdit->setValidator(pyValidator);
    lblYcor.setBuddy(pyEdit);

    QLineEdit* pzEdit = new QLineEdit;
    QDoubleValidator* pzValidator = new QDoubleValidator(pzEdit);
    pzValidator->setLocale(QLocale::C);
    pzEdit->setValidator(pzValidator);
    lblZcor.setBuddy(pzEdit);

    QLineEdit* pstepEdit = new QLineEdit;
    QDoubleValidator* pstepValidator = new QDoubleValidator(pstepEdit);
    pstepValidator->setLocale(QLocale::C);
    pstepEdit->setValidator(pstepValidator);
    lblStep.setBuddy(pstepEdit);
    // --------------------------------------------------------------------------------------------

    // ---- Connection
    QObject::connect(pdevNameEdit, SIGNAL(textChanged(QString)),
                     &device, SLOT(setDevName(QString))
                    );
    QObject::connect(pdevIdEdit, SIGNAL(textChanged(QString)),
                     &device, SLOT(setDevId(QString))
                    );
    QObject::connect(pxEdit, SIGNAL(textChanged(QString)),
                     &device, SLOT(setXcor(QString))
                    );
    QObject::connect(pyEdit, SIGNAL(textChanged(QString)),
                     &device, SLOT(setYcor(QString))
                    );
    QObject::connect(pzEdit, SIGNAL(textChanged(QString)),
                     &device, SLOT(setZcor(QString))
                    );
    QObject::connect(pstepEdit, SIGNAL(textChanged(QString)),
                     &device, SLOT(setStep(QString))
                    );
    QObject::connect(pcmdInit, SIGNAL(clicked()),
                     &device, SLOT(InitDevice())
                    );
    QObject::connect(pcmdExit, SIGNAL(clicked()),
                     &app, SLOT(quit())
                    );
    // --------------------------------------------------------------------------------------------

    QHBoxLayout* phbxLayout1 = new QHBoxLayout;
    phbxLayout1->setSpacing(5);
    phbxLayout1->addWidget(&lblName);
    phbxLayout1->addWidget(&lblId);
    phbxLayout1->addWidget(&lblXcor);
    phbxLayout1->addWidget(&lblYcor);
    phbxLayout1->addWidget(&lblZcor);
    phbxLayout1->addWidget(&lblStep);

    QHBoxLayout* phbxLayout2 = new QHBoxLayout;
    phbxLayout2->addWidget(pdevNameEdit);
    phbxLayout2->addWidget(pdevIdEdit);
    phbxLayout2->addWidget(pxEdit);
    phbxLayout2->addWidget(pyEdit);
    phbxLayout2->addWidget(pzEdit);
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
