#include<stdio.h> //printf
#include<string>    //strlen
#include<sys/socket.h>    //socket
#include<arpa/inet.h> //inet_addr
#include<unistd.h>
#include <sstream> //stringstream lib
#include <iostream>
#include <vector>
//#include <bits/stdc++.h>
#include <bitset>
#include <math.h>

#define NUM_POINTS 25
#define SCALING 21
#define STARTING_ANGLE 23
#define STEP_WIDTH 24
#define POINTS 26
#define DEG2RAD 3.14159265358972/180.0

//#define STEP_WIDTH 0.5
#define RESOLUTION 0.5
#define START_ANGLE 0


struct Data {
    double X;
    double Y;
};

class Laser {
    private:
    std::string ipAddr;
    int portNum;
    int sock; 
    char serverPacket[2048];
    char message[1000];

    public:
    int numData;
    int startingAngle;
    int stepWidth;
    u_int16_t vals[361];
    Data Values[361];

     ~Laser() {}
    Laser();
    Laser(std::string ip_addr,int portNum);
    bool getDataPacket();
    void processDataPacket();
    void closeSock();
    void polarToXY();
    void printVals(int numPrint);
};


int main(int argc , char *argv[])
{
    Laser lsr("192.168.1.200", 23000); // Initial setup
    int ret = lsr.getDataPacket(); // Send 0x02, sRN..., 0x03 to request and recieve data packet
    if (ret != 0) {
        std::cout << "Data packed fail" << std::endl;
        return 0;
    }
    lsr.processDataPacket(); // Convert from Hex to polar
    lsr.closeSock(); 
    lsr.polarToXY(); // Polar to XY cartesian
    lsr.printVals(5);
    
    return 0;
}

Laser::Laser() {

}
    
Laser::Laser(std::string ip_addr,int portNum) {
    ipAddr = ip_addr;
    this->portNum = portNum;
    struct sockaddr_in server;

    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    printf("Socket created");
     
    server.sin_addr.s_addr = inet_addr(ipAddr.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons( portNum );
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
    }
     
    printf("Connected\n");
}

bool Laser::getDataPacket() {
 
    //Create socket
    
    // Verify connection via student number
    const char studentNumber[9] = "5061927\n";
    send(sock,studentNumber,strlen(studentNumber),0);
    usleep(10);
    int bytes;
    bytes = recv(sock, message, sizeof(message),0);

    if( bytes < 0)
    {
        puts("recv failed");
    }

    puts("Verification reply :");
    puts(message);

    const char start[1] = {0x02};
    const char end[1] = {0x03};

    send(sock,start,1,0); // Start transmission

    // Start data transfer
    char reqData[100] = "sRN LMDscandata";
    bytes = send(sock, reqData, strlen(reqData), 0);
    if (bytes < 0) {
        puts("Request failed");
    }

    send(sock, end, 1, 0); // End transmission

    usleep(200);
   // Recieve data

   bytes = recv(sock, serverPacket, sizeof(serverPacket),0);
    std::cout << "Bytes: " << bytes << std::endl;
    while (bytes < 1750) {
        send(sock,start,1,0); // Start transmission
        bytes = send(sock, reqData, strlen(reqData), 0);
        if (bytes < 0) {
            puts("Request failed");
        }
        send(sock, end, 1, 0);
        std::cout << "Bytes: " << bytes << std::endl;
        bytes = recv(sock, serverPacket, sizeof(serverPacket),0);
    }
    
    // debug stuff to see what you receive
    puts("Server data:");
    puts(serverPacket);
    std::cout << "----------------------------------" << std::endl;

    return 0;
}

void Laser::processDataPacket() {
    char * dataStr = serverPacket;
    char * msg;
    std::stringstream ss;
    msg = strtok(dataStr," ");

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

        Values[i].X = (double)vals[i]/1000.0 * cos(theta * DEG2RAD);
        Values[i].Y = (double)vals[i]/1000.0 * sin(theta * DEG2RAD);
        
    }
}

void Laser::printVals(int numPrint) {
    for (int i = 0; i < numPrint; i++) {
       std::cout << "For " << i << " XVal: " << Values[i].X <<  "\tYVal: " << Values[i].Y << std::endl;
    }
}
 
 void Laser::closeSock() {
     close(sock);
 }

 
