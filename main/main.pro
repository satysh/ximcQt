TEMPLATE = app
HEADERS += ../devicewgt/STANDADeviceWidget.h \
           ../view/STANDAVisualization.h \
           ../deviceobj/STANDADevice.h
SOURCES += main.cpp ../devicewgt/STANDADeviceWidget.cpp \
                    ../view/STANDAVisualization.cpp \
                    ../deviceobj/STANDADevice.cpp
RESOURCES = ../view/resource.qrc
QT      += widgets
TARGET   = main.exe
