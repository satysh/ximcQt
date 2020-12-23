#include <stdio.h>

#include "STANDALoader.h"

const int seconds = 3;
const char* units = "mm";
const int probe_flags = ENUMERATE_PROBE /*| ENUMERATE_NETWORK*/;
const char* enumerate_hints = "addr=192.168.1.1,172.16.2.3";
engine_settings_t engine_settings;
status_t status;
status_calb_t status_calb;
calibration_t calibration;
int names_count;
char device_name[256];
char ximc_version_str[32];
device_enumeration_t devenum;

//const char* enumerate_hints = "addr="; // this hint will use broadcast enumeration, if ENUMERATE_NETWORK flag is enabled
STANDALoader::STANDALoader(QObject *parent/* = nullptr*/)
    : QObject(parent)
{

}
STANDALoader::~STANDALoader()
{
	printf( "Closing device..." );
//	Close specified device
	close_device( &device );
	printf( "done.\n" );
}


// ----- Public slots ----------------------------------------------
void STANDALoader::testappeasy()
{
/*
	Variables declaration.
	device_t, status_t, engine_settings_t, status_calb and calibration_t are types provided by the libximc library.
*/
	printf( "This is a ximc test program.\n" );
//	ximc_version returns library version string.
	ximc_version( ximc_version_str );
	printf( "libximc version %s\n", ximc_version_str );

//  Set bindy (network) keyfile. Must be called before any call to "enumerate_devices" or "open_device" if you
//  wish to use network-attached controllers. Accepts both absolute and relative paths, relative paths are resolved
//  relative to the process working directory. If you do not need network devices then "set_bindy_key" is optional.
	set_bindy_key( "keyfile.sqlite" );

//	Device enumeration function. Returns an opaque pointer to device enumeration data.
	devenum = enumerate_devices( probe_flags, enumerate_hints );

    bool devsCountFlag = true;
//	Gets device count from device enumeration data
	names_count = get_device_count( devenum );

//	Terminate if there are no connected devices
	if (names_count <= 0)
	{
		printf( "No devices found\n" );
	//	Free memory used by device enumeration data
		free_enumerate_devices( devenum );
		devsCountFlag = false;
		emit failed();
	}

    if (devsCountFlag) {
//	    Copy first found device name into a string
	    strcpy( device_name, get_device_name( devenum, 0 ) );
//	    Free memory used by device enumeration data
	    free_enumerate_devices( devenum );

	    printf( "Opening device...");
//	    Open device by device name
	    device = open_device( device_name );
	    printf( "done.\n" );

	    printf( "Getting status parameters: " );
//	    Read device status from a device
	    get_status( device, &status );
	    printf( "position %d, encoder %lld, speed %d\n", status.CurPosition, status.EncPosition, status.CurSpeed );

	    printf( "Getting engine parameters: " );
//	    Read engine settings from a device
	    get_engine_settings( device, &engine_settings );
	    printf( "voltage %d, current %d, speed %d\n", engine_settings.NomVoltage, engine_settings.NomCurrent, engine_settings.NomSpeed );


	    printf( "Getting calibrated parameters: " );
//      Setting calibration constant to 0.1 (one controller step equals this many units)
	    calibration.A = 0.1;
//      We have to set microstep mode to convert microsteps to calibrated units correctly
	    calibration.MicrostepMode = engine_settings.MicrostepMode;

//      Read calibrated device status from a device
	    get_status_calb( device, &status_calb, &calibration);
	    printf( "calibrated position %.3f %s, calibrated speed %.3f %s/s\n", status_calb.CurPosition, units, status_calb.CurSpeed, units );
/*
	    printf( "Closing device..." );
//	    Close specified device
	    close_device( &device );
	    printf( "done.\n" );*/
	    printf( "done.\n" );
	}
}

void STANDALoader::left()
{
	printf( "Rotating to the left for %d seconds...", seconds);
	command_left( device );
	/*msec_sleep( seconds*1000 );
	command_stop( device );*/
    printf( "\nGetting status parameters: " );
//	Read device status from a device
	get_status( device, &status );
	printf( "position %d, encoder %lld, speed %d\n", status.CurPosition, status.EncPosition, status.CurSpeed );
	printf( "done.\n" );
}

void STANDALoader::right()
{
	printf( "Rotating to the right for %d seconds...", seconds);
	command_right( device );
    /*msec_sleep( seconds*1000 );
    command_stop( device );*/
    //	Read device status from a device
    printf( "\nGetting status parameters: " );
	get_status( device, &status );
	printf( "position %d, encoder %lld, speed %d\n", status.CurPosition, status.EncPosition, status.CurSpeed );
	printf( "done.\n" );	
}

void  STANDALoader::stop()
{
    command_stop( device );
}