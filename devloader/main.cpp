#include <QtWidgets>
#include "STANDALoader.h"

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
    QWidget      wgt;

    QPushButton* pcmdOk = new QPushButton("ok", &wgt);
    QPushButton* pcmdLeft = new QPushButton("left", &wgt);
    QPushButton* pcmdRight = new QPushButton("right", &wgt);
    QPushButton* pcmdStop = new QPushButton("stop", &wgt);
    STANDALoader loader(&wgt);
    loader.findInputFile();

    qDebug() << "Loader results is " << loader.getResult();

/*
    QObject::connect(pcmdOk,  SIGNAL(clicked()),
    	             &loader, SLOT(testappeasy())
    	            );
    QObject::connect(pcmdLeft, SIGNAL(clicked()),
    	             &loader,  SLOT(left())
    	            );
    QObject::connect(pcmdRight, SIGNAL(clicked()),
    	             &loader,  SLOT(right())
    	            );
    QObject::connect(pcmdStop, SIGNAL(clicked()),
    	             &loader,  SLOT(stop())
    	            );
    QObject::connect(&loader, SIGNAL(failed()),
    	             &app,    SLOT(quit())
    	            );*/

    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    pvbxLayout->addWidget(pcmdOk);
    pvbxLayout->addWidget(pcmdLeft);
    pvbxLayout->addWidget(pcmdRight);
    pvbxLayout->addWidget(pcmdStop);
    wgt.setLayout(pvbxLayout);

    wgt.show();
	return app.exec();
}
