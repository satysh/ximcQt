TEMPLATE = app
HEADERS += STANDALoader.h ../settingsdata/STANDASettings.h ../deviceobj/STANDADevice.h
SOURCES += main.cpp STANDALoader.cpp ../settingsdata/STANDASettings.cpp ../deviceobj/STANDADevice.cpp
QT      += widgets
LIBS    += /usr/lib/libximc.so
TARGET   = devloader.exe
