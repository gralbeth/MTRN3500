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

#define LASER_KEY 1111
#define PM_KEY 2222
#define BUFFER_TIME 400

struct ModuleFlags
{
unsigned char PM:1,
GPS:1,
Laser:1,
Xbox:1,
Vehicle:1,
Display:1,
Unused:2;
};

struct Timestamps {
    double PM;
    double GPS;
    double Laser;
    double Xbox;
    double Vehicle;
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
    ExecFlags PMHeartbeats;

    double XVals[361];
    double YVals[361];
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
