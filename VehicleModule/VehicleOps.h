#include <iostream>
#include <structs.h>
#include <sys/socket.h>    //socket
#include <arpa/inet.h> //inet_addr
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define VEHICLE_PORT 25000

#define WAIT_COUNT 50

void VehicleOps(int sock, PM* PMPtr, bool *flag);
int VehicleSetup();
