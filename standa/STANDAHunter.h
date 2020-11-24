#pragma once

#include <QObject>
#include <QStringList>

class STANDAHunter : public QObject
{
    Q_OBJECT
public:
    STANDAHunter(QObject *parent = nullptr);
    ~STANDAHunter();

    QStringList getDevNamesList() { return listOfDevNames; }
    int getNDevs               () { return listOfDevNames.size(); }

    bool printDevicesInfo();

public slots:
    void printDevicesInfoSlot();

signals:
    void deviceNotfound();

private:
    QStringList listOfDevNames;
};
