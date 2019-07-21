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

int main(int argc, char* argv[])
{
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

    /* 
	Lsrptr = (Laser *)SMlsr;
	Lsr.numData = Lsrptr->numData;
	Lsr.data[0] = Lsrptr->data[0];
	printf("numData = %d, Data = %d \n", Lsr.numData, Lsr.data[0]);
    */
    printf("PMPtr->Shudown flag laser: %d\n", PMPtr->Shutdown.Flags.Laser);
    while(!PMPtr->Shutdown.Flags.Laser) {
        //printf("Entered laser while loop\n");
        if (PMPtr->Heartbeats.Flags.Laser == 0) 
            PMPtr->Heartbeats.Flags.Laser = 1;
        if (PMPtr->PMHeartbeats.Flags.PM == 1) 
            PMPtr->PMHeartbeats.Flags.PM = 0;
        usleep(20);
    }

//Strtok

	shmdt(SMlsr);
	return 0;
}

