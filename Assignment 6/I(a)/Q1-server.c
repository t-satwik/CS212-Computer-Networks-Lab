// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#define PORT 8080 
//This code is for I(a) part of the question
int main(int argc, char const *argv[]) 
{ 
	int server_fd, new_socket, valread; 
	struct sockaddr_in address; 
	int opt = 1; 
	int addrlen = sizeof(address); 
	char buffer[1024] = {0}; 
	char *hello = "Hello from server"; 
	
	// Creating socket file descriptor 
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 
	printf("SERVER: Socket created...\n");
	// Forcefully attaching socket to the port 8080 
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, 
												&opt, sizeof(opt))) 
	{ 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons( PORT ); 
	
	// Forcefully attaching socket to the port 8080 
	if (bind(server_fd, (struct sockaddr *)&address, 
								sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	}
	printf("SERVER: Socket binded successfully...\n");
	if (listen(server_fd, 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
	
	printf("SERVER: Server started listening for client at port number: 8080 ...\n");
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
					(socklen_t*)&addrlen))<0) 
	{ 
		perror("accept"); 
		exit(EXIT_FAILURE); 
	} 
	printf("SERVER: Accepted the connection from client...\n");

	valread = read( new_socket , buffer, 1024); 

	printf("The paragraph received from the client is: %s\n",buffer); 
    int numChars=strlen(buffer);
    int numWords=0, numSent=0;
    for(int i=0;buffer[i]!='\0';i++){
        if(buffer[i]==' ')
            numWords++;
        if(buffer[i]=='.')
            numSent++;
    }
    numWords++;
    char * values;
    sprintf(values, "%d, %d, %d", numChars, numWords, numSent);
    send(new_socket , values , strlen(values) , 0); 
    

	printf("SERVER: The calculated values(%s) are sent to the client\n", values); 

	close(new_socket);
	printf("SERVER: Connection closed\n");
 
	return 0; 
} 

