#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#define RCVBUFSIZE 32

void DieWithError(char* errorMessage);
void log_msg(char *msg);
void HandleTCPClient(int clntSocket){

    char echoBuffer[RCVBUFSIZE]; //Buffer for echo String
    int recvMsgSize;
    int totalByteRecvd=0;
 
    
    //recieve message from client    
    if((recvMsgSize=recv(clntSocket, echoBuffer, RCVBUFSIZE,0))<0)
        DieWithError("recv() failed");
    //write the message on Server side 버퍼에 기존의 값이 남아있음
    printf(echoBuffer);
    printf("\n");
    
    //mark the log on file
    log_msg(echoBuffer);
    
    //send received string and receive again until end of transmission
    while(recvMsgSize>0){ //zero indicates end of transmission
        //Echo message back to clients
        if(send(clntSocket,echoBuffer, recvMsgSize,0)!=recvMsgSize)
            DieWithError("send() failed");
     //see if there is more data to receive
        if((recvMsgSize=recv(clntSocket, echoBuffer, RCVBUFSIZE,0))<0)
            DieWithError("recv() failed");
            
    }
    //clean the Buffer
    memset(echoBuffer,0,sizeof(echoBuffer));
    close(clntSocket);
}

void log_msg(char *msg){
    
    FILE * fp;

    fp=fopen("echo_history.log","a");
    fprintf(fp,"write log on file: %s\n",msg);
    fclose(fp);
}
