#include <QtWidgets>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    QWidget wgt;

    QLabel *pstatusLabel = new QLabel("on/off");
    QPalette pstatusLabelPalette =  pstatusLabel->palette();
    pstatusLabelPalette.setColor(QPalette::Window, QColor(Qt::green));
    pstatusLabel->setAutoFillBackground(true);
    pstatusLabel->setPalette(pstatusLabelPalette);
    QPushButton *pcmdWarm = new QPushButton("Прогрев");
    QPalette pcmdWarmPalette = pcmdWarm->palette();
    pcmdWarmPalette.setColor(QPalette::Window, QColor(Qt::red));
    pcmdWarm->setPalette(pcmdWarmPalette);


    /* Layouts */
    QVBoxLayout *pvbxXRay1Layout = new QVBoxLayout;
    pvbxXRay1Layout->addWidget(pstatusLabel);
    pvbxXRay1Layout->addWidget(pcmdWarm);

    QHBoxLayout *phbxXRayPosLayout = new QHBoxLayout;
    phbxXRayPosLayout->addWidget(new QLabel("ypos: "));
    phbxXRayPosLayout->addWidget(new QLabel("-300"));
    QHBoxLayout *phbxXRayHVLayout = new QHBoxLayout;
    phbxXRayHVLayout->addWidget(new QLabel("HV: "));
    phbxXRayHVLayout->addWidget(new QLabel("1200"));
    QHBoxLayout *phbxXRayILayout = new QHBoxLayout;
    phbxXRayILayout->addWidget(new QLabel("I: "));
    phbxXRayILayout->addWidget(new QLabel("300"));
    QVBoxLayout *pvbxXRayCurrentsLayout = new QVBoxLayout;
    pvbxXRayCurrentsLayout->addLayout(phbxXRayPosLayout);
    pvbxXRayCurrentsLayout->addLayout(phbxXRayHVLayout);
    pvbxXRayCurrentsLayout->addLayout(phbxXRayILayout);

    QHBoxLayout *phbxXRayLayout1 = new QHBoxLayout;
    phbxXRayLayout1->addLayout(pvbxXRay1Layout);
    phbxXRayLayout1->addLayout(pvbxXRayCurrentsLayout);

    QVBoxLayout *pXRayParamsSetLayout = new QVBoxLayout;
    QHBoxLayout *pXRayParamsSetLayoutHV = new QHBoxLayout;
    pXRayParamsSetLayoutHV->addWidget(new QLabel("HV"));
    pXRayParamsSetLayoutHV->addWidget(new QLineEdit("13"));
    pXRayParamsSetLayoutHV->addWidget(new QPushButton("ok"));
    QHBoxLayout *pXRayParamsSetLayoutI = new QHBoxLayout;
    pXRayParamsSetLayoutI->addWidget(new QLabel("I"));
    pXRayParamsSetLayoutI->addWidget(new QLineEdit("31"));
    pXRayParamsSetLayoutI->addWidget(new QPushButton("ok"));
    pXRayParamsSetLayout->addLayout(pXRayParamsSetLayoutHV);
    pXRayParamsSetLayout->addLayout(pXRayParamsSetLayoutI);

    QHBoxLayout *phbxMoveLayout = new QHBoxLayout;
    phbxMoveLayout->addWidget(new QLabel("set y pos"));
    QLineEdit *pXRayyEdit = new QLineEdit("199");
    pXRayyEdit->setFixedSize(30, 30);
    phbxMoveLayout->addWidget(pXRayyEdit);
    QSlider *pXraySlaider = new QSlider(Qt::Horizontal);
    pXraySlaider->setTickInterval(10);
    pXraySlaider->setTickPosition(QSlider::TicksBelow);
    phbxMoveLayout->addWidget(pXraySlaider);
    QPushButton *pcmdXRayMove = new QPushButton("move");
    phbxMoveLayout->addWidget(pcmdXRayMove);
    QPushButton *pcmdXRayStop = new QPushButton("stop");
    phbxMoveLayout->addWidget(pcmdXRayStop);


    QVBoxLayout *pvbxXRayLayout = new QVBoxLayout;
    QLabel *pxrayLabel = new QLabel("X-RAY");
    QPalette pxrayLabelPal = pxrayLabel->palette();
    pxrayLabelPal.setColor(QPalette::Window, QColor(Qt::red));
    pxrayLabel->setAutoFillBackground(true);
    pxrayLabel->setPalette(pxrayLabelPal);
    pvbxXRayLayout->addWidget(pxrayLabel, 0, Qt::AlignCenter);
    pvbxXRayLayout->addLayout(phbxXRayLayout1);
    pvbxXRayLayout->addLayout(pXRayParamsSetLayout);
    pvbxXRayLayout->addLayout(phbxMoveLayout);

    QVBoxLayout *pvbxObjLayout = new QVBoxLayout;
    QLabel *pobjLabel = new QLabel("Object");
    QPalette pobjLabelPal = pobjLabel->palette();
    pobjLabelPal.setColor(QPalette::Window, QColor(Qt::yellow));
    pobjLabel->setAutoFillBackground(true);
    pobjLabel->setPalette(pobjLabelPal);
    pvbxObjLayout->addWidget(pobjLabel, 0, Qt::AlignCenter);

    QHBoxLayout *pobjyLayout = new QHBoxLayout;
    pobjyLayout->addWidget(new QLabel("ypos: "));
    pobjyLayout->addWidget(new QLabel("0"));
    QHBoxLayout *pobjzLayout = new QHBoxLayout;
    pobjzLayout->addWidget(new QLabel("zpos: "));
    pobjzLayout->addWidget(new QLabel("0"));
    QHBoxLayout *pobjangLayout = new QHBoxLayout;
    pobjangLayout->addWidget(new QLabel("angle: "));
    pobjangLayout->addWidget(new QLabel("0"));
    pvbxObjLayout->addLayout(pobjyLayout);
    pvbxObjLayout->addLayout(pobjzLayout);
    pvbxObjLayout->addLayout(pobjangLayout);

    QHBoxLayout *pobjhbxMoveLayout = new QHBoxLayout;
    pobjhbxMoveLayout->addWidget(new QLabel("set y pos"));
    QLineEdit *pObjyEdit = new QLineEdit("16");
    pObjyEdit->setFixedSize(30, 30);
    pobjhbxMoveLayout->addWidget(pObjyEdit);
    QSlider *pObjSlaider = new QSlider(Qt::Horizontal);
    pObjSlaider->setTickInterval(10);
    pObjSlaider->setTickPosition(QSlider::TicksBelow);
    pobjhbxMoveLayout->addWidget(pObjSlaider);
    QPushButton *pcmdObjMove = new QPushButton("move");
    pobjhbxMoveLayout->addWidget(pcmdObjMove);
    QPushButton *pcmdObjStop = new QPushButton("stop");
    pobjhbxMoveLayout->addWidget(pcmdObjStop);
    pvbxObjLayout->addLayout(pobjhbxMoveLayout);

    QHBoxLayout *pobjhbxMovezLayout = new QHBoxLayout;
    pobjhbxMovezLayout->addWidget(new QLabel("set z pos"));
    QLineEdit *pObjzEdit = new QLineEdit("991");
    pObjzEdit->setFixedSize(30, 30);
    pobjhbxMovezLayout->addWidget(pObjzEdit);
    QSlider *pObjzSlaider = new QSlider(Qt::Horizontal);
    pObjzSlaider->setTickInterval(10);
    pObjzSlaider->setTickPosition(QSlider::TicksBelow);
    pobjhbxMovezLayout->addWidget(pObjzSlaider);
    QPushButton *pcmdObjzMove = new QPushButton("move");
    pobjhbxMovezLayout->addWidget(pcmdObjzMove);
    QPushButton *pcmdObjzStop = new QPushButton("stop");
    pobjhbxMovezLayout->addWidget(pcmdObjzStop);
    pvbxObjLayout->addLayout(pobjhbxMovezLayout);

    QHBoxLayout *pobjhbxMoveangLayout = new QHBoxLayout;
    pobjhbxMoveangLayout->addWidget(new QLabel("set angle"));
    QLineEdit *pObjangEdit = new QLineEdit("0");
    pObjangEdit->setFixedSize(30, 30);
    pobjhbxMoveangLayout->addWidget(pObjangEdit);
    QDial *panglDial = new QDial;
    panglDial->setFocusPolicy(Qt::StrongFocus);
    panglDial->setNotchesVisible(true);
    pobjhbxMoveangLayout->addWidget(panglDial);
    QPushButton *pcmdObjangMove = new QPushButton("move");
    pobjhbxMoveangLayout->addWidget(pcmdObjangMove);
    QPushButton *pcmdObjangStop = new QPushButton("stop");
    pobjhbxMoveangLayout->addWidget(pcmdObjangStop);
    pvbxObjLayout->addLayout(pobjhbxMoveangLayout);


    QVBoxLayout *pvbxDetLayout = new QVBoxLayout;
    QLabel *pdetLabel = new QLabel("Detector");
    QPalette pdetLabelPal = pdetLabel->palette();
    pdetLabelPal.setColor(QPalette::Window, QColor(Qt::magenta));
    pdetLabel->setAutoFillBackground(true);
    pdetLabel->setPalette(pdetLabelPal);
    pvbxDetLayout->addWidget(pdetLabel, 0, Qt::AlignCenter);

    QHBoxLayout *pdetyLayout = new QHBoxLayout;
    pdetyLayout->addWidget(new QLabel("ypos: "));
    pdetyLayout->addWidget(new QLabel("300"));
    QHBoxLayout *pdetzLayout = new QHBoxLayout;
    pdetzLayout->addWidget(new QLabel("zpos: "));
    pdetzLayout->addWidget(new QLabel("150"));
    pvbxDetLayout->addLayout(pdetyLayout);
    pvbxDetLayout->addLayout(pdetzLayout);

    QHBoxLayout *pdethbxMoveLayout = new QHBoxLayout;
    pdethbxMoveLayout->addWidget(new QLabel("set y pos"));
    QLineEdit *pdetyEdit = new QLineEdit("87");
    pdetyEdit->setFixedSize(30, 30);
    pdethbxMoveLayout->addWidget(pdetyEdit);
    QSlider *pdetSlaider = new QSlider(Qt::Horizontal);
    pdetSlaider->setTickInterval(10);
    pdetSlaider->setTickPosition(QSlider::TicksBelow);
    pdethbxMoveLayout->addWidget(pdetSlaider);
    QPushButton *pcmddetMove = new QPushButton("move");
    pdethbxMoveLayout->addWidget(pcmddetMove);
    QPushButton *pcmddetStop = new QPushButton("stop");
    pdethbxMoveLayout->addWidget(pcmddetStop);
    pvbxDetLayout->addLayout(pdethbxMoveLayout);

    QHBoxLayout *pdethbxMovezLayout = new QHBoxLayout;
    pdethbxMovezLayout->addWidget(new QLabel("set z pos"));
    QLineEdit *pdetzEdit = new QLineEdit("78");
    pdetzEdit->setFixedSize(30, 30);
    pdethbxMovezLayout->addWidget(pdetzEdit);
    QSlider *pdetzSlaider = new QSlider(Qt::Horizontal);
    pdetzSlaider->setTickInterval(10);
    pdetzSlaider->setTickPosition(QSlider::TicksBelow);
    pdethbxMovezLayout->addWidget(pdetzSlaider);
    QPushButton *pcmddetMovez = new QPushButton("move");
    pdethbxMovezLayout->addWidget(pcmddetMovez);
    QPushButton *pcmddetStopz = new QPushButton("stop");
    pdethbxMovezLayout->addWidget(pcmddetStopz);
    pvbxDetLayout->addLayout(pdethbxMovezLayout);


    QWidget xraWgt;
    xraWgt.setLayout(pvbxXRayLayout);
    QWidget objWgt;
    QPalette objWgtPal = objWgt.palette();
    objWgtPal.setColor(QPalette::Window, QColor(Qt::green));
    objWgt.setAutoFillBackground(true);
    objWgt.setPalette(objWgtPal);
    objWgt.setLayout(pvbxObjLayout);

    QWidget detWgt;
    detWgt.setLayout(pvbxDetLayout);
    QPalette detWgtPal = detWgt.palette();
    detWgtPal.setColor(QPalette::Window, QColor(Qt::cyan));
    detWgt.setAutoFillBackground(true);
    detWgt.setPalette(detWgtPal);

    QHBoxLayout *phbxMainLayout1 = new QHBoxLayout;
    phbxMainLayout1->addWidget(&xraWgt);
    phbxMainLayout1->addWidget(&objWgt);
    phbxMainLayout1->addWidget(&detWgt);

    QWidget scanWgt;
    QVBoxLayout *pvbScanMainLayout = new QVBoxLayout;
    scanWgt.setLayout(pvbScanMainLayout);
    QLabel *pscanLabel = new QLabel("Scan");
    QPalette pscanLabelPal = pscanLabel->palette();
    pscanLabelPal.setColor(QPalette::Window, QColor(Qt::blue));
    pscanLabel->setAutoFillBackground(true);
    pscanLabel->setPalette(pscanLabelPal);
    pvbScanMainLayout->addWidget(pscanLabel, 0, Qt::AlignCenter);

    QHBoxLayout *phbScanMainLayout = new QHBoxLayout;
    pvbScanMainLayout->addLayout(phbScanMainLayout);

    QWidget xrayscwgt;
    QVBoxLayout *pxrayscwgtLayout = new QVBoxLayout;
    pxrayscwgtLayout->addWidget(new QLabel("x-ray"), 0, Qt::AlignCenter);
    QHBoxLayout *xrayscyposLayout = new QHBoxLayout;
    QRadioButton *pcmdscyxray = new QRadioButton("y pos");
    xrayscyposLayout->addWidget(pcmdscyxray);
    xrayscyposLayout->addWidget(new QLabel("from: "));
    xrayscyposLayout->addWidget(new QLineEdit("36"));
    xrayscyposLayout->addWidget(new QLabel("to: "));
    xrayscyposLayout->addWidget(new QLineEdit("63"));
    pxrayscwgtLayout->addLayout(xrayscyposLayout);
    xrayscwgt.setLayout(pxrayscwgtLayout);
    phbScanMainLayout->addWidget(&xrayscwgt, 0, Qt::AlignTop);

    QWidget objscwgt;
    objscwgt.setAutoFillBackground(true);
    objscwgt.setPalette(objWgtPal);
    QVBoxLayout *pobjscwgtLayout = new QVBoxLayout;
    pobjscwgtLayout->addWidget(new QLabel("Object"), 0, Qt::AlignCenter);
    QHBoxLayout *objscyposLayout = new QHBoxLayout;
    QRadioButton *pcmdscyobj = new QRadioButton("y pos");
    objscyposLayout->addWidget(pcmdscyobj);
    objscyposLayout->addWidget(new QLabel("from: "));
    objscyposLayout->addWidget(new QLineEdit("36"));
    objscyposLayout->addWidget(new QLabel("to: "));
    objscyposLayout->addWidget(new QLineEdit("63"));
    pobjscwgtLayout->addLayout(objscyposLayout);

    QHBoxLayout *objsczposLayout = new QHBoxLayout;
    QRadioButton *pcmdsczobj = new QRadioButton("z pos");
    objsczposLayout->addWidget(pcmdsczobj);
    objsczposLayout->addWidget(new QLabel("from: "));
    objsczposLayout->addWidget(new QLineEdit("6"));
    objsczposLayout->addWidget(new QLabel("to: "));
    objsczposLayout->addWidget(new QLineEdit("9"));
    pobjscwgtLayout->addLayout(objsczposLayout);

    QHBoxLayout *objscangposLayout = new QHBoxLayout;
    QRadioButton *pcmdscangobj = new QRadioButton("angle: ");
    objscangposLayout->addWidget(pcmdscangobj);
    objscangposLayout->addWidget(new QLabel("from: "));
    objscangposLayout->addWidget(new QLineEdit("0"));
    objscangposLayout->addWidget(new QLabel("to: "));
    objscangposLayout->addWidget(new QLineEdit("360"));
    pobjscwgtLayout->addLayout(objscangposLayout);


    objscwgt.setLayout(pobjscwgtLayout);
    phbScanMainLayout->addWidget(&objscwgt);

    QWidget detscwgt;
    detscwgt.setAutoFillBackground(true);
    detscwgt.setPalette(detWgtPal);
    QVBoxLayout *pdetscwgtLayout = new QVBoxLayout;
    pdetscwgtLayout->addWidget(new QLabel("Detector"), 0, Qt::AlignCenter);
    QHBoxLayout *detscyposLayout = new QHBoxLayout;
    QRadioButton *pcmdscydet = new QRadioButton("y pos");
    detscyposLayout->addWidget(pcmdscydet);
    detscyposLayout->addWidget(new QLabel("from: "));
    detscyposLayout->addWidget(new QLineEdit("36"));
    detscyposLayout->addWidget(new QLabel("to: "));
    detscyposLayout->addWidget(new QLineEdit("63"));
    pdetscwgtLayout->addLayout(detscyposLayout);

    QHBoxLayout *detsczposLayout = new QHBoxLayout;
    QRadioButton *pcmdsczdet = new QRadioButton("z pos");
    detsczposLayout->addWidget(pcmdsczdet);
    detsczposLayout->addWidget(new QLabel("from: "));
    detsczposLayout->addWidget(new QLineEdit("6"));
    detsczposLayout->addWidget(new QLabel("to: "));
    detsczposLayout->addWidget(new QLineEdit("9"));
    pdetscwgtLayout->addLayout(detsczposLayout);

    detscwgt.setLayout(pdetscwgtLayout);
    phbScanMainLayout->addWidget(&detscwgt);


    pvbScanMainLayout->addWidget(new QLineEdit("Start scan"));
    pvbScanMainLayout->addWidget(new QPushButton("Start scan"));
    pvbScanMainLayout->addWidget(new QTextEdit);

    QVBoxLayout *pvbxMainLayout = new QVBoxLayout;
    pvbxMainLayout->addLayout(phbxMainLayout1);
    pvbxMainLayout->addWidget(&scanWgt);

    wgt.setLayout(pvbxMainLayout);
    wgt.show();

    return app.exec();
}
