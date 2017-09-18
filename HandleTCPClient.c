#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#define RCVBUFSIZE 32

void DieWithError(char* errorMessage);

void HandleTCPClient(int clntSocket){

    char echoBuffer[RCVBUFSIZE]; //Buffer for echo String
    int recvMsgSize;
    //recieve message from client
    if((recvMsgSize=recv(clntSocket, echoBuffer, RCVBUFSIZE,0))<0)
        DieWithError("recv() failed");
    //send received string and receive again until end of transmission
    while(recvMsgSize>0){ //zero indicates end of transmission
        //Echo message back to clients
        if(send(clntSocket,echoBuffer, recvMsgSize,0)!=recvMsgSize)
            DieWithError("send() failed");
     //see if there is more data to receive
        if((recvMsgSize=recv(clntSocket, echoBuffer, RCVBUFSIZE,0))<0)
            DieWithError("recv() failed");
            
    }
    close(clntSocket);
}
