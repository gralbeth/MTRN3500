#include <structs.h>
#include <VehicleOps.h>
#include <iostream>

int waitCount;

int main() {

    void* SMpm;
    PM* PMPtr; 
    waitCount = 0;
    int flag = 0;

    SMpm = SMCreate(PM_KEY,sizeof(PM)); //Create shared memory
    PMPtr = (PM*)SMpm;

    int sock = VehicleSetup();

    while(!PMPtr->Shutdown.Flags.Vehicle) {
        usleep(5000);
        if (PMPtr->Heartbeats.Flags.Vehicle) {
            if (++waitCount > WAIT_COUNT) {
                std::cout << "VehicleMain shutting down PM" << std::cout;
                PMPtr->Shutdown.Status = 0xFF; // PM Failure
            }
        } else {
            waitCount = 0;
            PMPtr->Heartbeats.Flags.Vehicle = 1;
        }
        vehicleOps(sock, PMPtr, flag);
    }

    std::cout << "Shutdown status high" << std::endl;
    int detRet = shmdt(SMpm);
    if (detRet != 0) {
        std::cout << "SMpm detach failed" << std::endl;
    }
    shmctl(PM_KEY, IPC_RMID, NULL); 
	return 0;
}

