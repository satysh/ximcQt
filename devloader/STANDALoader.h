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
    bool getResult() { return m_result; }

    bool doesDevFromFileExist(QString devName);

    void setResult(bool rslt) { m_result=rslt; }
    void check();
    void findDevices();
    void findInputFile();
signals:
    void successfull();
    void failed();

private:
    int m_ndevs=0;
    QStringList m_vDevnamesList;
    QStringList m_vDevnamesListFromFile;

    bool m_result=true;

	device_t m_device;
};
