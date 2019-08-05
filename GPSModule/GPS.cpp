#include <GPS.h>

#pragma pack(1)

void GPSOps(int sock) {
   
    usleep(10000);
    char Data[112];
    GPSData gps;
   // int ret = recv(sock, gps->Data, sizeof(gps),0);
    int ret = recv(sock, Data, sizeof(Data),0);
    if (ret < 0) {
        std::cout << "GPS Data recv fail" << std::endl;
    } 
    std::cout << "Recieved " << ret << " bytes successfully" << std::endl;
    // int begin;
    // for (begin = 0; begin < ret; begin++) {
    //     if (Data[begin] == 0xaa && Data[begin+1] == 0x44 && Data[begin+2] == 0x12) break;
    // }

    // if (ret < sizeof(gps)) return;

    // int headerLength = Data[begin + 3];
    int headerLength = 0;


    unsigned char * BytePtr = (unsigned char*)&gps;
    unsigned char Buffer[sizeof(GPSData)];
    for (int i = 0; i < sizeof(GPSData); i++) {
        *(BytePtr+i) = Data[i];
        Buffer[i] = Data[i]; 
    }

    std::cout << "Size of GPSData: " << sizeof(GPSData) << std::endl;
    std::cout << "Readings - Northing: " << gps.Northing << "\tEasting: " << gps.Easting << "\tHeight: " << gps.Height << std::endl;
    unsigned long CRCCalc = CalculateBlockCRC32(108,Buffer);

    double CRCDiff = CRCCalc - gps.CRC;
    std::cout << "CRC from string: " << gps.CRC << "\tCRC calculated: " << CRCCalc << "\tDiff: " << CRCDiff << std::endl;

}

int GPSConnect() {
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
    server.sin_port = htons( GPS_PORT );
 
    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
    }
     
    printf("Connected\n");

    return sock;
}

void GPSDisconnect(int sock) {
    close(sock);
}

/*
	//Get Northing
	unsigned char* BytePtr = nullptr; 
	BytePtr = (unsigned char*)&gpsData->northing; 
	int start = NORTHING_OFFSET;
	for (int i = start; i < start + sizeof(double); i++) { 
		*(BytePtr++) = newData[i]; 
	}
 */