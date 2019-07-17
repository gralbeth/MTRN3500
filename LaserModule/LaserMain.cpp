// #include "../Include/SMObject.h"

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <structs.h>
#include <SM.h>

int main(int argc, char* argv[])
{
    Laser Lsr;
	Laser* Lsrptr;
	void* shm;
    
	shm = SMCreate(LASER_KEY,sizeof(Laser));

// Read from SM
	Lsrptr = (Laser *)shm;
	Lsr.numData = Lsrptr->numData;
	Lsr.data[0] = Lsrptr->data[0];
	printf("Lat = %10.3f, Long = %10.3f \n", Lsr.numData, Lsr.data[0]);
	shmdt(shm);
	return 0;
}

