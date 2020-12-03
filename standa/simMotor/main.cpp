#include <QtWidgets>
#include "STANDASim.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QWidget wgt;
    wgt.setFixedSize(1000, 1000);
    wgt.resize(1000, 1000);

    STANDASim sim(&wgt);
    sim.move(500, 500);

    QWidget setwgt(&wgt);
    QLineEdit* pPosEdit = new QLineEdit("0", &setwgt);
    pPosEdit->setValidator(new QIntValidator(0, 800));
    QPushButton* pcmdStart = new QPushButton("start", &setwgt);
    QPushButton* pcmdStop = new QPushButton("stop", &setwgt);

    QHBoxLayout* phbxLayout = new QHBoxLayout;
    phbxLayout->addWidget(pPosEdit);
    phbxLayout->addWidget(pcmdStart);
    phbxLayout->addWidget(pcmdStop);
    setwgt.setLayout(phbxLayout);


    QObject::connect(pPosEdit, SIGNAL(textChanged(QString)),
                     &sim,     SLOT(setPos(QString))
                    );
    QObject::connect(pcmdStart, SIGNAL(clicked()),
                     &sim,      SLOT(start())
                    );
    QObject::connect(pcmdStop, SIGNAL(clicked()),
                     &sim,      SLOT(stop())
                    );

    wgt.show();
    return app.exec();
}
