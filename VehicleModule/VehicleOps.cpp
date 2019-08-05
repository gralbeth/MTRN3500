#include <VehicleOps.h>

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
    server.sin_port = htons( VEHCILE_PORT );
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
    }
     
    printf("Connected\n");

    return sock;
}

void VehicleOps(int sock, PM* PMPtr, int flag) {
   
    usleep(10000);
    char Data[112];
    GPSData gps;
   // int ret = recv(sock, gps->Data, sizeof(gps),0);
    int ret = recv(sock, Data, sizeof(Data),0);
    if (ret < 0) {
        std::cout << "Vehicle Data recv fail" << std::endl;
    } 
    std::cout << "Recieved " << ret << " bytes successfully" << std::endl;

    char vehicleString[54];
    flag = !flag;
    sprintf(vehicleString, "# <%lf> <%lf> <%lf> #", PMPtr->RemoteSteering, PMPtr->RemoteSpeed, flag);
    std::cout << "String sent is: " << vehicleString << std::endl;

    send(sock,vehicleString,1,0); // Send string
}