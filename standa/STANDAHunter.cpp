#include <stdio.h>
#include <ximc.h>

#include "STANDAHunter.h"

STANDAHunter::STANDAHunter(QObject *parent/*=nullptr*/)
    : QObject(parent)
{
}
STANDAHunter::~STANDAHunter()
{
}

bool STANDAHunter::printDevicesInfo()
{
    device_t device;
    int names_count;
    char device_name[256];
    const int probe_flags = ENUMERATE_PROBE;
    const char* enumerate_hints = "";
    char ximc_version_str[32];
    device_enumeration_t devenum;

    printf( "Devices finding proccesing...\n" );
//  ximc_version returns library version string.
    ximc_version( ximc_version_str );
    printf( "libximc version %s\n", ximc_version_str );

//  Device enumeration function. Returns an opaque pointer to device enumeration data.
    devenum = enumerate_devices( probe_flags, enumerate_hints );

//  Gets device count from device enumeration data
    names_count = get_device_count( devenum );

//  Terminate if there are no connected devices
    if (names_count <= 0)
    {
        printf( "No devices found\n" );
    //  Free memory used by device enumeration data
        free_enumerate_devices( devenum );
        return false;
    }

    printf("%d Devices were found!\n", names_count);
    for (int i=0; i<names_count; i++)
    {
        // Copy found device name into a string
        strcpy( device_name, get_device_name( devenum, i ) );
        printf("device index: %d, device name: %s\n", i, device_name);

        listOfDevNames << device_name; // fill listOfDevNames

        // Open device by device name
        printf( "Opening device: %s\n", device_name);
        device = open_device( device_name );
        printf( "done.\n" );

        // Get device info
        device_information_t device_information;
        memset((void*)&device_information, 0, sizeof(device_information));
        get_device_information(device, &device_information);
        printf("Manufacturer: %s\n", device_information.Manufacturer);
        printf("Manufacturer ID: %s\n", device_information.ManufacturerId);
        printf("Product Description: %s\n", device_information.ProductDescription);
        printf("Hardware major version number: %d\n", device_information.Major);
        printf("Hardware minor version number: %d\n", device_information.Minor);
        printf("The revision number of this hardware version: %d\n", device_information.Release);

        printf( "Closing device..." );
        // Close specified device
        close_device( &device );
        printf( "done.\n" );
    }
    // Free memory used by device enumeration data
    free_enumerate_devices( devenum );

    return true;
}

void STANDAHunter::printDevicesInfoSlot()
{
    if (!printDevicesInfo()) emit deviceNotfound();
}
