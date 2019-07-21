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
    char serverPacket[4000];
    char message[1000];

    public:
    int numData;
    u_int16_t vals[361];
    Data Values[361];

     ~Laser() {}
    Laser();
    Laser(std::string ip_addr,int portNum);
    bool getDataPacket();
    void processDataPacket();
    void closeSock();
    void polarToXY();
};


int main(int argc , char *argv[])
{
    Laser lsr("192.168.1.200", 23000);
    lsr.getDataPacket();
    lsr.processDataPacket();
    lsr.closeSock();
  // lsr.polarToXY();
  
   
    // Vals array now holds polar values


   // std::istringstream ss(server_reply);
   // std::cout << ss.str() << std::endl;
     
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
    puts("Socket created");
     
    server.sin_addr.s_addr = inet_addr(ipAddr.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons( portNum );
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");

    }
     
    puts("Connected\n");

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

    if( bytes < 0)
    {
        puts("recv failed");
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

    for (int i = 0; i < NUM_POINTS; i++) {
        msg = strtok(NULL," ");
    }

    ss << std::hex << msg;
    ss >> numData;
    std::cout << "NumData: " << numData << std::endl;

    std::cout << "-----------------" << std::endl;
 
    char * dat;
    std::stringstream str1;
    u_int16_t val;

    for (int i = 0; i < numData; i++) {
        dat = strtok(NULL," ");
        val = 0;
        if (dat != NULL) {
            //std::cout << i << ": " << dat << std::endl;
            str1 << std::hex << dat;
            str1 >> vals[i];
            str1.clear();
            std::cout << "Polar val #" << i << "\t" << vals[i] << std::endl;
         }
     }
}

void Laser::polarToXY() {
    for (int i = 0; i < numData; i++) {
        double theta = START_ANGLE + i * RESOLUTION;
        if (theta > 90) {
            theta = theta - 90;
        }
        Values[i].X = vals[i]/1000.0 * abs(cos(theta * DEG2RAD));
        Values[i].Y = vals[i]/1000.0 * abs(sin(theta * DEG2RAD));
        std::cout << "X Val for " << i << " " << Values[i].X <<  "\tY Val " << Values[i].Y << std::endl;
    }
}
 
 void Laser::closeSock() {
     close(sock);
 }

 
