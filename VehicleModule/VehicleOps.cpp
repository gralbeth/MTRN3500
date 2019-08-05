#include "VehicleOps.h"

int VehicleSetup() {

    struct sockaddr_in server;

    int sock; 
    sock = socket(AF_INET , SOCK_STREAM , 0);

    if (sock == -1)
    {
        printf("Could not create socket");
    }
    printf("Socket created\n");
     
    server.sin_addr.s_addr = inet_addr("192.168.1.200");
    server.sin_family = AF_INET;
    server.sin_port = htons( VEHICLE_PORT );
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
    }
     
    printf("Connected\n");

    return sock;
}

void VehicleOps(int sock, PM* PMPtr, bool *flag) {
   
    char vehicleString[54];
    *flag = !(*flag);
    sprintf(vehicleString, "# %lf %lf %d #", PMPtr->RemoteSteering, PMPtr->RemoteSpeed, *flag);
    std::cout << "String sent is: " << vehicleString << std::endl;
    send(sock,vehicleString,strlen(vehicleString),0); // Send string
}