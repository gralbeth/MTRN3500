#include <structs.h>
#include <SM.h>

using namespace std;

int startProcess(char* str);

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
    PMPtr->Heartbeats.Flags.Laser = 0;
    PMPtr->Shutdown.Status = 0x00;
//Assigns laser values in SM
	Lsrptr = (Laser *)SMlsr;
	Lsrptr->numData = 450;
	Lsrptr->data[0] = 80;
    while(1) {
        printf("Laser Heartbeat: %d\n",PMPtr->Heartbeats.Flags.Laser);
        if (PMPtr->Heartbeats.Flags.Laser == 1) {
            PMPtr->Heartbeats.Flags.Laser = 0;
        }
        usleep(100);
    }
	shmdt(SMlsr);
    shmdt(SMpm);
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
