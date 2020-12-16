#include <QtWidgets>
#include "STANDALoader.h"

int main(int argc, char** argv)
{
	QApplication app(argc, argv);
    QWidget      wgt;

    QPushButton* pcmdOk = new QPushButton("ok", &wgt);
    STANDALoader loader(&wgt);

    QObject::connect(pcmdOk,  SIGNAL(clicked()),
    	             &loader, SLOT(testappeasy())
    	            );
    QObject::connect(&loader, SIGNAL(failed()),
    	             &app,    SLOT(quit())
    	            );
    
    QVBoxLayout* pvbxLayout = new QVBoxLayout;
    pvbxLayout->addWidget(pcmdOk);
    wgt.setLayout(pvbxLayout);
    
    wgt.show();
	return app.exec();
}