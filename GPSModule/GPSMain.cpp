 
 
 SMObject PMOBj("PMObj", sizeof(PM));
    PM* PMSMPtr = nullptr;

    PMObj.SMAccess();
    if (PMObj.SMAccessError) {
        Console::WriteLine("Shared memory access failed");
        return -2;
    }