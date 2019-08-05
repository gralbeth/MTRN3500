 
#include <GPS.h>

#define WAIT_COUNT 50
int waitCount;


int main() {
    void* SMpm;
    PM* PMPtr; 
    waitCount = 0;
    SMpm = SMCreate(PM_KEY,sizeof(PM));
// Read from SM
    PMPtr = (PM*)SMpm;
    int sock = GPSConnect();

    while (!PMPtr->Shutdown.Flags.GPS) {
        if (PMPtr->Heartbeats.Flags.GPS) {
            if (++waitCount > WAIT_COUNT) {
				std::cout << "GPS shutting down PM" << std::cout;
                PMPtr->Shutdown.Status = 0xFF; // PM Failure
            }
        }
        PMPtr->Heartbeats.Flags.GPS = 1;
        GPSOps(sock);
    }
    GPSDisconnect(sock);
    int detRet = shmdt(SMpm);
    if (detRet != 0) {
        std::cout << "SMpm detach failed" << std::endl;
    }
    shmctl(PM_KEY, IPC_RMID, NULL); 

    return 0;
}

    // No need to authenticate
    // Begin with same IP addr and diff port num
    // Sleep 
    // Will return 112 data points
    // Header is 25 bytes long
    // Sync bytes are 3 bytes
    // Data starts at 28th index
    // Take bytes and convert it to doubles - 8 bytes per double