#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#include <time.h>
#include <errno.h> 
#include <netdb.h>
#include <signal.h> 
#include <assert.h>


#define MAXLINE 1024 

//I have used the given template and added the code in Q1 and Q2 at approprieate positions
//I have also changed the name of few variables as it was different in the code for toher questions

int max(int x, int y) 
{ 
    if (x > y) 
        return x; 
    else
        return y; 
} 


int main(int argc, char** argv) 
{ 

    if(argc < 2){
        printf("Enough arguments not provided.\n");
        exit (0);
    }

    int PORT = atoi(argv[1]);

    int listenfd, connfd, udpfd, nready, maxfdp1; 
    char buff[MAXLINE]; 
    pid_t childpid; 
    fd_set rset; 
    ssize_t n; 
    socklen_t len; 
    char str[100];
    const int on = 1; 
    struct sockaddr_in cliaddr, servaddr; 
    // char* message = "Hello Client"; 
    void sig_chld(int); 
  
    /* create listening TCP socket */
    listenfd = socket(AF_INET, SOCK_STREAM, 0); 
    bzero(&servaddr, sizeof(servaddr)); 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(PORT); 
  
    // binding server addr structure to listenfd 
    bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)); 
    listen(listenfd, 10); 
  
    /* create UDP socket */
    udpfd = socket(AF_INET, SOCK_DGRAM, 0); 
    // binding server addr structure to udp sockfd 
    bind(udpfd, (struct sockaddr*)&servaddr, sizeof(servaddr)); 
  
    // clear the descriptor set 
    FD_ZERO(&rset); 
  
    // get maxfd 
    maxfdp1 = max(listenfd, udpfd) + 1; 
    for (;;) { //infinite for loop
  
        // set listenfd and udpfd in readset 
        FD_SET(listenfd, &rset); 
        FD_SET(udpfd, &rset); 
  
        // select the ready descriptor 
        nready = select(maxfdp1, &rset, NULL, NULL, NULL); 
  
        // if tcp socket is readable then handle 

        // it by accepting the connection 
        if (FD_ISSET(listenfd, &rset)) { //Proccessing the time/tcp socket
            //Code is same as Q1
            connfd=accept(listenfd,(struct sockaddr*)&cliaddr,&len);
            printf("Accepted.\n");

            time_t t = time(NULL);              //I used some web resources for this
            struct tm *local_tm = localtime(&t);
            assert(strftime(str, sizeof(str), "%c", local_tm));
            printf("Date and time sent.\n");
            write(connfd,str,100);
        } 

        // if udp socket is readable receive the message. 
        if (FD_ISSET(udpfd, &rset)) {   //Proccessing the dns/udp socket
            //Code is same as Q1
            len = sizeof(cliaddr); //len is value/resuslt 
            n = recvfrom(udpfd, (char *)buff, MAXLINE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);

            buff[n-1] = '\0'; 
            /* gethostbyname() to find ip address for dns
                    put your code here */
            int i;
            struct hostent *he;
            struct in_addr addr;
            he = gethostbyname(buff);
            if (he == NULL) { // do some error checking
                herror("gethostbyname"); // herror(), NOT perror()
                exit(1);
            }

            // print information about this host:
            printf("Official name is: %s\n", he->h_name);
            printf("IP address: %s\n", inet_ntoa(*(struct in_addr*)he->h_addr));


            bzero (buff, MAXLINE);
            strcpy(buff, inet_ntoa(*(struct in_addr*)he->h_addr));	

            sendto(udpfd, (const char *)buff, strlen(buff), MSG_CONFIRM, (const struct sockaddr *) &cliaddr, len); 
        }
    } 
    return 0;

} 


//OUTPUT(server):
// Accepted.
// Date and time sent.
// Official name is: www.iitdh.ac.in
// IP address: 61.0.239.228


//OUTPUT(client1-time):
// Socket is Opened.
// Connected successfully.
// The local date and time is: Thu Mar 25 16:15:38 2021


//OUTPUT(client2-dns):
// DNS name to be entered here: www.iitdh.ac.in
// IP Address : 61.0.239.228


