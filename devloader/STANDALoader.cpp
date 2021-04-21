#include <stdio.h>

#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QFile>
#include <QTextStream>

#include "STANDALoader.h"
#include "../deviceobj/STANDADevice.h"


//const char* enumerate_hints = "addr="; // this hint will use broadcast enumeration, if ENUMERATE_NETWORK flag is enabled
STANDALoader::STANDALoader(QObject *parent/* = nullptr*/)
    : QObject(parent)
{

}
STANDALoader::~STANDALoader()
{
}
/*
QString STANDALoader::getDevName(int i)
{
	if (i >= m_vDevnamesList.size()) {
		qDebug() << "STANDALoader::getDevName(" << i << ") is uncorrect!";
		return NULL;
	}
    return m_vDevnamesList.at(i);
}

bool STANDALoader::doesDevFromFileExist(QString devName)
{
    for (int i=0; i<m_nAvailableDevs; i++) {
        if (devName == getDevName(i)) return true;
    }
    return false;
}
void STANDALoader::check()
{

}
*/
void STANDALoader::findAvailableDevices()
{
    qDebug() << "STANDALoader::findAvailableDevices";
	const int probe_flags = ENUMERATE_PROBE;
	const char* enumerate_hints = "";
	char ximc_version_str[32];
	device_enumeration_t devenum;

//	ximc_version returns library version string.
	ximc_version( ximc_version_str );
	printf( "libximc version %s\n", ximc_version_str );

//  Set bindy (network) keyfile. Must be called before any call to "enumerate_devices" or "open_device" if you
//  wish to use network-attached controllers. Accepts both absolute and relative paths, relative paths are resolved
//  relative to the process working directory. If you do not need network devices then "set_bindy_key" is optional.
	set_bindy_key( "keyfile.sqlite" );

//	Device enumeration function. Returns an opaque pointer to device enumeration data.
	devenum = enumerate_devices( probe_flags, enumerate_hints );

//	Gets device count from device enumeration data
	m_nAvailableDevs = get_device_count( devenum );

    //m_nAvailableDevs=2;
//	Terminate if there are no connected devices
	if (m_nAvailableDevs <= 0)
	{
		printf( "No devices found\n" );
	//	Free memory used by device enumeration data
		free_enumerate_devices( devenum );
		emit failed();
	}
    else {
		for (int i=0; i<m_nAvailableDevs; i++) {
            QString curDeviceName(get_device_name(devenum, i));
            STANDADevice curDevice;
            curDevice.setName(curDeviceName);
            curDevice.Init();
            STANDASettings *curDeviceSettings = new STANDASettings(this);
            curDeviceSettings->setDeviceName(curDeviceName);
            curDeviceSettings->setFriendlyName(curDevice.getFriendlyName());
            curDeviceSettings->setNomVoltage(curDevice.getNomVoltage());
            curDeviceSettings->setNomSpeed(curDevice.getNomSpeed());
            curDeviceSettings->setAccel(curDevice.getCurAcceleration());
            curDeviceSettings->setDecel(curDevice.getCurDeceleration());
            //m_vOfAvailableDevsSettings
			/*QString s = "device_";
			s += QString().setNum(i+1);
			qDebug() << "s=" << s;
			m_vDevnamesList << s;*/
			//m_vDevnamesList << get_device_name(devenum, i);
            curDevice.Close();
            m_vOfAvailableDevsSettings << curDeviceSettings;
		}


		qDebug() << m_nAvailableDevs << " devices are found:";
        for (int i=0; i<m_vOfAvailableDevsSettings.size(); i++) {
            STANDASettings *curDeviceSettings = m_vOfAvailableDevsSettings.at(i);
            qDebug() << curDeviceSettings->getDeviceName() << '\t'
                     << curDeviceSettings->getFriendlyName() << '\t'
                     << curDeviceSettings->getNomVoltage() << '\t'
                     << curDeviceSettings->getNomSpeed() << '\t'
                     << curDeviceSettings->getAccel() << '\t'
                     << curDeviceSettings->getDecel() << '\t'
                     << curDeviceSettings->getZeroPosition() << '\t'
                     << curDeviceSettings->getMaxPosition();
        }
        /*
		for (int i=0; i<m_vDevnamesList.size(); i++) {
			qDebug() << " " << i+1 << ": " << m_vDevnamesList.at(i);
		}*/
		//Free memory used by device enumeration data
	    free_enumerate_devices( devenum );
		emit successfull();
	}

}

void STANDALoader::findUserSettingsFile()
{
    qDebug() << "STANDALoader::findUserSettingsFile";
    QFile fileOut("../settingsdata/fileout.txt");
    int ndevsInFile=0;
    if(fileOut.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream readStream(&fileOut);
        while (!readStream.atEnd()) {
            // read device name
            QString curString;
            readStream >> curString;
            if (curString == "") break;

            STANDASettings *curDeviceSettings = new STANDASettings(this);
            curDeviceSettings->setDeviceName(curString);

            // read setting for the one device

            readStream >> curString;
            curDeviceSettings->setFriendlyName(curString);
            readStream >> curString;
            curDeviceSettings->setNomVoltage((int)curString.toDouble());
            readStream >> curString;
            curDeviceSettings->setNomSpeed((int)curString.toDouble());
            readStream >> curString;
            curDeviceSettings->setAccel((int)curString.toDouble());
            readStream >> curString;
            curDeviceSettings->setDecel((int)curString.toDouble());
            readStream >> curString;
            curDeviceSettings->setZeroPosition((int)curString.toDouble());
            readStream >> curString;
            curDeviceSettings->setMaxPosition((int)curString.toDouble());
            ndevsInFile++;
            m_vOfUserDevsSettings << curDeviceSettings;
        }
    }
    else {
        qDebug() << "The input file doesn't exist!";
    }
    qDebug() << "ndevsInFile=" << ndevsInFile;
    for (int i=0; i<m_vOfUserDevsSettings.size(); i++) {
        STANDASettings *curDeviceSettings = m_vOfUserDevsSettings.at(i);
        qDebug() << curDeviceSettings->getDeviceName() << '\t'
                 << curDeviceSettings->getFriendlyName() << '\t'
                 << curDeviceSettings->getNomVoltage() << '\t'
                 << curDeviceSettings->getNomSpeed() << '\t'
                 << curDeviceSettings->getAccel() << '\t'
                 << curDeviceSettings->getDecel() << '\t'
                 << curDeviceSettings->getZeroPosition() << '\t'
                 << curDeviceSettings->getMaxPosition();
    }
    fileOut.close();
}

void STANDALoader::compareAvailableAndUserSettings()
{
    qDebug() << "STANDALoader::compareAvailableAndUserSettings";
    if (m_vOfAvailableDevsSettings.size() != m_vOfUserDevsSettings.size()) {
        qDebug() << "[WARNING]: the number of the available devs is " << m_vOfAvailableDevsSettings.size()
                 << ", but the number of the user devs is " << m_vOfUserDevsSettings.size();
    }

    for (int i=0; i<m_vOfAvailableDevsSettings.size(); i++) {
        STANDASettings *curAvailableDevsSettings = m_vOfAvailableDevsSettings.at(i);
        for (int j=0; j<m_vOfUserDevsSettings.size(); j++) {
            STANDASettings *curUserDevsSettings = m_vOfUserDevsSettings.at(j);
            /* TODO add compare here! */
        }
    }
}
