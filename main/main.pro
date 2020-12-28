TEMPLATE = app
HEADERS += ../devicewgt/STANDADeviceWidget.h \
           ../view/STANDAVisualization.h \
           ../deviceobj/STANDADevice.h \
           ../devloader/STANDALoader.h \
           ../devsettings/STANDASettingsWidget.h
SOURCES += main.cpp ../devicewgt/STANDADeviceWidget.cpp \
                    ../view/STANDAVisualization.cpp \
                    ../deviceobj/STANDADevice.cpp \
                    ../devloader/STANDALoader.cpp \
                    ../devsettings/STANDASettingsWidget.cpp
RESOURCES = ../view/resource.qrc
QT      += widgets
LIBS    += /usr/lib/libximc.so
TARGET   = main.exe
