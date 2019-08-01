#include <structs.h>
#include <iostream>
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>


#define GPS_PORT 24000

#define NORTHING_INDEX 44
#define EASTING_INDEX 52
#define HEIGHT_INDEX 60

#define END_OF_DATA 108 //start of CRC - unsigned long size

#define CRC32_POLYNOMIAL 0xEDB88320L

struct GPSData {
    double Northing; 
    double Easting;
    double Height;
    unsigned int CRC;
};

void GPSOps(int sock);
int GPSConnect();
void GPSDisconnect(int sock);