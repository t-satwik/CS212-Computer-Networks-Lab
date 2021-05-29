// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <stdlib.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#define PORT 8080 
//This code is for I(b) part of the question

int main(int argc, char const *argv[]){
	int sock = 0;
	struct sockaddr_in serv_addr; 
		
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){ 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 
	printf("CLIENT: Socket created...\n");
	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
		
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0){ 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	}	

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){ 
		printf("\nConnection Failed \n"); 
		return -1; 
	} 
	printf("CLIENT: Connected to the Server successfully. Please enter 'terminate' as the para input to complete the server program.\n");
	
	char para[1000];
	char * termination="terminate";


	

	printf("Please enter the paragraph to send: ");
	scanf("%[^\n]", para);
	send(sock , para , strlen(para) , 0 ); 
	printf("CLIENT: Paragraph sent\n"); 


	if(strcmp(para, termination)==0){
		close(sock);
		printf("CLIENT: Connection closed\n"); 
		printf("Server is closing the connection. Server program is also completely executed. It is going offline.\n");
		return 0;
	}

	else{
		char values[100] = {0};
		int valread1 = read( sock , values, 100); 
		printf("The number of character, words and sentences in the paragraph respectively are: %s\n", values);
		close(sock);
		printf("CLIENT: Connection closed\n"); 
	}

	return 0;
} 

