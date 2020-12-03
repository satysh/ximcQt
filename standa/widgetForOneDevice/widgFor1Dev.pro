TEMPLATE = app
HEADERS += STANDADeviceWidget.h ../deviceObject/STANDADevice.h ../visualWidget/STANDAVisualization.h
SOURCES += main.cpp STANDADeviceWidget.cpp ../deviceObject/STANDADevice.cpp ../visualWidget/STANDAVisualization.cpp
QT      += widgets
RESOURCES = ../visualWidget/resource.qrc
TARGET   = widget.exe
