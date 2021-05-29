// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#define PORT 8080 
//This code is for I(a) part of the question
int main(int argc, char const *argv[]) 
{ 
	int sock = 0;
	struct sockaddr_in serv_addr; 
	// char *hello = "Hello from client"; 
	 
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 
	printf("CLIENT: Socket created...\n");
	serv_addr.sin_family = AF_INET; 
	serv_addr.sin_port = htons(PORT); 
	
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
		printf("\nConnection Failed \n"); 
		return -1; 
	} 
	printf("CLIENT: Connected to the Server successfully....\n");
    char para[1000];
    printf("Please enter the paragraph to send: ");
    scanf("%[^\n]", para);
	send(sock , para , strlen(para) , 0 ); 
	printf("CLIENT: Paragraph sent\n"); 
	char values[100] = {0};
	int valread1 = read( sock , values, 100); 
	printf("The number of character, words and sentences in the paragraph respectively are: %s\n", values);
	close(sock);
	printf("CLIENT: Connection closed\n"); 
	return 0; 
} 

