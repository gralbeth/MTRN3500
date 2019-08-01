 
int main() {
    void* SMpm;
    PM* PMPtr; 


    std::cout << "Laser opened at time: " << ms << std::endl;
    
//	SMlsr = SMCreate(LASER_KEY,sizeof(Laser));
    SMpm = SMCreate(PM_KEY,sizeof(PM));
// Read from SM
    PMPtr = (PM*)SMpm;

    // No need to authenticate
    // Begin with same IP addr and diff port num
    // Sleep 
    // Will return 112 data points
    // Header is 25 bytes long
    // Sync bytes are 3 bytes
    // Data starts at 28th index
    // Take bytes and convert it to doubles - 8 bytes per double
}

