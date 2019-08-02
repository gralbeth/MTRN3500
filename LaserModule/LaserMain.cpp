#include <Laser.h>
#include <structs.h>
#include <kbhit.h>
#include <iostream>

#define WAIT_COUNT 50

int waitCount;

int main(int argc, char* argv[])
{
    Laser* Lsrptr;
	void* SMlsr;
    void* SMpm;
    PM* PMPtr; 
    waitCount = 0;

    long int ms = duration_cast< milliseconds >(system_clock::now().time_since_epoch()).count();
    std::cout << "Laser opened at time: " << ms << std::endl;
    
//	SMlsr = SMCreate(LASER_KEY,sizeof(Laser));
    SMpm = SMCreate(PM_KEY,sizeof(PM));
// Read from SM
    PMPtr = (PM*)SMpm;

    Laser lsr("192.168.1.200", LASER_PORT, PMPtr); // Initial setup

    int lasRet = 0;
    //PMPtr->Heartbeats.Flags.Laser = 0;

   // std::cout << "PMPtr->Shudown flag laser: " << PMPtr->Shutdown.Flags.Laser << std::endl;
    while(!PMPtr->Shutdown.Flags.Laser) {
   // while(true) {
        usleep(5000);
        if (PMPtr->Heartbeats.Flags.Laser) {
            if (++waitCount > WAIT_COUNT) {
                std::cout << "Laser shutting down PM" << std::cout;
                PMPtr->Shutdown.Status = 0xFF; // PM Failure
            }
        } else {
            waitCount = 0;
            PMPtr->Heartbeats.Flags.Laser = 1;
        }

        long int ms1 = duration_cast< milliseconds >(system_clock::now().time_since_epoch()).count();
        std::cout << "Pre laser time: " << ms1 << std::endl;
        lasRet = lsr.LaserOps();
        if (lasRet != 0) {
            while(!kbhit());
            return 0;
        } //PMPtr->Shutdown.Status = 0xff;

        
        long int ms2 = duration_cast< milliseconds >(system_clock::now().time_since_epoch()).count();
        std::cout << "Post laser time: " << ms2 << std::endl;
        // if (PMPtr->Heartbeats.Flags.Laser == 0) 
        // std::cout << "Resetting laser heartbeat to 1" << std::endl;
        //     PMPtr->Heartbeats.Flags.Laser = 1;

    }
    lsr.closeSock();
    std::cout << "Shutdown status high" << std::endl;
	//shmdt(SMlsr);
    int detRet = shmdt(SMpm);
    if (detRet != 0) {
        std::cout << "SMpm detach failed" << std::endl;
    }
    shmctl(PM_KEY, IPC_RMID, NULL); 
	return 0;
}
