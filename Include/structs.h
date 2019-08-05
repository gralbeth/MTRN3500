#pragma once
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <SM.h>
#include <chrono>
#include <iostream>
#include <ctime>
#include <bitset>

#define LASER_KEY 3333
#define PM_KEY 4444
#define BUFFER_TIME 400

#define NUM_PROCESSES 6 // PM, GPS, Laser, Display, Xbox, Vehicle TODO CHANGE TO 6
#define PM_INDX 0
#define GPS_INDX 1
#define LSR_INDX 2
#define DISP_INDX 3
#define XBOX_INDX 4
#define VEHICLE_INDX 5

struct ModuleFlags
{
unsigned char PM:1,
GPS:1,
Laser:1,
Display:1,
Xbox:1,
Vehicle:1,
Unused:2;
};

union ExecFlags
{
//unsigned char Status;
unsigned char Status;
ModuleFlags Flags;
};

struct PM {
    ExecFlags Shutdown;
    //Timestamps Tstamps;
    ExecFlags Heartbeats;

    double XVals[361];
    double YVals[361];

    double RemoteSteering;
    double RemoteSpeed;

};

struct LaserData {
    uint16_t numData;
    uint16_t data[50];    
};




struct GPS {

};

struct Display {

};


struct Xbox {

}; 

struct memID {

};
