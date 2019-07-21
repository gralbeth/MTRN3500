#include <structs.h>
#include <SM.h>
#include <unistd.h>
#include <chrono>
#include <iostream>

using namespace std;

using namespace std::chrono;

int startProcess(char* str);
int kbhit();

int main(int argc, char* argv[])
{
    PM* PMPtr; 
	Laser* Lsrptr;
	void* SMlsr;
    void* SMpm;
	SMlsr = SMCreate(LASER_KEY,sizeof(Laser));
    SMpm = SMCreate(PM_KEY, sizeof(PM));

//Beginning LaserMain
    int val = startProcess((char*)"bin/laser.sh");
//PM
    PMPtr = (PM *)SMpm;
    PMPtr->Shutdown.Status = 0x00;
    PMPtr->Shutdown.Flags.PM = 0;
    printf("Setting shutdown status: %d\n",PMPtr->Shutdown.Status);
//Assigns laser values in SM - modifies pm for some reason
	/* 
    Lsrptr = (Laser *)SMlsr;
    printf("Setting shutdown 1: %d\n",PMPtr->Shutdown.Status);
	Lsrptr->numData = 450;
    printf("Setting shutdown 2: %d\n",PMPtr->Shutdown.Status);
	Lsrptr->data[0] = 80;
    printf("Setting shutdown 3: %d\n",PMPtr->Shutdown.Status);*/
    //usleep(1000);

    auto start = std::chrono::system_clock::now();
    usleep(1000);
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
   // std::time_t end_time = std::chrono::system_clock::to_time_t(end);
   // std::cout << "finished computation at " << std::ctime(&end_time)
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

    long int ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch()).count();
    std::cout << "MS: " << ms << std::endl;
    usleep(1000);
    ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch()).count();
    std::cout << "MS post 1000 sleep: " << ms << std::endl;

    while(!PMPtr->Shutdown.Flags.PM) { 
        usleep(50);
        //printf("Laser Heartbeat: %d\n",PMPtr->Heartbeats.Flags.Laser);
      
        if (kbhit()) {
            getchar();
            printf("Key hit\n");
            PMPtr->Shutdown.Status = 0xFF;
        }
    }

    printf("Process Management terminated normally\n");

	int detRet = shmdt(SMlsr);
    if (detRet != 0) {
        printf("SMlsr detach failed\n");
    }
    shmctl(LASER_KEY, IPC_RMID, NULL);
    detRet = shmdt(SMpm);
    if (detRet != 0) {
        printf("SMpm detach failed\n");
    }
    shmctl(PM_KEY, IPC_RMID, NULL);

	return 0;
}

int startProcess(char* str) {
        pid_t pid = fork();
        if (pid == 0) { // Child Process; start a new process
            char *args[] = {str, NULL};
            execvp(args[0], args); // Go to a new process
            return 1; // something has gone wrong if we're here!
        } else if (pid > 0) { // Parent process; return 0
            return 0;
        }
        // Something went wrong if we get to here!
        // Return an error
        return 1;
    }

int kbhit() {
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}
