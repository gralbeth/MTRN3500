//Process 2
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct _GPS
{
	double Lat;
	double Long;
}GPS;

int main()
{
	GPS Novatel;
	int shmid;
	//key_t key;
	GPS* gpsPtr;
	void* shm;
	int key = 1234;
	if ((shmid = shmget(key, sizeof(GPS), IPC_CREAT | 0666)) < 0)
	{
		printf("Shared memory allocation failed\n");
		return -1;
	}
	if ((shm = shmat(shmid, NULL, 0)) == NULL)
	{
		printf("Shared memory attach failed\n");
		return -1;
	}
// Read SM
	gpsPtr = (GPS*)shm;
	Novatel.Lat = gpsPtr->Lat;
	Novatel.Long = gpsPtr->Long;
	printf("Lat = %10.3f, Long = %10.3f \n", Novatel.Lat, Novatel.Long);
	shmdt(shm);
	return 0;
}