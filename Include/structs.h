#pragma once
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#define LASER_KEY 111
#define PM_KEY 222

struct ModuleFlags
{
unsigned char PM:1,
GPS:1,
Laser:1,
Xbox:1,
Vehicle:1,
Unused:3;
};

union ExecFlags
{
unsigned char Status;
ModuleFlags Flags;
};

struct PM {
    ExecFlags Heartbeats;
    ExecFlags Shutdown;
};

struct Laser {
    uint16_t numData;
    uint16_t data[50];    
};

struct GPS {

};

struct Display {

};

struct Vehicle {

};

struct Xbox {

}; 

struct memID {

};
