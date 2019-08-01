#include <structs.h>
#include <Laser.h>

int Laser::LaserOps() {

    std::cout << "Entered laserOps" << std::endl;
    int ret = getDataPacket(); // Send 0x02, sRN..., 0x03 to request and recieve data packet
    while (ret != 0) {
        std::cout << "Data packet fail" << std::endl;
        ret = getDataPacket();
      //  return -1;
    }
    processDataPacket(); // Convert from Hex to polar
  
    polarToXY(); // Polar to XY cartesian
   // printVals(5);
    return 0;
}

Laser::Laser() {

}
    
Laser::Laser(std::string ip_addr,int portNum, PM* PMPtr) {
    PMSMPtr = PMPtr;
    std::cout << "Entered laser constructor" << std::endl;
    ipAddr = ip_addr;
    this->portNum = portNum;
    struct sockaddr_in server;

    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    printf("Socket created\n");
     
    server.sin_addr.s_addr = inet_addr(ipAddr.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons( portNum );
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
    }
     
    printf("Connected\n");

    //Create socket
    // Verify connection via student number
    const char studentNumber[9] = "5061927\n";
    send(sock,studentNumber,strlen(studentNumber),0);

    usleep(100);
    int bytes;
       // message[0] = '\0';
    bytes = recv(sock, message, sizeof(message),0);
 
  
    if( bytes < 0)
    {
    
        puts("recv failed");
    }

    puts("Verification reply :");
    puts(message);
    //usleep(500);

}
   

bool Laser::getDataPacket() {
    int bytes;
   char buffer[ 256 ];
    const char start[1] = {0x02};
    const char end[1] = {0x03};

    send(sock,start,1,0); // Start transmission

    // Start data transfer
    char reqData[100] = "sRN LMDscandata";
    bytes = send(sock, reqData, strlen(reqData), 0); // returns -1 on error
    if (bytes < 0) {
        strerror_r( errno, buffer, 256 );
        printf("Error %s", buffer);
        std::cout << "Bytes from packet request: " << bytes << std::endl;
        puts("Request failed for packet");
        return -1;
    }

    send(sock, end, 1, 0); // End transmission

    usleep(50000);
   // Recieve data

    bytes = recv(sock, serverPacket, sizeof(serverPacket),0);
    std::cout << "Bytes: " << bytes << std::endl;
    while (bytes < 1500) {

        send(sock,start,1,0); // Start transmission
        bytes = send(sock, reqData, strlen(reqData), 0);
        if (bytes < 0) {

            std::cout << "Request failed in loop" << std::endl;
        }
        send(sock, end, 1, 0);
        usleep(5000);
        bytes = recv(sock, serverPacket, sizeof(serverPacket),0);
         std::cout << "Bytes: " << bytes << std::endl;
    }
    
    // debug stuff to see what you receive
    puts("Server data:");
    puts(serverPacket);
    
    char dataPack[2048]; 
    //std::string pack(serverPacket);
    //strcpy(dataPack,pack.c_str());
    memcpy(dataPack,&serverPacket,sizeof(serverPacket));

    char * msg = strtok(dataPack," ");
    char * msg2 = strtok(NULL," "); // should be lmd etc
    std::cout << "String comp: " << strcmp(msg2,"LMDscandata") << std::endl;

    if (strcmp(msg2,"LMDscandata")) {
        std::cout << "Wrong header\n" << std::endl;
        return -1;
    }
    std::cout << "----------------------------------" << std::endl;

    return 0;
}

void Laser::processDataPacket() {
    char * dataStr = serverPacket;
    
    char * msg;
    std::stringstream ss;
    msg = strtok(dataStr," ");

    //Check message is right here??
    for (int i = 0; i < STARTING_ANGLE; i++) {
        msg = strtok(NULL," ");
    }

    ss << std::hex << msg;
    ss >> startingAngle;
    std::cout << "StartingAngle: " << startingAngle << std::endl;
    std::cout << "-----------------" << std::endl;
    ss.clear();

    msg = strtok(NULL," ");

    ss << std::hex << msg;
    ss >> stepWidth;
    std::cout << "StepWidth: " << stepWidth/10000.0 << std::endl;
    std::cout << "-----------------" << std::endl;
    ss.clear();
    msg = strtok(NULL," ");

    ss << std::hex << msg;
    ss >> numData;
    std::cout << "NumData: " << numData << std::endl;
    std::cout << "-----------------" << std::endl;
    ss.clear();
    msg = strtok(NULL," ");


    char * dat;
    std::stringstream str1;

    for (int i = 0; i < numData; i++) {
        dat = strtok(NULL," ");
        if (dat != NULL) {
            //std::cout << i << ": " << dat << std::endl;
            str1 << std::hex << dat;
            str1 >> vals[i];
            str1.clear();
            //std::cout << "Polar val #" << i << "\t" << vals[i] << std::endl;
         }
     }
}

void Laser::polarToXY() {
    for (int i = 0; i < numData; i++) {
        double theta = START_ANGLE + (double)i * RESOLUTION;
        std::cout << "Vals[i]: " << vals[i] << std::endl;
        ValuesX[i] = (double)vals[i]/1000.0 * sin(theta * DEG2RAD);
        ValuesY[i] = (double)vals[i]/1000.0 * cos(theta * DEG2RAD);
        
        
        PMSMPtr->XVals[i] = ValuesX[i];
        PMSMPtr->YVals[i] = ValuesY[i];

        std::cout << "PMSMPtr -  X: " <<  PMSMPtr->XVals[i] << " Y: " << PMSMPtr->YVals[i] << std::endl;
    }
}
    


void Laser::printVals(int numPrint) {
    for (int i = 0; i < numPrint; i++) {
       std::cout << "For " << i << " XVal: " << ValuesX[i] <<  "\tYVal: " << ValuesY[i] << std::endl;
    }
}
 
 void Laser::closeSock() {
     close(sock);
 }

 
