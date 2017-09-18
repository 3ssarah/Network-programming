# include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define RCVBUFSIZE 32

void DieWithError(char* errorMessage);

int main(int argc, char* argv[]){

    int sock;
    struct sockaddr_in echoServAddr;
    unsigned short echoServPort;
    char* serverIP;
    char* echoString;
    char echoBuffer[RCVBUFSIZE];
    unsigned int echoStringLen;
    int bytesRcvd, totalBytesRcvd;

    if((argc<3)|| (argc>4)){
        fprintf(stderr,"Usage : %s <Server IP> <Eco Word> [<Eco port>]\n",argv[0]);
        exit(1);
    }
    serverIP= argv[1];
    echoString=argv[2];

    if(argc==4)
        echoServPort=atoi(argv[3]);
    else 
        echoServPort=7; /*7 is the well-known prot*/

    if((sock=socket(PF_INET,SOCK_STREAM, IPPROTO_TCP))<0)
        DieWithError("socket() failed");

    memset(&echoServAddr, 0, sizeof(echoServAddr));
    echoServAddr.sin_family=AF_INET;
    echoServAddr.sin_addr.s_addr=inet_addr(serverIP);
    echoServAddr.sin_port=htons(echoServPort);

    // Establish the connection to the echo server
    if(connect(sock,(struct sockaddr*)&echoServAddr, sizeof(echoServAddr))<0)
        DieWithError("connect() failed");

    echoStringLen=strlen(echoString);
    
    //send the string to the server
    if(send(sock,echoString,echoStringLen,0)!=echoStringLen)
        DieWithError("send() sent a different number of bytes than expected");

    //Recieve the same string back from the server
    totalBytesRcvd=0;
    printf("Received:");
    while(totalBytesRcvd < echoString){
    /*Receive up to the buffer size (minus 1 to leave space for a null terminator) bytes from the sender*/
        if((bytesRcvd=recv(sock,echoBuffer, RCVBUFSIZE-1,0))<=0)
            DieWithError("recv() failed or connection closed prematurely");
        totalBytesRcvd+=bytesRcvd;
        echoBuffer[bytesRcvd]='\0';
        printf(echoBuffer);
    }

    printf("\n");
    close(sock);
    exit(0);
}
