#pragma once 

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <structs.h>
#include <stdint.h>
#include <SM.h>
#include <iostream>
#include <chrono>
#include <string>    //strlen
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr
#include <sstream> //stringstream lib
#include <vector>
#include <bitset>
#include <math.h>

using namespace std::chrono;

#define NUM_POINTS 25
#define SCALING 21
#define STARTING_ANGLE 23
#define STEP_WIDTH 24
#define POINTS 26
#define DEG2RAD 3.14159265358972/180.0
#define LASER_PORT 23000

//#define STEP_WIDTH 0.5
#define RESOLUTION 0.5
#define START_ANGLE 0

struct Data {
    double X;
    double Y;
};

class Laser {
    private:
    std::string ipAddr;
    int portNum;
    int sock; 
    char serverPacket[2048];
    char message[1000];
    PM* PMSMPtr;

    public:
    int numData;
    int startingAngle;
    int stepWidth;
    u_int16_t vals[361];
    double ValuesX[361];
    double ValuesY[361];

     ~Laser() {}
    Laser();
    Laser(std::string ip_addr,int portNum, PM* PMPtr);
    bool getDataPacket();
    void processDataPacket();
    void closeSock();
    void polarToXY();
    void printVals(int numPrint);
    int LaserOps();
};
