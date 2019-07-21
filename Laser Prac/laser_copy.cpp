//
//  laser.cpp
//  assign2
//

#define _USE_MATH_DEFINES
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "structDat.h"
#include <sstream> //stringstream lib
#include <cmath>

struct Points {
    double X;
    double Y;
};


class Laser
{
private:
    int portNumber;
    int sock;
    std::string ip;
    char message[1000] , server_reply[4000]; //where the data is stored
    
    
    
public:
    int numData;
    Points Ranges[361]; //number of data points = 361
    
    
    ~Laser() {}
    
    Laser(std::string ip,int portNumber) { //need to input an ip string and a port num
        
        // *********************** Server Setup ***********************
        
        struct sockaddr_in server;
        
        this->ip = ip; //host name
        this->portNumber = portNumber; //port name
        
        //creating a socket
        sock = socket(AF_INET , SOCK_STREAM , 0);
        if (sock == -1)
        {
            printf("Could not create socket");
        }
        puts("Socket created");
        
        //Sets up the server
        server.sin_addr.s_addr = inet_addr(ip.c_str());
        server.sin_family = AF_INET;
        server.sin_port = htons(portNumber);
        
        //Connect to remote server
        if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
        {
            perror("connect failed. Error");
            // return 1;
        }
        
        // *********************** Authenticating ***********************

        //authenticate with the server
        // i.e. send student number and check response
        const char studentNumber[9] = "5157353\n";
        send(sock, studentNumber, strlen(studentNumber),0);
        
        int bytes = recv(sock, server_reply, sizeof(server_reply) , 0);
        
        //Receive a reply from the server
        if( bytes < 0)
        {
            puts("recv failed");
        }
        
        // debug stuff to see what you receive
        puts("Server reply :");
        puts(server_reply);
        
    }
    
    void parseScan(){
        //Placing the data into a stringstream obj
        std::istringstream ss(server_reply);
        
        
        //Changing data into strings
        std::string dataStr;
        

        // one way to do this is to copy data into a stringstream object
        // then read from it into a string object until you get to the data you want
        // then read in the resolution (its stored a hex value)
        
        for (int i=0; i<23; i++) {
            ss >> dataStr;
        }
        int data;
        ss >> std::hex >> data;
        std::cout << "Start angle: " << data << std::endl;
        double startAngle = data/10000.0; // process Data to get startAngle (skip 23 spaces)
        // then fill in an int32 byte by byte and divide the result by 10000.
        
        ss >> std::hex >> data;
        std::cout << "Resolution: " << data << std::endl;
        double resolution = data/10000.0;//process Data to get resolution, i.e. skip 24 spaces (0x20) in Data,
					   //then fill in an Int16 byte by byte, divide result by 10000.
        
        ss >> std::hex >> data;
        std::cout << data << std::endl;
        numData = data; // process Data to get numData skip 25 spaces and
        
        // then fill in an uint_16  byte by byte.
        
        double theta;
        // process Data to update ranges skip 26 spaces, then run a for loop
        for (int i = 0; i < numData; i++)
        {
            ss >> std::hex >> data;
            theta = startAngle + i * resolution;

            Ranges[i].X = data*cos(theta*M_PI/180.0);// Process data to fill these values
            Ranges[i].Y = data*sin(theta*M_PI/180.0);// The angle is converted to radians
        }
        
    }
    
    bool getSingleScan(){
        int bytes;
        const char STX[1] = {0x02};
        const char ETX[1] = {0x03};
        
        send(sock , STX, 1 , 0); //STX
        
        char scanOnce[100] = "sRN LMDscandata";
        //send laser data request to server
        bytes = send(sock , scanOnce , strlen(scanOnce) , 0);
        if(bytes < 0)
        {
            puts("Send failed");
            // return 1;
        }
        send(sock , ETX, 1 , 0); //ETX
        
        bytes = recv(sock, server_reply, sizeof(server_reply) , 0);
        
        //Receive a reply from the server
        if( bytes < 0)
        {
            puts("recv failed");
        }
        
        
        // debug stuff to see what you receive
        
        if (server_reply[0] != 0x02 || server_reply[bytes-1] != 0x03) {
            printf("bad data skipping\n");
            return false;
        }
        
        parseScan();
        return true;
    }
};


int main(){
    
    // ============= SHARED MEMORY =====================
    // ***** Laser SM ******
     int shmid; //share memory id
     void* shm; //shared memory
     int HBCount = 0;
    
     //Create the segment
     if ((shmid = shmget(lasID, sizeof(laser), IPC_CREAT | 0666)) < 0)
     {
     printf("Shared memory allocation failed\n");
     return -1;
     }
     
     //Attach the segment to the server's address space
     if ((shm = shmat(shmid, NULL, 0)) == NULL)
     //shmat -> attaches shared memory segments identified by shmid
     //shmid -> the identifier of the segment
     {
     printf("Shared memory attach failed\n");
     return -1;
     }
    
    laser* laserPtr = (laser*)shm;
    
    // ***** PM SM ******
    
    //Create the segment
    if ((shmid = shmget(pmID, sizeof(processManagement), IPC_CREAT | 0666)) < 0)
    {
        printf("Shared memory allocation failed\n");
        return -1;
    }
    
    //Attach the segment to the server's address space
    if ((shm = shmat(shmid, NULL, 0)) == NULL)
        //shmat -> attaches shared memory segments identified by shmid
        //shmid -> the identifier of the segment
    {
        printf("Shared memory attach failed\n");
        return -1;
    }
    processManagement* pMPtr = (processManagement*)shm;
    
    
    pMPtr->shutDown.flags.laser = 0;
    
    Laser MyLaser = Laser("192.168.1.200", 23000);
    
    usleep(100*1000);

    
    while(!pMPtr->shutDown.flags.laser){
        if(pMPtr->heartbeat.flags.laser == 1){
            //counter
            HBCount++;
            std::cout << "PM failing " << HBCount << "\n";
            if (HBCount > 40) { //counter > certain value
                std::cout << "Laser SHUTDOWN\n";
                pMPtr->shutDown.status = 0xff;
                exit(0);
            }
           
        }
        
        pMPtr->heartbeat.flags.laser = 1; // set = 1 after reset in PM
        
        laserPtr->numPoints = MyLaser.numData;
        
        if ( MyLaser.getSingleScan() ) {//if we obtain data
            
            // first data point
            printf("X = %f \tY = %f\n", MyLaser.Ranges[0].X, MyLaser.Ranges[0].Y);
            
            for (int i = 0; i < MyLaser.numData; i++)
            {
                laserPtr->x[i] = MyLaser.Ranges[i].X;
                laserPtr->y[i] = MyLaser.Ranges[i].Y;
            }
        }
        
        usleep(200*1000);
    }
    
    return 0;
}
