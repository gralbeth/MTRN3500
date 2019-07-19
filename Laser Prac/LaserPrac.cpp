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

#define NO_POINTS 25
#define STARTING_ANGLE 23
#define RESOLUTION 24
#define POINTS 26
#define DEG2RAD 3.14159265358972/100.0

struct Data {
    double X;
    double Y;
};



int main(int argc , char *argv[])
{
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];
     
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

    puts("Verif reply :");
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

   // Recieve data

   bytes = recv(sock, server_reply, sizeof(server_reply),0);

    if( bytes < 0)
    {
        puts("recv failed");
    }
    
    // debug stuff to see what you receive
    puts("Server reply :");
    puts(server_reply);
    std::cout << sizeof(server_reply) << std::endl;
    
    std::istringstream ss(server_reply);
    std::cout << ss.str() << std::endl;
 
    /* 
    std::vector<char *> newData;
    ss >> newData;
    newData.push_back(strtok(server_reply," "));
    while (newData != NULL) {
        std::cout << std::hex << newData << std::endl;
       // printf("%s\n", newData);
        newData.push_back(strtok(server_reply," "));
    }*/
/*
 std::string dataStr;

    // Dont want the first 30 entries
    for (int i = 0; i < STARTING_ANGLE; i++) {
        ss >> dataStr;
         std::cout << "Angle: "<< std::hex << dataStr << std::endl;
    }

    int data; 
    ss >> std::hex >> data;
    std::cout << "Angle: " << data << std::endl;
     
    ss >> std::hex >> data;
    std::cout << "Angle: " << data << std::endl;

    ss >> std::hex >> data;
    std::cout << "Angle: " << data << std::endl;   
    
    //printf("%s\n", dataStr.c_str());
 */
    
   
 
    //keep communicating with server
    while(1)
    {
  
 
            //Begin parse scan

        // printf("Enter message : ");
        // scanf("%s" , message);
         
        // //Send some data
        // if( send(sock , message , strlen(message) , 0) < 0)
        // {
        //     puts("Send failed");
        //     return 1;
        // }
        
        // bytes = recv(sock , server_reply , 2000 , 0);
        // //Receive a reply from the server
        // if( bytes < 0)
        // {
        //     puts("recv failed");
        //     break;
        // }
         
        // puts("Server reply :");
        // puts(server_reply);
    }
     
    close(sock);
    return 0;
}