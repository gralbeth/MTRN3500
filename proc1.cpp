//Process 1
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
typedef struct GPS
{
	double Lat;
	double Long;
}GPS;

int main(int argc, char* argv[])
{
	GPS Novatel;
	int shmid;
	//key_t key;
	double* dblPtr;
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

	Novatel.Lat = 128402.293;
	Novatel.Long = 29384.29394103;
// Write to SM
	dblPtr = (double *)shm;
	*dblPtr = Novatel.Lat;
	*(dblPtr+1) = Novatel.Long;
	while(getchar()!= 'Q');
	shmdt(shm);
	return 0;
}