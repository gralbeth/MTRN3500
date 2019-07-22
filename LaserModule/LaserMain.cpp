// #include "../Include/SMObject.h"
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

using namespace std::chrono;

int main(int argc, char* argv[])
{
    long int ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch()).count();
    std::cout << "Laser opened at time: " << ms << std::endl;
    Laser Lsr;
	Laser* Lsrptr;
	void* SMlsr;
    void* SMpm;
    PM* PMPtr; 
    
//	SMlsr = SMCreate(LASER_KEY,sizeof(Laser));
    SMpm = SMCreate(PM_KEY,sizeof(PM));
// Read from SM
    PMPtr = (PM*)SMpm;
    //PMPtr->Heartbeats.Flags.Laser = 0;

    printf("PMPtr->Shudown flag laser: %d\n", PMPtr->Shutdown.Flags.Laser);
    while(!PMPtr->Shutdown.Flags.Laser) {
        usleep(50000);
        //printf("Entered laser while loop\n");
        PMPtr->Heartbeats.Flags.Laser = 1;

        // if (PMPtr->Heartbeats.Flags.Laser == 0) 
        // std::cout << "Resetting laser heartbeat to 1" << std::endl;
        //     PMPtr->Heartbeats.Flags.Laser = 1;
        if (PMPtr->PMHeartbeats.Flags.PM == 1) 
            PMPtr->PMHeartbeats.Flags.PM = 0;
    }
    printf("Shutdown status high\n");
	shmdt(SMlsr);
	return 0;
}

    /* 
	Lsrptr = (Laser *)SMlsr;
	Lsr.numData = Lsrptr->numData;
	Lsr.data[0] = Lsrptr->data[0];
	printf("numData = %d, Data = %d \n", Lsr.numData, Lsr.data[0]);
    */