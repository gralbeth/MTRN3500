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
#define BUFFER_TIME 160

#define NUM_PROCESSES 5 // PM, GPS, Laser, Display, Xbox, Vehicle TODO CHANGE TO 6
#define GPS_INDX 0
#define LSR_INDX 1
#define DISP_INDX 2
#define XBOX_INDX 3
#define VEHICLE_INDX 4

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
    ExecFlags Shutdown; //Array of shutdown flags for modules
    ExecFlags Heartbeats; //Array of heartbeat flags for modules

    double XVals[361]; //Laser Xvalues
    double YVals[361]; //Laser Yvalues

    double RemoteSteering; //Xbox Steering between +-40
    double RemoteSpeed; //Xbox speed between +-1

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
