TEMPLATE = app
HEADERS += ../devicewgt/STANDADeviceWidget.h \
           ../view/STANDAVisualization.h
SOURCES += main.cpp ../devicewgt/STANDADeviceWidget.cpp \
                    ../view/STANDAVisualization.cpp
RESOURCES = ../view/resource.qrc
QT      += widgets
TARGET   = main.exe
