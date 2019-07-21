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

#define NUM_POINTS 25
#define SCALING 21
#define STARTING_ANGLE 23
#define STEP_WIDTH 24
#define POINTS 26
#define DEG2RAD 3.14159265358972/100.0

#define STEP_WIDTH 0.5


bool getDataPacket();
void saveData();

struct Data {
    double X;
    double Y;
};

class Laser {
    private:
    std::string ip_addr;
    int portNum;
    char serverPacket[4000];

    public:
    int numData;
    Data Values[361];

     ~Laser() {}
    Laser();
    Laser(std::string ip_addr,int portNum);
    bool getDataPacket();
    void saveData();
};

int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[4000];
     
    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    server.sin_addr.s_addr = inet_addr("192.168.1.200");
    server.sin_family = AF_INET;
    server.sin_port = htons( 23000 );
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
     
    puts("Connected\n");

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

   bytes = recv(sock, server_reply, sizeof(server_reply),0);

    if( bytes < 0)
    {
        puts("recv failed");
    }
    
    // debug stuff to see what you receive
    puts("Server data:");
    puts(server_reply);
    std::cout << "----------------------------------" << std::endl;

    char * strdata = server_reply;
    char * msg;
    msg = strtok(strdata," ");
    std::stringstream ss;
    u_int16_t vals[500] = {0};
    /* 
    for (int i = 0; i < 24; i++) {
        msg = strtok(NULL," ");
    }

    ss << std::hex << msg;
    u_int16_t ang;
    std::cout << ss.str() << std::endl;
    ss >> ang;
    std::cout << ang << std::endl;
*/

    for (int i = 0; i < NUM_POINTS; i++) {
        msg = strtok(NULL," ");
    }

    ss << std::hex << msg;
    u_int16_t numData;
    ss >> numData;
    std::cout << "NumData: " << numData << std::endl;

    std::cout << "-----------------" << std::endl;
 
    char * Pointdata = server_reply;
    char * dat;
    std::stringstream str(server_reply);
    std::stringstream str1;
    u_int16_t val;

    for (int i = 0; i < numData; i++) {
        dat = strtok(NULL," ");
        val = 0;
        if (dat != NULL) {
            std::cout << i << ": " << dat << std::endl;
            str1 << std::hex << dat;
            str1 >> vals[i];
            str1.clear();
            std::cout << "Polar val array: " << vals[i] << std::endl;
         }
     }

    // Vals array now holds polar values


   // std::istringstream ss(server_reply);
   // std::cout << ss.str() << std::endl;
     
    close(sock);
    return 0;
}
    
/* 
   

bool Laser::getDataPacket() {
    return 0;
}

void Laser::saveData() {

}

Laser::Laser(std::string ip_addr,int portNum) {
     this->ip_addr = ip_addr;
     this->portNum = portNum;
 }
 
 
*/