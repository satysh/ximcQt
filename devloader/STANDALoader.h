#pragma once

#include <ximc.h>

#include <vector>

#include <QObject>
#include <QVector>

class QStringList;
class QString;

#include "../settingsdata/STANDASettings.h"

class STANDALoader : public QObject
{
	Q_OBJECT
public:
	STANDALoader(QObject *parent = nullptr);
	~STANDALoader();

    int getnAvailableDevs() { return m_nAvailableDevs; }
    STANDASettings *getUserSettings(int iDev);

    //QString getDevName(int i);

    //bool getResult() { return m_result; }

    //bool doesDevFromFileExist(QString devName);

    //void setResult(bool rslt) { m_result=rslt; }
    //void check();
    void findAvailableDevices();
    void findUserSettingsFile();
    void compareAvailableAndUserSettings();
    //void findInputFile();
signals:
    void successfull();
    void failed();

private:
    int m_nAvailableDevs=0;

    QVector<STANDASettings*> m_vOfAvailableDevsSettings;
    QVector<STANDASettings*> m_vOfUserDevsSettings;
    //QStringList m_vDevnamesList;
    //QStringList m_vDevnamesListFromFile;

    //bool m_result=true;

	//device_t m_device;
};
