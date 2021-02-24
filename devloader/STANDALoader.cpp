#include <stdio.h>

#include <QDebug>
#include <QString>
#include <QByteArray>
#include <QFile>

#include "STANDALoader.h"


//const char* enumerate_hints = "addr="; // this hint will use broadcast enumeration, if ENUMERATE_NETWORK flag is enabled
STANDALoader::STANDALoader(QObject *parent/* = nullptr*/)
    : QObject(parent)
{

}
STANDALoader::~STANDALoader()
{
}

QString STANDALoader::getDevName(int i)
{
	if (i >= m_vDevnameslist.size()) {
		qDebug() << "STANDALoader::getDevName(" << i << ") is uncorrect!";
		return NULL;
	}
    return m_vDevnameslist.at(i);
}

void STANDALoader::check()
{

    QFile fileOut("../settingsdata/fileout.txt");
    if(fileOut.open(QIODevice::ReadOnly | QIODevice::Text)) {

        QByteArray data;
        data = fileOut.readAll();
        qDebug() << "file: " << QString(data);
    }
    fileOut.close();

}
void STANDALoader::findDevices()
{
	const int probe_flags = ENUMERATE_PROBE/* | BORDER_IS_ENCODER*/;
	//const char* enumerate_hints = "addr=192.168.1.1,172.16.2.3";
	const char* enumerate_hints = "";
	/*engine_settings_t engine_settings;
	const char* units = "mm";
	const int seconds = 3;
	status_t status;
	status_calb_t status_calb;
	calibration_t calibration;
	char device_name[256];*/
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
	m_ndevs = get_device_count( devenum );

    //m_ndevs=2;
//	Terminate if there are no connected devices
	if (m_ndevs <= 0)
	{
		printf( "No devices found\n" );
	//	Free memory used by device enumeration data
		free_enumerate_devices( devenum );
		emit failed();
	}
    else {
		for (int i=0; i<m_ndevs; i++) {
			/*QString s = "device_";
			s += QString().setNum(i+1);
			qDebug() << "s=" << s;
			m_vDevnameslist << s;*/
			m_vDevnameslist << get_device_name(devenum, i);
		}


		qDebug() << m_ndevs << " devices are found:";
		for (int i=0; i<m_vDevnameslist.size(); i++) {
			qDebug() << " " << i+1 << ": " << m_vDevnameslist.at(i);
		}
		//Free memory used by device enumeration data
	    free_enumerate_devices( devenum );
		emit successfull();
	}
}
