//
//  header.h
//  

#ifndef STRUCTDAT_H
#define STRUCTDAT_H

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>

#pragma pack(1)

#define pmID 2463
#define gpsID 2223
#define lasID 3334
#define plotID 4445
#define xboxID 5556
#define simID 6667
#define ugvID 7778



struct GPS{
    unsigned char discard[44];
    double northings; //8 bytes
    double eastings;
    double height;
    unsigned char discard2[40];
    unsigned int checksum;
    
    //unsigned char GPSQuality;
    //double GPSTimeStamp;
};

struct laser{
    //double remove[23];
    double x[1100];
    double y[1100];
    int numPoints;
    //double LaserTimeStamp;
};

struct plotting{
    double X;
    double Y;
    double heading;
    double steering;
    //double plotTimeStamp;
};

struct remote {
    double setSteering;
    double setSpeed;
    unsigned char UGV_sim;
    int terminate;
    //double xboxTimeStamp;
};

struct unitFlags {
    unsigned short PM : 1, //process management
                  GPS : 1,
                laser : 1,
                 xbox : 1,
                  UGV : 1,
               openGL : 1,
               unused : 10;
};

union execFlags {
    unitFlags flags;
    unsigned short status; // allows for the whole unitFlags number to be returned
};

struct processManagement {
    execFlags heartbeat; //monitors the devices
    execFlags shutDown; //controls the devices
    
    //    double PMTimeStamp;
};


#endif
