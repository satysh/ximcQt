TEMPLATE = app
HEADERS += STANDACalibratorWidget.h ../deviceobj/STANDADevice.h ../settingsdata/STANDASettings.h
SOURCES += STANDACalibratorWidget.cpp main.cpp ../deviceobj/STANDADevice.cpp ../settingsdata/STANDASettings.cpp
QT      += widgets
LIBS    += /usr/lib/libximc.so
TARGET  = calibratorwgt.exe
