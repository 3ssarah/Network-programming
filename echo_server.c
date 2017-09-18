#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAXPENDING 5

void DieWithError(char* errorMessage);
void HandleTCPClient(int clntSocket);
void log_msg(char* msg);
int main(int argc, char* argv[]){

    int servSock;
    int clntSock;
    struct sockaddr_in echoServAddr;
    struct sockaddr_in echoClntAddr;
    unsigned short echoServPort;
    unsigned int clntLen;

            
            if(argc!=2){
            //test for correct number ofargyments
            fprintf(stderr,"Usage: %s <server Port>\n", argv[0]);
            exit(1);
            }

    echoServPort =atoi(argv[1]);
    //create socket for incoming connections
    if((servSock=socket(PF_INET,SOCK_STREAM,IPPROTO_TCP))<0)
        DieWithError("socket() failed");
    //Construct loacl address structure
    
    memset(&echoServAddr,0x00,sizeof(echoServAddr));
   
    echoServAddr.sin_family=AF_INET;
    echoServAddr.sin_addr.s_addr=htonl(INADDR_ANY);
    echoServAddr.sin_port=htons(echoServPort);
    
    //Bind to the local address
    if(bind(servSock, (struct sockaddr*)&echoServAddr, sizeof(echoServAddr))<0)
        DieWithError("bind() failed");

    //Mark the socket so it will listen for incoming connections
    if(listen(servSock,MAXPENDING)<0)
        DieWithError("listen() failed");

    for(;;)
    {
        //set the size of the in-out parameter
        clntLen=sizeof(echoClntAddr);
        //wait for a client to connect
        if((clntSock=accept(servSock,(struct sockaddr*)&echoClntAddr,&clntLen))<0)
            DieWithError("accpet() failed");

        //clntSock is connected to a client!
        printf("Handling client %s \n",inet_ntoa(echoClntAddr.sin_addr));
        HandleTCPClient(clntSock);
            
    }
     printf("not reached\n");       

}


void log_msg(char *msg){

   FILE fp*;
   fp=fopen("history.log","a");
   fprintf(fp, "%s\n",msg);
   fclose(fp);
}
