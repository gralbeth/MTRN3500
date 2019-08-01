#include <structs.h>
#include <kbhit.h>
#include <SM.h>

using namespace std;
using namespace std::chrono;

int startProcess(char* str);

int waitCount[NUM_PROCESSES];

int main(int argc, char* argv[])
{
    // Set all wait counts to zero
    for (int i = 0; i < NUM_PROCESSES; i++) {
        waitCount[i] = 0;
    }

    PM* PMPtr; 
	//Laser* Lsrptr;
	void* SMlsr;
    void* SMpm;
	//SMlsr = SMCreate(LASER_KEY,sizeof(LaserData));
    std::cout << "Size of PM: " << sizeof(PM) << std::endl;
    SMpm = SMCreate(PM_KEY, sizeof(PM));

//Beginning LaserMain
    //int val2 = startProcess((char*)"bin/display.sh");
   // int val = startProcess((char*)"bin/laser.sh");
    int val = startProcess((char*)"bin/lsr");
    int val2 = startProcess((char*)"bin/dm");
    int val3 = startProcess((char*)"bin/gps");

//PM
    PMPtr = (PM *)SMpm;
    PMPtr->Shutdown.Status = 0x00;
    PMPtr->Shutdown.Flags.PM = 0;
    PMPtr->Heartbeats.Status = 0x00;
    printf("Setting shutdown status: %d\n",PMPtr->Shutdown.Status);
    long int ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch()).count();
    std::cout << "Time before sleep: " << ms << std::endl;
    usleep(200000);
    
    while(!PMPtr->Shutdown.Flags.PM) { 
        if (duration_cast< milliseconds >(system_clock::now().time_since_epoch()).count() - ms > 1000) {
            printf("Laser Heartbeat: %d\n",PMPtr->Heartbeats.Flags.Laser);
            ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch()).count();
        }
        
        usleep(20000);
        // Update all heartbeats
        if (PMPtr->Heartbeats.Flags.Laser == 1) {
            waitCount[LSR_INDX] = 0;
            PMPtr->Heartbeats.Flags.Laser = 0;
        } else { //Wait more, check again     
            if (++waitCount[LSR_INDX] > BUFFER_TIME) {
                printf("LSR Heartbeat not reset\n");
                PMPtr->Shutdown.Status = 0xFF;
            }
        }
        if (PMPtr->Heartbeats.Flags.GPS == 1) {
            waitCount[GPS_INDX] = 0;
            PMPtr->Heartbeats.Flags.GPS = 0;
        } else { //Wait more, check again     
            if (++waitCount[GPS_INDX] > BUFFER_TIME) {
                printf("GPS Heartbeat not reset\n");
                PMPtr->Shutdown.Status = 0xFF;
            }
        }
        if (PMPtr->Heartbeats.Flags.Display== 1) {
            waitCount[DISP_INDX] = 0;
            PMPtr->Heartbeats.Flags.Display = 0;
        } else { //Wait more, check again     
            if (++waitCount[DISP_INDX] > BUFFER_TIME) {
                printf("Display Heartbeat not reset\n");
                PMPtr->Shutdown.Status = 0xFF;
            }
        }

        if (kbhit()) {
            getchar();
            printf("Key hit\n");
            PMPtr->Shutdown.Status = 0xFF;
            usleep(1000);
        }
    }

    printf("Process Management terminated normally\n");

	// int detRet = shmdt(SMlsr);
    // if (detRet != 0) {
    //     printf("SMlsr detach failed\n");
    // }
    // shmctl(LASER_KEY, IPC_RMID, NULL);
    int detRet = shmdt(SMpm);
    if (detRet != 0) {
        printf("SMpm detach failed\n");
    }
    shmctl(PM_KEY, IPC_RMID, NULL);

	return 0;
}

int startProcess(char* str) {
    pid_t pid = fork();
    if (pid == 0) { // Child Process; start a new process
        execlp("open", "open", "-a", "Terminal", (char*)str, (char *)NULL); // Go to a new process
        return -1; // something has gone wrong if we're here!
    } else if (pid > 0) { // Parent process; return the pid of the child process
        return pid;
    }

    // Something went wrong if we get to here!
    // Return an error
    return -1;
}

/* 
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

*/

/*
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
 */