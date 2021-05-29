#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <time.h>
#include <assert.h>



int main(int argc, char** argv)
{
    if(argc < 2){
        printf("Enough arguments not provided.\n");
        exit (0);
    }
    
    int PORTN = atoi (argv[1]);

    struct sockaddr_in address;
    struct sockaddr_in cli;
    int sockfd,conntfd;
    int len,ch;
    char str[100];
    time_t tick;
    
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    
    if(sockfd<0){
        printf("error in socket\n");
        exit(0);
    }
    else 
        printf("Socket opened.\n");
    
    bzero(&address,sizeof(address));
    address.sin_port=htons(PORTN);
    address.sin_addr.s_addr=htonl(0);
    
    if(bind(sockfd,(struct sockaddr*)&address,sizeof(address))<0){
        printf("Error in binding\n");
    }
    
    else
        printf("Binded Successfully.\n");
    
    listen(sockfd,50);
    
    while (1){     //infinite while loop
        len=sizeof(ch);
        conntfd=accept(sockfd,(struct sockaddr*)&cli,&len);
        printf("Accepted.\n");

        time_t t = time(NULL);              //I used some web resources for this
        struct tm *local_tm = localtime(&t);
        assert(strftime(str, sizeof(str), "%c", local_tm));
        printf("Date and time sent.\n");
        write(conntfd,str,100);
    }

return 0;
}

//OUTPUT(server):
// Socket opened.
// Binded Successfully.
// Accepted.
// Date and time sent.

//OUTPUT(client):
// Socket is Opened.
// Connected successfully.
// The local date and time is: Thu Mar 25 16:11:57 2021



  
