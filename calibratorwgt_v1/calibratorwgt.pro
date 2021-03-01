TEMPLATE = app
HEADERS += STANDACalibratorWidget.h ../deviceobj/STANDADevice.h
SOURCES += STANDACalibratorWidget.cpp main.cpp ../deviceobj/STANDADevice.cpp
QT      += widgets
LIBS    += /usr/lib/libximc.so
TARGET  = calibratorwgt.exe
