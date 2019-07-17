#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <structs.h>
#include <SM.h>

void* SMCreate(int key, double size) {
    int shmid;
    void* shm;
    if ((shmid = shmget(LASER_KEY, size, IPC_CREAT | 0666)) < 0)
	{
		printf("Shared memory allocation failed\n");
        //return = -1;
	}
	if ((shm = shmat(shmid, NULL, 0)) == NULL)
	{
		printf("Shared memory attach failed\n");
         //return = -1;
	}
    return shm;
}