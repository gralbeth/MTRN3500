#include <structs.h>
#include <iostream>
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr

#define GPS_PORT 24000

#define NORTHING_INDEX 44
#define EASTING_INDEX 52
#define HEIGHT_INDEX 60

#define END_OF_DATA 108 //start of CRC - unsigned long size

#define CRC32_POLYNOMIAL 0xEDB88320L

struct GPS {
    unsigned char Data[112]; //from this
    double Northing; // to here
    double Easting;
    double Height;
}

void GPSConnection();

void GPSStoreData();

void GPSPrintData();