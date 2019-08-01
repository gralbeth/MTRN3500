#include <GPS.h>

#pragma pack(1)

void GPSOps(int sock) {
   
    usleep(100000);
    char Data[112];
    GPSData gps;
   // int ret = recv(sock, gps->Data, sizeof(gps),0);
    int ret = recv(sock, Data, sizeof(Data),0);
    if (ret < 0) {
        std::cout << "GPS Data recv fail" << std::endl;
    } 
    std::cout << "Recieved " << ret << " bytes successfully" << std::endl;
     
    // Northing data
	int start = NORTHING_INDEX;
    unsigned char * BytePtr = NULL;
    BytePtr = (unsigned char*)&gps.Northing;
	for (int i = start; i < start + sizeof(double); i++) { 
		*(BytePtr++) = Data[i]; 
	}

    BytePtr = (unsigned char*)&gps.Easting; 
	start = EASTING_INDEX;
	for (int i = start; i < start + sizeof(double); i++) { 
		*(BytePtr++) = Data[i]; 
	}

    BytePtr = (unsigned char*)&gps.Height; 
	start = HEIGHT_INDEX;
	for (int i = start; i < start + sizeof(double); i++) { 
		*(BytePtr++) = Data[i]; 
	}

    std::cout << "Northing: " << gps.Northing << std::endl;
    std::cout << "Easting: " << gps.Easting << std::endl;
    std::cout << "Height: " << gps.Height << std::endl;
    
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