void GPSConnection() {

    std::cout << "Entered gps connection" << std::endl;
    
    struct sockaddr_in server;

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
}

void GPSStoreData() {

}

void GPSPrintData() {

}