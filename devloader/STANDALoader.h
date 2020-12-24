#pragma once

#include <ximc.h>

#include <vector>

#include <QObject>

class QStringList;
class QString;

class STANDALoader : public QObject
{
	Q_OBJECT
public:
	STANDALoader(QObject *parent = nullptr);
	~STANDALoader();

    int getnDevs() { return m_ndevs; }
    QString getDevName(int i);


    void findDevices();
signals:
    void successfull();
    void failed();

private:
    int m_ndevs=0;
    QStringList m_vDevnameslist;

	device_t m_device;
};
